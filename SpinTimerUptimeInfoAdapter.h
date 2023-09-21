/*
 * SpinTimerUptimeInfoAdapter.h
 *
 *  Created on: 20.09.2023
 *      Author: dniklaus
 */

#ifndef SpinTimerUptimeInfoAdapter_H_
#define SpinTimerUptimeInfoAdapter_H_

#include <stdint.h>

/**
 * Adapter Interface, will call-out the platform specific up-time info time in microseconds.
 */

typedef struct SpinTimerUptimeInfoAdapter SpinTimerUptimeInfoAdapter;
typedef struct SpinTimerUptimeInfoAdapterVtable SpinTimerUptimeInfoAdapterVtable;               

struct SpinTimerUptimeInfoAdapter
{
    SpinTimerUptimeInfoAdapterVtable const* vptr;   /* SpinTimerUptimeInfoAdapter's pointer to immutable virtual function table */
    uint32_t (*currentTimeMicros)(SpinTimerUptimeInfoAdapter const* const me);  /* interface function currentTimeMicros() (pure virtual) */
    uint32_t (*maxTimeValue)(SpinTimerUptimeInfoAdapter const* const me);       /* interface function maxTimeValue() (pure virtual) */
};

/* SpinTimerUptimeInfoAdapter's virtual function table */
struct SpinTimerUptimeInfoAdapterVtable
{
    uint32_t (*currentTimeMicros)(SpinTimerUptimeInfoAdapter const* const me);  /* pointer to virtual function currentTimeMicros() */
    uint32_t (*maxTimeValue)(SpinTimerUptimeInfoAdapter const* const me);       /* pointer to virtual function maxTimeValue() */
};

/* Function prototypes */
void SpinTimerUptimeInfoAdapter_init(SpinTimerUptimeInfoAdapter* const me);

/* inline virtual calls */
static inline uint32_t SpinTimerUptimeInfoAdapter_currentTimeMicros_vcall(SpinTimerUptimeInfoAdapter const* const me)
{
    return (*me->vptr->currentTimeMicros)(me); /* late binding: calls the function in the current virtual table */
}

static inline uint32_t SpinTimerUptimeInfoAdapter_maxTimeValue_vcall(SpinTimerUptimeInfoAdapter const* const me)
{
    return (*me->vptr->maxTimeValue)(me); /* late binding: calls the function in the current virtual table */
}

#endif
