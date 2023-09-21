/*
 * SpinTimerUptimeInfo.c
 *
 *  Created on: 20.09.2023
 *      Author: dniklaus
 */
#include "SpinTimerUptimeInfo.h"

// Singleton
static SpinTimerUptimeInfo* s_spinTimerUptimeInfo = 0;

SpinTimerUptimeInfo* SpinTimerUptimeInfo_instance()
{
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
    s_spinTimerUptimeInfo = me;
    
    me->attr.adapter = 0;
    me->setAdapter = &SpinTimerUptimeInfo_setAdapter;
    me->currentTimeMicros = &SpinTimerUptimeInfo_currentTimeMicros;
    me->maxTimeValue = &SpinTimerUptimeInfo_maxTimeValue;
}

void SpinTimerUptimeInfo_destroy(SpinTimerUptimeInfo* me)
{
    free(me);
}

void SpinTimerUptimeInfo_setAdapter(SpinTimerUptimeInfo* me, SpinTimerUptimeInfoAdapter* adapter)
{
    me->attr.adapter = adapter;
}

