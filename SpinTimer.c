#include <limits.h>
#include <stdlib.h>
#include "SpinTimerHwHandler.h"
#include "SpinTimer.h"

struct SpinTimer
{
    SpinTimerMode mode;
    bool isRunning;
    bool isExpiredFlag;
    bool willOverflow;
    uint32_t delayMicros;
    uint32_t currentTimeMicros;
    uint32_t triggerTimeMicros;
    uint32_t triggerTimeMicrosUpperLimit;
    void (*funcTimeExpired)();
    uint32_t (*funcTMicros)();
    SpinTimerHwHandler* hwHandler;
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
    self->isRunning = false;
    self->isExpiredFlag = false;
    self->willOverflow = false;
    self->delayMicros = 0;
    self->currentTimeMicros = 0;
    self->triggerTimeMicros = 0;
    self->triggerTimeMicrosUpperLimit = 0;
    self->funcTimeExpired = 0;
    self->funcTMicros = 0;
    self->hwHandler = 0;

    return self;
}

void SpinTimer_destroy(SpinTimer* self)
{
    self->funcTimeExpired = 0;
    self->funcTMicros = 0;
    self->hwHandler = 0;
    free(self);
}

SpinTimerMode SpinTimer_getMode(SpinTimer* self)
{
    return self->mode;
}

void SpinTimer_start(SpinTimer* self, uint32_t timeMicros)
{
    self->isRunning = true;
    self->delayMicros = timeMicros;

    if (0 != self->hwHandler)
    {
        if (0 != self->hwHandler->start)
        {    
            self->hwHandler->start(self->delayMicros);
        }
    }
    else
    {
        // this runs only as long as no SpinTimerHwHandler is assigned
        if (0 != self->funcTMicros)
        {
            self->currentTimeMicros = self->funcTMicros();
        }
        startInterval(self);
    }
}

void SpinTimer_cancel(SpinTimer* self)
{
    self->isRunning = false;
    self->isExpiredFlag = false;

    if (0 != self->hwHandler)
    {
        if (0 != self->hwHandler->stop)
        {
            self->hwHandler->stop();
        }
    }
}

bool SpinTimer_isRunning(SpinTimer* self)
{
    return self->isRunning;
}

bool SpinTimer_isExpired(SpinTimer* self)
{
    if ((0 != self->hwHandler) && (0 != self->hwHandler->intControl))
    {
        self->hwHandler->intControl(SpinTimerHwHandlerIntAction_disable);
    }

    if (0 == self->hwHandler)
    {
        // runs only as long as no SpinTimerHwHandler is assigned
        internalTick(self);
    }
    bool isExpired = self->isExpiredFlag;
    self->isExpiredFlag = false;
    
    if ((0 != self->hwHandler) && (0 != self->hwHandler->intControl))
    {
        self->hwHandler->intControl(SpinTimerHwHandlerIntAction_enable);
    }

    return isExpired;
}

void SpinTimer_tick(SpinTimer* self)
{
    if (0 == self->hwHandler)
    {
        // runs only as long as no SpinTimerHwHandler is assigned
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
            if (0 == self->hwHandler)
            {
                // start next interval (only as long as no SpinTimerHwHandler is assigned)
                startInterval(self);
            }
        }
        else
        {
            self->isRunning = false;
        }

        self->isExpiredFlag = true;
        if (0 != self->funcTimeExpired)
        {
            self->funcTimeExpired();
        }
    }
}

void SpinTimer_assignTimeExpiredCallback(SpinTimer* self, void (*timeExpired)())
{
    self->funcTimeExpired = timeExpired;
}

void SpinTimer_assignUptimeInfoCallout(SpinTimer* self, uint32_t (*tMicros)())
{
    if (0 != tMicros)
    {
        // mutual exclusion: SpinTimerHwHandler operation suppressed as soon as uptime lookup callout is assigned
        self->hwHandler = 0;
    }
    self->funcTMicros = tMicros;
}

void SpinTimer_assignHwHandler(SpinTimer* self, SpinTimerHwHandler* hwHandler)
{
    if (0 != hwHandler)
    {
        // mutual exclusion: uptime lookup suppressed as soon as SpinTimerHwHandler is assigned
        self->funcTMicros = 0;
    }
    self->hwHandler = hwHandler;
}

void startInterval(SpinTimer* self)
{
    uint32_t deltaTime = ULONG_MAX - self->currentTimeMicros;
    self->willOverflow = (deltaTime < self->delayMicros);
    if (self->willOverflow)
    {
        // overflow will occur
        self->triggerTimeMicros = self->delayMicros - deltaTime - 1;
        self->triggerTimeMicrosUpperLimit = self->currentTimeMicros;
    }
    else
    {
        self->triggerTimeMicros = self->currentTimeMicros + self->delayMicros;
        self->triggerTimeMicrosUpperLimit = ULONG_MAX - deltaTime;
    }
}


void internalTick(SpinTimer* self)
{
    bool intervalIsOver = false;

    if (0 != self->funcTMicros)
    {
        self->currentTimeMicros = self->funcTMicros();

        // check if interval is over as long as the timer shall be running
        if (self->isRunning)
        {
            if (self->willOverflow)
            {
                intervalIsOver = ((self->triggerTimeMicros <= self->currentTimeMicros) && (self->currentTimeMicros < self->triggerTimeMicrosUpperLimit));
            }
            else
            {
                intervalIsOver = ((self->triggerTimeMicros <= self->currentTimeMicros) || (self->currentTimeMicros < self->triggerTimeMicrosUpperLimit));
            }
            
            if (intervalIsOver)
            {
                SpinTimer_notifyExpired(self);
            }
        }
    }
}
