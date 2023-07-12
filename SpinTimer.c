#include <limits.h>
#include <stdlib.h>
#include "SpinTimerHwHandler.h"
#include "SpinTimer.h"


//-----------------------------------------------------------------------------
// Declaration of private 
//-----------------------------------------------------------------------------
typedef struct SpinTimerAttributes
{
    SpinTimerMode mode;
    bool isRunning;
    bool isExpiredFlag;
    bool willOverflow;
    uint32_t delayMicros;
    uint32_t currentTimeMicros;
    uint32_t triggerTimeMicros;
    uint32_t triggerTimeMicrosUpperLimit;
    uint32_t (*funcTMicros)();
    SpinTimerHwHandler* hwHandler;
    SpinTimerAction* action;
} SpinTimerHwHandlerAttributes;

/**
 * @brief Evaluate time expired condition (private local function)
 */
static void SpinTimer_internalTick(SpinTimer* self);

/**
 * @brief Start the (new) interval (private local function)
 */
static void SpinTimer_startInterval(SpinTimer* self);

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------
SpinTimer* SpinTimer_create(SpinTimerMode mode)
{
    SpinTimer* self = malloc(sizeof(SpinTimer));
    SpinTimerHwHandlerAttributes* attr = malloc(sizeof(SpinTimerHwHandlerAttributes));

    self->attr = attr;

    self->attr->mode = mode;
    self->attr->isRunning = false;
    self->attr->isExpiredFlag = false;
    self->attr->willOverflow = false;
    self->attr->delayMicros = 0;
    self->attr->currentTimeMicros = 0;
    self->attr->triggerTimeMicros = 0;
    self->attr->triggerTimeMicrosUpperLimit = 0;
    self->attr->funcTMicros = 0;
    self->attr->hwHandler = 0;
    self->attr->action = 0;

    self->destroy = &SpinTimer_destroy;
    self->getMode = &SpinTimer_getMode;
    self->start = &SpinTimer_start;
    self->cancel = &SpinTimer_cancel;
    self->isRunning = &SpinTimer_isRunning;
    self->isExpired = &SpinTimer_isExpired;
    self->tick = &SpinTimer_tick;
    self->notifyExpired = &SpinTimer_notifyExpired;
    self->assignAction = &SpinTimer_assignAction;
    self->action = &SpinTimer_action;
    self->assignUptimeInfoCallout = &SpinTimer_assignUptimeInfoCallout;
    self->assignHwHandler = &SpinTimer_assignHwHandler;

    return self;
}

void SpinTimer_destroy(SpinTimer* self)
{
    self->attr->funcTMicros = 0;
    self->attr->hwHandler = 0;
    self->attr->action = 0;
    free(self->attr);
    free(self);
}

SpinTimerMode SpinTimer_getMode(SpinTimer* self)
{
    return self->attr->mode;
}

void SpinTimer_start(SpinTimer* self, uint32_t timeMicros)
{
    self->attr->isRunning = true;
    self->attr->delayMicros = timeMicros;

    if (0 != self->attr->hwHandler)
    {
        self->attr->hwHandler->start(self->attr->hwHandler, self->attr->delayMicros);
    }
    else
    {
        // this runs only as long as no SpinTimerHwHandler is assigned
        if (0 != self->attr->funcTMicros)
        {
            self->attr->currentTimeMicros = self->attr->funcTMicros();
        }
        SpinTimer_startInterval(self);
    }
}

void SpinTimer_cancel(SpinTimer* self)
{
    self->attr->isRunning = false;
    self->attr->isExpiredFlag = false;

    if (0 != self->attr->hwHandler)
    {
        self->attr->hwHandler->stop(self->attr->hwHandler);
    }
}

bool SpinTimer_isRunning(SpinTimer* self)
{
    return self->attr->isRunning;
}

