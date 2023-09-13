#include <limits.h>
#include <stdlib.h>
#include "SpinTimerHwHandler.h"
#include "SpinTimerAction.h"
#include "SpinTimer.h"

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
    me->attr.hwHandler = 0;
    me->attr.action = 0;

    me->getMode = &SpinTimer_getMode;
    me->start = &SpinTimer_start;
    me->cancel = &SpinTimer_cancel;
    me->isRunning = &SpinTimer_isRunning;
    me->isExpired = &SpinTimer_isExpired;
    me->notifyExpired = &SpinTimer_notifyExpired;
    me->assignAction = &SpinTimer_assignAction;
    me->action = &SpinTimer_action;
    me->assignHwHandler = &SpinTimer_assignHwHandler;
}

void SpinTimer_destroy(SpinTimer* me)
{
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

    bool isExpired = me->attr.isExpiredFlag;
    me->attr.isExpiredFlag = false;
    
    if (0 != me->attr.hwHandler)
    {
        me->attr.hwHandler->intControl(me->attr.hwHandler, SpinTimerHwHandlerIntAction_enable);
    }
    return isExpired;
}

void SpinTimer_notifyExpired(SpinTimer* me)
{
    if (0 != me)
    {
        // interval is over
        if (me->attr.mode == SpinTimerMode_oneShot)
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

void SpinTimer_assignHwHandler(SpinTimer* me, SpinTimerHwHandler* hwHandler)
{
    me->attr.hwHandler = hwHandler;
}
