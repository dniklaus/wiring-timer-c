#include <limits.h>
#include <stdlib.h>
#include "HwTimerHandler.h"
#include "SpinTimer.h"

// TODO: get this dependency out of here :-) !!
#include "project.h"

struct SpinTimer
{
    SpinTimerMode mode;
    bool m_isRunning;
    bool m_isExpiredFlag;
    bool m_willOverflow;
    uint32_t m_delayMicros;
    uint32_t m_currentTimeMicros;
    uint32_t m_triggerTimeMicros;
    uint32_t m_triggerTimeMicrosUpperLimit;
    void (*m_funcTimeExpired)();
    uint32_t (*m_funcTMicros)();
    HwTimerHandler* m_hwTimerHandler;
};

/**
 * @brief Evaluate time expired condition (private local function)
 */
void internalTick(SpinTimer* self);

/**
 * @brief Start the (new) interval (private local function)
 */
void startInterval(SpinTimer* self);

SpinTimer* SpinTimer_create(SpinTimerMode mode)
{
    SpinTimer* self = malloc(sizeof(SpinTimer));

    self->mode = mode;
    self->m_isRunning = false;
    self->m_isExpiredFlag = false;
    self->m_willOverflow = false;
    self->m_delayMicros = 0;
    self->m_currentTimeMicros = 0;
    self->m_triggerTimeMicros = 0;
    self->m_triggerTimeMicrosUpperLimit = 0;
    self->m_funcTimeExpired = 0;
    self->m_funcTMicros = 0;
    self->m_hwTimerHandler = 0;

    return self;
}

void SpinTimer_destroy(SpinTimer* self)
{
    self->m_funcTimeExpired = 0;
    self->m_funcTMicros = 0;
    self->m_hwTimerHandler = 0;
    free(self);
}

SpinTimerMode SpinTimer_getMode(SpinTimer* self)
{
    return self->mode;
}

void SpinTimer_start(SpinTimer* self, uint32_t timeMicros)
{
    self->m_isRunning = true;
    self->m_delayMicros = timeMicros;

    if (0 != self->m_hwTimerHandler)
    {
        if (0 != self->m_hwTimerHandler->start)
        {    
            self->m_hwTimerHandler->start(self->m_delayMicros);
        }
    }
    else
    {
        // this runs only as long as no HwTimerHandler is assigned
        if (0 != self->m_funcTMicros)
        {
            self->m_currentTimeMicros = self->m_funcTMicros();
        }
        startInterval(self);
    }
}

void SpinTimer_cancel(SpinTimer* self)
{
    self->m_isRunning = false;
    self->m_isExpiredFlag = false;

    if (0 != self->m_hwTimerHandler)
    {
        if (0 != self->m_hwTimerHandler->stop)
        {
            self->m_hwTimerHandler->stop();
        }
    }
}

bool SpinTimer_isRunning(SpinTimer* self)
{
    return self->m_isRunning;
}

bool SpinTimer_isExpired(SpinTimer* self)
{
    // TODO: get this dependency out of here :-) !!
    CyGlobalIntDisable;     

    if (0 == self->m_hwTimerHandler)
    {
        // runs only as long as no HwTimerHandler is assigned
        internalTick(self);
    }
    bool isExpired = self->m_isExpiredFlag;
    self->m_isExpiredFlag = false;
    
    // TODO: get this dependency out of here :-) !!
    CyGlobalIntEnable;

    return isExpired;
}

void SpinTimer_tick(SpinTimer* self)
{
    if (0 == self->m_hwTimerHandler)
    {
        // runs only as long as no HwTimerHandler is assigned
        internalTick(self);
    }
}

void SpinTimer_notifyExpired(SpinTimer* self)
{
    if (0 != self)
    {
        // interval is over
        if (self->mode == SpinTimerMode_continuous)
        {
            if (0 == self->m_hwTimerHandler)
            {
                // start next interval (only as long as no HwTimerHandler is assigned)
                startInterval(self);
            }
        }
        else
        {
            self->m_isRunning = false;
        }

        self->m_isExpiredFlag = true;
        if (0 != self->m_funcTimeExpired)
        {
            self->m_funcTimeExpired();
        }
    }
}

void SpinTimer_assignTimeExpiredCallback(SpinTimer* self, void (*timeExpired)())
{
    self->m_funcTimeExpired = timeExpired;
}

void SpinTimer_assignUptimeInfoCallout(SpinTimer* self, uint32_t (*tMicros)())
{
    if (0 != tMicros)
    {
        // mutual exclusion: HwTimerHandler operation suppressed as soon as uptime lookup callout is assigned
        self->m_hwTimerHandler = 0;
    }
    self->m_funcTMicros = tMicros;
}

void SpinTimer_assignHwTimerHandler(SpinTimer* self, HwTimerHandler* hwTimerHandler)
{
    if (0 != hwTimerHandler)
    {
        // mutual exclusion: uptime lookup suppressed as soon as HwTimerHandler is assigned
        self->m_funcTMicros = 0;
    }
    self->m_hwTimerHandler = hwTimerHandler;
}

void startInterval(SpinTimer* self)
{
    uint32_t deltaTime = ULONG_MAX - self->m_currentTimeMicros;
    self->m_willOverflow = (deltaTime < self->m_delayMicros);
    if (self->m_willOverflow)
    {
        // overflow will occur
        self->m_triggerTimeMicros = self->m_delayMicros - deltaTime - 1;
        self->m_triggerTimeMicrosUpperLimit = self->m_currentTimeMicros;
    }
    else
    {
        self->m_triggerTimeMicros = self->m_currentTimeMicros + self->m_delayMicros;
        self->m_triggerTimeMicrosUpperLimit = ULONG_MAX - deltaTime;
    }
}


void internalTick(SpinTimer* self)
{
    bool intervalIsOver = false;

    if (0 != self->m_funcTMicros)
    {
        self->m_currentTimeMicros = self->m_funcTMicros();

        // check if interval is over as long as the timer shall be running
        if (self->m_isRunning)
        {
            if (self->m_willOverflow)
            {
                intervalIsOver = ((self->m_triggerTimeMicros <= self->m_currentTimeMicros) && (self->m_currentTimeMicros < self->m_triggerTimeMicrosUpperLimit));
            }
            else
            {
                intervalIsOver = ((self->m_triggerTimeMicros <= self->m_currentTimeMicros) || (self->m_currentTimeMicros < self->m_triggerTimeMicrosUpperLimit));
            }
            
            if (intervalIsOver)
            {
                SpinTimer_notifyExpired(self);
            }
        }
    }
}