bool SpinTimer_isExpired(SpinTimer* self)
{
    if (0 != self->attr->hwHandler)
    {
        self->attr->hwHandler->intControl(self->attr->hwHandler, SpinTimerHwHandlerIntAction_disable);
    }

    if (0 == self->attr->hwHandler)
    {
        // runs only as long as no SpinTimerHwHandler is assigned
        SpinTimer_internalTick(self);
    }
    bool isExpired = self->attr->isExpiredFlag;
    self->attr->isExpiredFlag = false;
    
    if (0 != self->attr->hwHandler)
    {
        self->attr->hwHandler->intControl(self->attr->hwHandler, SpinTimerHwHandlerIntAction_enable);
    }
    return isExpired;
}

void SpinTimer_tick(SpinTimer* self)
{
    if (0 == self->attr->hwHandler)
    {
        // runs only as long as no SpinTimerHwHandler is assigned
        SpinTimer_internalTick(self);
    }
}

void SpinTimer_notifyExpired(SpinTimer* self)
{
    if (0 != self)
    {
        // interval is over
        if (self->attr->mode == SpinTimerMode_continuous)
        {
            if (0 == self->attr->hwHandler)
            {
                // start next interval (only as long as no SpinTimerHwHandler is assigned)
                SpinTimer_startInterval(self);
            }
        }
        else
        {
            self->attr->isRunning = false;
        }

        self->attr->isExpiredFlag = true;
        if (0 != self->attr->action)
        {
            self->attr->action->timeExpired(self->attr->action);
        }
    }
}

void SpinTimer_assignAction(SpinTimer* self, SpinTimerAction* action)
{
    self->attr->action = action;
}

SpinTimerAction* SpinTimer_action(SpinTimer* self)
{
    return self->attr->action;   
}

void SpinTimer_assignUptimeInfoCallout(SpinTimer* self, uint32_t (*tMicros)())
{
    if (0 != tMicros)
    {
        // mutual exclusion: SpinTimerHwHandler operation suppressed as soon as uptime lookup callout is assigned
        self->attr->hwHandler = 0;
    }
    self->attr->funcTMicros = tMicros;
}

void SpinTimer_assignHwHandler(SpinTimer* self, SpinTimerHwHandler* hwHandler)
{
    if (0 != hwHandler)
    {
        // mutual exclusion: uptime lookup suppressed as soon as SpinTimerHwHandler is assigned
        self->attr->funcTMicros = 0;
    }
    self->attr->hwHandler = hwHandler;
}

void SpinTimer_startInterval(SpinTimer* self)
{
    uint32_t deltaTime = ULONG_MAX - self->attr->currentTimeMicros;
    self->attr->willOverflow = (deltaTime < self->attr->delayMicros);
    if (self->attr->willOverflow)
    {
        // overflow will occur
        self->attr->triggerTimeMicros = self->attr->delayMicros - deltaTime - 1;
        self->attr->triggerTimeMicrosUpperLimit = self->attr->currentTimeMicros;
    }
    else
    {
        self->attr->triggerTimeMicros = self->attr->currentTimeMicros + self->attr->delayMicros;
        self->attr->triggerTimeMicrosUpperLimit = ULONG_MAX - deltaTime;
    }
}

void SpinTimer_internalTick(SpinTimer* self)
{
    bool intervalIsOver = false;

    if (0 != self->attr->funcTMicros)
    {
        self->attr->currentTimeMicros = self->attr->funcTMicros();

        // check if interval is over as long as the timer shall be running
        if (self->attr->isRunning)
        {
            if (self->attr->willOverflow)
            {
                intervalIsOver = ((self->attr->triggerTimeMicros <= self->attr->currentTimeMicros) && (self->attr->currentTimeMicros < self->attr->triggerTimeMicrosUpperLimit));
            }
            else
            {
                intervalIsOver = ((self->attr->triggerTimeMicros <= self->attr->currentTimeMicros) || (self->attr->currentTimeMicros < self->attr->triggerTimeMicrosUpperLimit));
            }
            
            if (intervalIsOver)
            {
                SpinTimer_notifyExpired(self);
            }
        }
    }
}
