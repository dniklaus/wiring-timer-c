#include <limits.h>
#include <stdlib.h>
#include "SpinTimerContext.h"
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
    me->attr.maxUptimeValue = SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance());
    me->attr.microsPerTick = SpinTimerUptimeInfo_instance()->microsPerTick(SpinTimerUptimeInfo_instance());
    me->attr.action = 0;
    me->attr.next = 0;

    me->getMode = &SpinTimer_getMode;
    me->start = &SpinTimer_start;
    me->cancel = &SpinTimer_cancel;
    me->isRunning = &SpinTimer_isRunning;
    me->isExpired = &SpinTimer_isExpired;
    me->tick = &SpinTimer_tick;
    me->notifyExpired = &SpinTimer_notifyExpired;
    me->assignAction = &SpinTimer_assignAction;
    me->action = &SpinTimer_action;
    me->next = &SpinTimer_next;
    me->setNext = &SpinTimer_setNext;

    SpinTimerContext_instance()->attach(SpinTimerContext_instance(), me);
}

void SpinTimer_destroy(SpinTimer* me)
{
    me->attr.action = 0;
    SpinTimerContext_instance()->detach(SpinTimerContext_instance(), me);
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
    
    SpinTimerUptimeInfo* uptimeInfo = SpinTimerUptimeInfo_instance();
    if (0 != uptimeInfo)
    {
        me->attr.currentTimeMicros = uptimeInfo->currentTimeMicros(uptimeInfo);
        if (timeMicros > me->attr.maxUptimeValue)
        {
            // limit time value to the max
            me->attr.delayMicros = me->attr.maxUptimeValue;
        }
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
            me->attr.currentTimeMicros = me->attr.triggerTimeMicros;    // interval end becomes the new interval beginning
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
    uint32_t timeUntilOverflow = me->attr.maxUptimeValue - me->attr.currentTimeMicros;
    me->attr.willOverflow = timeUntilOverflow < me->attr.delayMicros;
    if (me->attr.willOverflow)
    {
        // overflow will occur
        me->attr.triggerTimeMicros = me->attr.delayMicros - timeUntilOverflow - me->attr.microsPerTick;
    }
    else
    {
        me->attr.triggerTimeMicros = me->attr.currentTimeMicros + me->attr.delayMicros;
    }        
}

static void SpinTimer_internalTick(SpinTimer* me)
{
    if (me->attr.isRunning )
    {
        //  check as long as the timer shall be running only
        if (0 != SpinTimerUptimeInfo_instance())
        {
            // uptime info object is ready, get current time count
            uint32_t currentTimeMicros = SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance());
            if (me->attr.willOverflow)
            {
                if (currentTimeMicros < me->attr.currentTimeMicros)
                {
                    me->attr.willOverflow = false;
                }
            }
            me->attr.currentTimeMicros = currentTimeMicros;

            if ((!me->attr.willOverflow) && (me->attr.triggerTimeMicros <= me->attr.currentTimeMicros))
            {
                // interval is over
                me->notifyExpired(me);
            }
        }
    }
}

SpinTimer* SpinTimer_next(SpinTimer* me)
{
  return me->attr.next;
}

void SpinTimer_setNext(SpinTimer* me, SpinTimer* timer)
{
  me->attr.next = timer;
}
