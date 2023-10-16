/*
 * SpinTimerSysTickUptimeInfoAdapter.h
 *
 *  Created on: 20.09.2023
 *      Author: dniklaus
 */

#ifndef SpinTimerSysTickUptimeInfoAdapter_H_
#define SpinTimerSysTickUptimeInfoAdapter_H_

#include "SpinTimerUptimeInfoAdapter.h"

/**
 * Adapter Interface, call-out to get the platform specific up-time info time every system tick.
 */

typedef struct SpinTimerSysTickUptimeInfoAdapter SpinTimerSysTickUptimeInfoAdapter;
struct SpinTimerSysTickUptimeInfoAdapter
{
    SpinTimerUptimeInfoAdapter super;
};

/* Function prototypes */
SpinTimerSysTickUptimeInfoAdapter* SpinTimerSysTickUptimeInfoAdapter_create();
void SpinTimerSysTickUptimeInfoAdapter_init(SpinTimerSysTickUptimeInfoAdapter* const me);
void SpinTimerSysTickUptimeInfoAdapter_destroy(SpinTimerSysTickUptimeInfoAdapter* const me);

uint32_t SpinTimerSysTickUptimeInfoAdapter_currentTimeMicros(SpinTimerSysTickUptimeInfoAdapter const* const me);
uint32_t SpinTimerSysTickUptimeInfoAdapter_maxTimeValue(SpinTimerSysTickUptimeInfoAdapter const* const me);
uint32_t SpinTimerSysTickUptimeInfoAdapter_microsPerTick(SpinTimerSysTickUptimeInfoAdapter const* const me);

void SpinTimerSysTickUptimeInfoAdapter_updateTickCount();

#endif
