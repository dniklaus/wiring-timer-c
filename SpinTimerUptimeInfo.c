/*
 * SpinTimerUptimeInfo.c
 *
 *  Created on: 20.09.2023
 *      Author: dniklaus
 */
#include "SpinTimerUptimeInfo.h"


// Private functions
SpinTimerUptimeInfo* SpinTimerUptimeInfo_create();
void SpinTimerUptimeInfo_init(SpinTimerUptimeInfo* me);

// Singleton
static SpinTimerUptimeInfo* s_spinTimerUptimeInfo = 0;

SpinTimerUptimeInfo* SpinTimerUptimeInfo_instance()
{
    if (0 == s_spinTimerUptimeInfo)
    {
        s_spinTimerUptimeInfo = SpinTimerUptimeInfo_create();
    }
    return s_spinTimerUptimeInfo;
}

SpinTimerUptimeInfo* SpinTimerUptimeInfo_create()
{
    SpinTimerUptimeInfo* me = malloc(sizeof(SpinTimerUptimeInfo));
    SpinTimerUptimeInfo_init(me);
    return me;
}

void SpinTimerUptimeInfo_init(SpinTimerUptimeInfo* me)
{
    me->attr.adapter = 0;
    me->assignAdapter = &SpinTimerUptimeInfo_assignAdapter;
    me->adapter = &SpinTimerUptimeInfo_adapter;
    me->currentTimeMicros = &SpinTimerUptimeInfo_currentTimeMicros;
    me->maxTimeValue = &SpinTimerUptimeInfo_maxTimeValue;
    me->microsPerTick = &SpinTimerUptimeInfo_microsPerTick;
}

void SpinTimerUptimeInfo_destroy()
{
    SpinTimerUptimeInfo* me = SpinTimerUptimeInfo_instance();
    s_spinTimerUptimeInfo = 0;
    free(me);
}

void SpinTimerUptimeInfo_assignAdapter(SpinTimerUptimeInfo* me, SpinTimerUptimeInfoAdapter* adapter)
{
    me->attr.adapter = adapter;
}

SpinTimerUptimeInfoAdapter* SpinTimerUptimeInfo_adapter(SpinTimerUptimeInfo* me)
{
    return me->attr.adapter;
}