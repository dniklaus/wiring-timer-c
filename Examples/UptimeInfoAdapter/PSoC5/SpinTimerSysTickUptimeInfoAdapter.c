/*
 * SpinTimerSysTickUptimeInfoAdapter.cpp
 *
 *  Created on: 20.09.2023
 *      Author: dniklaus
 */

#include <stdlib.h>
#include <project.h>
#include "SpinTimerSysTickUptimeInfoAdapter.h"

static const uint32_t c_microsPerTick = 10;         /// System Tick time: 10 us
static const uint32_t c_maxCount = (1 << 24) - 3;   /// System Tick Counter Period: (1 << 24) - 2, max count is one less.

static uint32_t s_sysTickCount = 0;

SpinTimerSysTickUptimeInfoAdapter* SpinTimerSysTickUptimeInfoAdapter_create()
{
    SpinTimerSysTickUptimeInfoAdapter* me = malloc(sizeof(SpinTimerSysTickUptimeInfoAdapter));
    SpinTimerSysTickUptimeInfoAdapter_init(me);
    return me;
}

void SpinTimerSysTickUptimeInfoAdapter_init(SpinTimerSysTickUptimeInfoAdapter* const me)
{
    /* static const (immutable) virtual function table allocation, kept in ROM */
    static const SpinTimerUptimeInfoAdapterVtable vtable = {
        (uint32_t (*)(SpinTimerUptimeInfoAdapter const* const me))&SpinTimerSysTickUptimeInfoAdapter_currentTimeMicros,   /* init ptr to own currentTimeMicros() impl. with "me" upcast */
        (uint32_t (*)(SpinTimerUptimeInfoAdapter const* const me))&SpinTimerSysTickUptimeInfoAdapter_maxTimeValue,        /* init ptr to own maxTimeValue() impl. with "me" upcast */
        (uint32_t (*)(SpinTimerUptimeInfoAdapter const* const me))&SpinTimerSysTickUptimeInfoAdapter_microsPerTick        /* init ptr to own microsPerTick() impl. with "me" upcast */
    };
    SpinTimerUptimeInfoAdapter_init(&me->super);
    me->super.vptr = &vtable;                                 /* assign own vtable to SpinTimerUptimeInfoAdapter's vptr (overwrite) */
}

void SpinTimerSysTickUptimeInfoAdapter_destroy(SpinTimerSysTickUptimeInfoAdapter* const me)
{
    free(me);
}

/* SpinTimerSysTickUptimeInfoAdapter's specific implementation of currentTimeMicros() */
uint32_t SpinTimerSysTickUptimeInfoAdapter_currentTimeMicros(SpinTimerSysTickUptimeInfoAdapter const* const me)
{
    (void)me;
    CyGlobalIntDisable;
    uint32_t currentTimeMicros = s_sysTickCount * c_microsPerTick;
    CyGlobalIntEnable;
    return currentTimeMicros;
}

/* SpinTimerSysTickUptimeInfoAdapter's specific implementation of maxTimeValue() */
uint32_t SpinTimerSysTickUptimeInfoAdapter_maxTimeValue(SpinTimerSysTickUptimeInfoAdapter const* const me)
{
    (void)me;
    return c_maxCount * c_microsPerTick;
}

uint32_t SpinTimerSysTickUptimeInfoAdapter_microsPerTick(SpinTimerSysTickUptimeInfoAdapter const* const me)
{
    (void)me;
    return c_microsPerTick;
}

void SpinTimerSysTickUptimeInfoAdapter_updateTickCount()
{
    CyGlobalIntDisable;
    s_sysTickCount = Counter_SystemTick_ReadCounter();
    CyGlobalIntEnable;
}
