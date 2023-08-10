#include <limits.h>
#include <stdlib.h>
#include "SpinTimerHwHandler.h"
#include "SpinTimerAction.h"
#include "SpinTimer.h"


//-----------------------------------------------------------------------------
// Declaration of private 
//-----------------------------------------------------------------------------

/**
 * @brief Evaluate time expired condition (private local function)
 */
static void SpinTimer_internalTick(SpinTimer* me);

/**
 * @brief Start the (new) interval (private local function)
 */
static void SpinTimer_startInterval(SpinTimer* me);

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------
SpinTimer* SpinTimer_create(SpinTimerMode mode)
{
    SpinTimer* me = malloc(sizeof(SpinTimer));
    SpinTimer_init(me, mode);

    return me;
}

void SpinTimer_init(SpinTimer* me, SpinTimerMode mode)
{
    me->attr.mode = mode;
    me->attr.isRunning = false;
    me->attr.isExpiredFlag = false;
    me->attr.willOverflow = false;
    me->attr.delayMicros = 0;
    me->attr.currentTimeMicros = 0;
    me->attr.triggerTimeMicros = 0;
    me->attr.triggerTimeMicrosUpperLimit = 0;
    me->attr.funcTMicros = 0;
    me->attr.hwHandler = 0;
    me->attr.action = 0;

    me->getMode = &SpinTimer_getMode;
    me->start = &SpinTimer_start;
    me->cancel = &SpinTimer_cancel;
    me->isRunning = &SpinTimer_isRunning;
    me->isExpired = &SpinTimer_isExpired;
    me->tick = &SpinTimer_tick;
    me->notifyExpired = &SpinTimer_notifyExpired;
    me->assignAction = &SpinTimer_assignAction;
    me->action = &SpinTimer_action;
    me->assignUptimeInfoCallout = &SpinTimer_assignUptimeInfoCallout;
    me->assignHwHandler = &SpinTimer_assignHwHandler;
}

void SpinTimer_destroy(SpinTimer* me)
{
    me->attr.funcTMicros = 0;
    me->attr.hwHandler = 0;
    me->attr.action = 0;
    free(me);
}

SpinTimerMode SpinTimer_getMode(SpinTimer* me)
{
    return me->attr.mode;
}

void SpinTimer_start(SpinTimer* me, uint32_t timeMicros)
{
    me->attr.isRunning = true;
    me->attr.delayMicros = timeMicros;

    if (0 != me->attr.hwHandler)
    {
        me->attr.hwHandler->start(me->attr.hwHandler, me->attr.delayMicros);
    }
    else
    {
        // this runs only as long as no SpinTimerHwHandler is assigned
        if (0 != me->attr.funcTMicros)
        {
            me->attr.currentTimeMicros = me->attr.funcTMicros();
        }
        SpinTimer_startInterval(me);
    }
}

void SpinTimer_cancel(SpinTimer* me)
{
    me->attr.isRunning = false;
    me->attr.isExpiredFlag = false;

    if (0 != me->attr.hwHandler)
    {
        me->attr.hwHandler->stop(me->attr.hwHandler);
    }
}

bool SpinTimer_isRunning(SpinTimer* me)
{
    return me->attr.isRunning;
}

bool SpinTimer_isExpired(SpinTimer* me)
{
    if (0 != me->attr.hwHandler)
    {
        me->attr.hwHandler->intControl(me->attr.hwHandler, SpinTimerHwHandlerIntAction_disable);
    }

    if (0 == me->attr.hwHandler)
    {
        // runs only as long as no SpinTimerHwHandler is assigned
        SpinTimer_internalTick(me);
    }
    bool isExpired = me->attr.isExpiredFlag;
    me->attr.isExpiredFlag = false;
    
    if (0 != me->attr.hwHandler)
    {
        me->attr.hwHandler->intControl(me->attr.hwHandler, SpinTimerHwHandlerIntAction_enable);
    }
    return isExpired;
}

void SpinTimer_tick(SpinTimer* me)
{
    if (0 == me->attr.hwHandler)
    {
        // runs only as long as no SpinTimerHwHandler is assigned
        SpinTimer_internalTick(me);
    }
}

void SpinTimer_notifyExpired(SpinTimer* me)
{
    if (0 != me)
    {
        // interval is over
        if (me->attr.mode == SpinTimerMode_continuous)
        {
            // in continuous mode
            if (0 == me->attr.hwHandler)
            {
                // start next interval (only as long as no SpinTimerHwHandler is assigned)
                SpinTimer_startInterval(me);
            }
        }
        else
        {
            // in one-shot mode
            me->attr.isRunning = false;
        }

        me->attr.isExpiredFlag = true;
        if (0 != me->attr.action)
        {
            me->attr.action->timeExpired(me->attr.action);
        }
    }
}

void SpinTimer_assignAction(SpinTimer* me, SpinTimerAction* action)
{
    me->attr.action = action;
}

SpinTimerAction* SpinTimer_action(SpinTimer* me)
{
    return me->attr.action;   
}

void SpinTimer_assignUptimeInfoCallout(SpinTimer* me, uint32_t (*tMicros)())
{
    if (0 != tMicros)
    {
        // mutual exclusion: SpinTimerHwHandler operation suppressed as soon as uptime lookup callout is assigned
        me->attr.hwHandler = 0;
    }
    me->attr.funcTMicros = tMicros;
}

void SpinTimer_assignHwHandler(SpinTimer* me, SpinTimerHwHandler* hwHandler)
{
    if (0 != hwHandler)
    {
        // mutual exclusion: uptime lookup suppressed as soon as SpinTimerHwHandler is assigned
        me->attr.funcTMicros = 0;
    }
    me->attr.hwHandler = hwHandler;
}

static void SpinTimer_startInterval(SpinTimer* me)
{
    uint32_t deltaTime = ULONG_MAX - me->attr.currentTimeMicros;
    me->attr.willOverflow = (deltaTime < me->attr.delayMicros);
    if (me->attr.willOverflow)
    {
        // overflow will occur
        me->attr.triggerTimeMicros = me->attr.delayMicros - deltaTime - 1;
        me->attr.triggerTimeMicrosUpperLimit = me->attr.currentTimeMicros;
    }
    else
    {
        me->attr.triggerTimeMicros = me->attr.currentTimeMicros + me->attr.delayMicros;
        me->attr.triggerTimeMicrosUpperLimit = ULONG_MAX - deltaTime;
    }
}

static void SpinTimer_internalTick(SpinTimer* me)
{
    bool intervalIsOver = false;

    if (0 != me->attr.funcTMicros)
    {
        me->attr.currentTimeMicros = me->attr.funcTMicros();

        // check if interval is over as long as the timer shall be running
        if (me->attr.isRunning)
        {
            if (me->attr.willOverflow)
            {
                intervalIsOver = ((me->attr.triggerTimeMicros <= me->attr.currentTimeMicros) && (me->attr.currentTimeMicros < me->attr.triggerTimeMicrosUpperLimit));
            }
            else
            {
                intervalIsOver = ((me->attr.triggerTimeMicros <= me->attr.currentTimeMicros) || (me->attr.currentTimeMicros < me->attr.triggerTimeMicrosUpperLimit));
            }
            
            if (intervalIsOver)
            {
                SpinTimer_notifyExpired(me);
            }
        }
    }
}
