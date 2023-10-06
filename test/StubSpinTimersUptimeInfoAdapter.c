/*
 * StubSpinTimersUptimeInfoAdapter.cpp
 *
 *  Created on: 20.09.2023
 *      Author: dniklaus
 */

#include <stdlib.h>
#include "StubSpinTimersUptimeInfoAdapter.h"

static const uint32_t c_microsPerTick = 50;
static const uint32_t c_maxCount = ((1 << 24) - 3);

StubSpinTimersUptimeInfoAdapter* StubSpinTimersUptimeInfoAdapter_create()
{
    StubSpinTimersUptimeInfoAdapter* me = malloc(sizeof(StubSpinTimersUptimeInfoAdapter));
    StubSpinTimersUptimeInfoAdapter_init(me);
    return me;
}

void StubSpinTimersUptimeInfoAdapter_init(StubSpinTimersUptimeInfoAdapter* const me)
{
    /* static const (immutable) virtual function table allocation, kept in ROM */
    static const SpinTimerUptimeInfoAdapterVtable vtable = {
        (uint32_t (*)(SpinTimerUptimeInfoAdapter const* const me))&StubSpinTimersUptimeInfoAdapter_currentTimeMicros,   /* init ptr to own currentTimeMicros() impl. with "me" upcast */
        (uint32_t (*)(SpinTimerUptimeInfoAdapter const* const me))&StubSpinTimersUptimeInfoAdapter_maxTimeValue,        /* init ptr to own maxTimeValue() impl. with "me" upcast */
        (uint32_t (*)(SpinTimerUptimeInfoAdapter const* const me))&StubSpinTimersUptimeInfoAdapter_microsPerTick        /* init ptr to own microsPerTick() impl. with "me" upcast */
    };
    SpinTimerUptimeInfoAdapter_init(&me->super);
    me->super.vptr = &vtable;                                 /* assign own vtable to SpinTimerUptimeInfoAdapter's vptr (overwrite) */
    me->attr.timeMicros = 0;
    me->setTimeMicros = &StubSpinTimersUptimeInfoAdapter_setTimeMicros;
    me->incrementCount = &StubSpinTimersUptimeInfoAdapter_incrementCount;
}

void StubSpinTimersUptimeInfoAdapter_destroy(StubSpinTimersUptimeInfoAdapter* const me)
{
    free(me);
}

void StubSpinTimersUptimeInfoAdapter_setTimeMicros(StubSpinTimersUptimeInfoAdapter* me,  uint32_t timeMicros)
{
    if (c_maxCount * c_microsPerTick >= timeMicros)
    {
        me->attr.timeMicros = timeMicros;
    }
    else
    {
        me->attr.timeMicros = c_maxCount * c_microsPerTick;
    }
}

void StubSpinTimersUptimeInfoAdapter_incrementCount(StubSpinTimersUptimeInfoAdapter* me)
{
    if ((me->attr.timeMicros + c_microsPerTick) <= c_maxCount * c_microsPerTick)
    {
        me->attr.timeMicros += c_microsPerTick;
    }
    else
    {
        // wrap around
        me->attr.timeMicros = 0;
    }
}

/* StubSpinTimersUptimeInfoAdapter's specific implementation of currentTimeMicros() */
uint32_t StubSpinTimersUptimeInfoAdapter_currentTimeMicros(StubSpinTimersUptimeInfoAdapter const* const me)
{
    (void)me;
    return me->attr.timeMicros;
}

/* StubSpinTimersUptimeInfoAdapter's specific implementation of maxTimeValue() */
uint32_t StubSpinTimersUptimeInfoAdapter_maxTimeValue(StubSpinTimersUptimeInfoAdapter const* const me)
{
    (void)me;
    return c_maxCount * c_microsPerTick;
}

uint32_t StubSpinTimersUptimeInfoAdapter_microsPerTick(StubSpinTimersUptimeInfoAdapter const* const me)
{
    (void)me;
    return c_microsPerTick;
}
