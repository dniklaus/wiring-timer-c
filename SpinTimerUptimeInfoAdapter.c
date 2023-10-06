/*
 * SpinTimerUptimeInfoAdapter.c
 *
 *  Created on: 20.09.2023
 *      Author: dniklaus
 */

#include <assert.h>
#include "SpinTimerUptimeInfoAdapter.h"

/* pure virtual functions, local (not accessible outside this source file) */
static uint32_t SpinTimerUptimeInfoAdapter_currentTimeMicros(SpinTimerUptimeInfoAdapter const* const me);
static uint32_t SpinTimerUptimeInfoAdapter_maxTimeValue(SpinTimerUptimeInfoAdapter const* const me);
static uint32_t SpinTimerUptimeInfoAdapter_microsPerTick(SpinTimerUptimeInfoAdapter const* const me);

void SpinTimerUptimeInfoAdapter_init(SpinTimerUptimeInfoAdapter* const me)
{
    /* static const (immutable) virtual function table allocation, kept in ROM */
    static const SpinTimerUptimeInfoAdapterVtable vtable = {
        &SpinTimerUptimeInfoAdapter_currentTimeMicros,  /* initialize pointer to local pure virtual function currentTimeMicros() */
        &SpinTimerUptimeInfoAdapter_maxTimeValue,       /* initialize pointer to local pure virtual function maxTimeValue() */
        &SpinTimerUptimeInfoAdapter_microsPerTick       /* initialize pointer to local pure virtual function microsPerTick() */
    };
    me->vptr = &vtable;                                 /* default assignment to own virtual table */
    
    me->currentTimeMicros = &SpinTimerUptimeInfoAdapter_currentTimeMicros_vcall;    /* assign I/F with pointer to inline virtual call */
    me->maxTimeValue = &SpinTimerUptimeInfoAdapter_maxTimeValue_vcall;              /* assign I/F with pointer to inline virtual call */
    me->microsPerTick = &SpinTimerUptimeInfoAdapter_microsPerTick_vcall;            /* assign I/F with pointer to inline virtual call */
}

/* empty "implementation" of the pure virtual function currentTimeMicros() */
static uint32_t SpinTimerUptimeInfoAdapter_currentTimeMicros(SpinTimerUptimeInfoAdapter const* const me)
{
    (void)me;   /* avoid compiler warnings regarding unused parameter */
    assert(0);  /* pure virtual function, shall never be called */
    return 0;
}

/* empty "implementation" of the pure virtual function maxTimeValue() */
static uint32_t SpinTimerUptimeInfoAdapter_maxTimeValue(SpinTimerUptimeInfoAdapter const* const me)
{
    (void)me;   /* avoid compiler warnings regarding unused parameter */
    assert(0);  /* pure virtual function, shall never be called */
    return 0;
}

/* empty "implementation" of the pure virtual function microsPerTick() */
static uint32_t SpinTimerUptimeInfoAdapter_microsPerTick(SpinTimerUptimeInfoAdapter const* const me)
{
    (void)me;   /* avoid compiler warnings regarding unused parameter */
    assert(0);  /* pure virtual function, shall never be called */
    return 0;
}
