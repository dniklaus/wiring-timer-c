/*
 * StubSpinTimersUptimeInfoAdapter.h
 *
 *  Created on: 26.09.2023
 *      Author: dniklaus
 */

#ifndef StubSpinTimersUptimeInfoAdapter_H_
#define StubSpinTimersUptimeInfoAdapter_H_

#include "SpinTimerUptimeInfoAdapter.h"

/**
 * Unit Test Stub Adapter Interface.
 */

typedef struct StubSpinTimersUptimeInfoAdapter StubSpinTimersUptimeInfoAdapter;
typedef struct StubSpinTimersUptimeInfoAdapterAttr 
{
    uint32_t timeMicros;
} StubSpinTimersUptimeInfoAdapterAttr;

struct StubSpinTimersUptimeInfoAdapter
{
    SpinTimerUptimeInfoAdapter super;
    StubSpinTimersUptimeInfoAdapterAttr attr;
    void (*setTimeMicros)(StubSpinTimersUptimeInfoAdapter* me,  uint32_t timeMicros);
    void (*incrementCount)(StubSpinTimersUptimeInfoAdapter* me);
};

/* Function prototypes */
StubSpinTimersUptimeInfoAdapter* StubSpinTimersUptimeInfoAdapter_create();
void StubSpinTimersUptimeInfoAdapter_init(StubSpinTimersUptimeInfoAdapter* const me);
void StubSpinTimersUptimeInfoAdapter_destroy(StubSpinTimersUptimeInfoAdapter* const me);

void StubSpinTimersUptimeInfoAdapter_setTimeMicros(StubSpinTimersUptimeInfoAdapter* me,  uint32_t timeMicros);
void StubSpinTimersUptimeInfoAdapter_incrementCount(StubSpinTimersUptimeInfoAdapter* me);
uint32_t StubSpinTimersUptimeInfoAdapter_currentTimeMicros(StubSpinTimersUptimeInfoAdapter const* const me);
uint32_t StubSpinTimersUptimeInfoAdapter_maxTimeValue(StubSpinTimersUptimeInfoAdapter const* const me);
uint32_t StubSpinTimersUptimeInfoAdapter_microsPerTick(StubSpinTimersUptimeInfoAdapter const* const me);

#endif
