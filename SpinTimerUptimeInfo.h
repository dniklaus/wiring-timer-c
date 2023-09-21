/*
 * SpinTimerUptimeInfo.h
 *
 *  Created on: 01.10.2013
 *      Author: niklausd
 */

#ifndef SpinTimerUptimeInfo_H_
#define SpinTimerUptimeInfo_H_

#include <stdlib.h>
#include "SpinTimerUptimeInfoAdapter.h"

/**
 * Helper class to use the appropriate time base depending on framework.
 * Supported default frameworks:
 * - Arduino (AVR and SAM)
 * - POSIX
 */


typedef struct SpinTimerUptimeInfo SpinTimerUptimeInfo;
typedef struct SpinTimerUptimeInfoAttr
{
  SpinTimerUptimeInfoAdapter* adapter;
} SpinTimerUptimeInfoAttr;

struct SpinTimerUptimeInfo
{
    SpinTimerUptimeInfoAttr attr;
    void (*setAdapter)(SpinTimerUptimeInfo* me, SpinTimerUptimeInfoAdapter* adapter);
    uint32_t (*currentTimeMicros)(SpinTimerUptimeInfo const* const me);
    uint32_t (*maxTimeValue)(SpinTimerUptimeInfo const* const me);
};

SpinTimerUptimeInfo* SpinTimerUptimeInfo_instance();
void SpinTimerUptimeInfo_destroy(SpinTimerUptimeInfo* me);

/**
 * @brief Get current uptime in microseconds.
 * @return uint32_t current uptime [us].
 */
static inline uint32_t SpinTimerUptimeInfo_currentTimeMicros(SpinTimerUptimeInfo const* const me)
{
  uint32_t us = 0;
  if (0 != me->attr.adapter)
  {
    us = me->attr.adapter->currentTimeMicros(me->attr.adapter);
  }
  return us;
}

static inline uint32_t SpinTimerUptimeInfo_maxTimeValue(SpinTimerUptimeInfo const* const me)
{
    uint32_t max = 0;
    if (0 != me->attr.adapter)
    {
        max = me->attr.adapter->maxTimeValue(me->attr.adapter);
    }
    return max;
}

void SpinTimerUptimeInfo_setAdapter(SpinTimerUptimeInfo* me, SpinTimerUptimeInfoAdapter* adapter);

#endif
