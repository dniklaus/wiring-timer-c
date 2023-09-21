#include <limits.h>
#include <stdlib.h>
#include "SpinTimerUptimeInfo.h"
#include "SpinTimerAction.h"
#include "SpinTimer.h"

//-----------------------------------------------------------------------------
// Declaration of private 
//-----------------------------------------------------------------------------
/**
 * @brief Start the (new) interval (private local function)
 */
static void SpinTimer_startInterval(SpinTimer* me);

/**
 * @brief Internal tick method, evaluates the expired state.
 */
static void SpinTimer_internalTick(SpinTimer* me);

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
    me->attr.maxUptimeValue = 0;
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
}

void SpinTimer_destroy(SpinTimer* me)
{
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
    
    if (0 != SpinTimerUptimeInfo_instance())
    {
        me->attr.currentTimeMicros = SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance());
        me->attr.maxUptimeValue = SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance());
    }
    
    SpinTimer_startInterval(me);
}

void SpinTimer_cancel(SpinTimer* me)
{
    me->attr.isRunning = false;
    me->attr.isExpiredFlag = false;
}

bool SpinTimer_isRunning(SpinTimer* me)
{
    return me->attr.isRunning;
}

bool SpinTimer_isExpired(SpinTimer* me)
{
    SpinTimer_internalTick(me);
    bool isExpired = me->attr.isExpiredFlag;
    me->attr.isExpiredFlag = false;
    return isExpired;
}

void SpinTimer_tick(SpinTimer* me)
{
    SpinTimer_internalTick(me);
}

void SpinTimer_notifyExpired(SpinTimer* me)
{
    if (0 != me)
    {
        // interval is over
        if (me->attr.mode == SpinTimerMode_continuous)
        {
            // start next interval
            SpinTimer_startInterval(me);
        }
        else
        {
            // in one-shot mode: timer gets stopped now
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

static void SpinTimer_startInterval(SpinTimer* me)
{
    if (0 != SpinTimerUptimeInfo_instance())
    {
        uint32_t deltaTime = me->attr.maxUptimeValue - me->attr.currentTimeMicros;
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
            me->attr.triggerTimeMicrosUpperLimit = me->attr.maxUptimeValue - deltaTime;
        }        
    }
}

static void SpinTimer_internalTick(SpinTimer* me)
{
    bool intervalIsOver = false;

    if (0 != SpinTimerUptimeInfo_instance())
    {
        // uptime info object is ready
        me->attr.currentTimeMicros = SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance());

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
                me->notifyExpired(me);
            }
        }
    }
}
