#ifndef __Timer_H
#define __Timer_H

#include <stdbool.h>
#include <stdint.h>
#include "SpinTimerAdapter.h"
#include "UptimeInfo.h"
#include "SpinTimerHwHandler.h"

typedef enum SpinTimerMode
{
  SpinTimerMode_oneShot = 0,
  SpinTimerMode_continuous = 1
} SpinTimerMode;
    
typedef struct SpinTimerHwHandler SpinTimerHwHandler;
typedef struct SpinTimer SpinTimer;

SpinTimer* SpinTimer_create(SpinTimerMode mode);
void SpinTimer_destroy(SpinTimer* self);

SpinTimerMode SpinTimer_getMode(SpinTimer* self);
void SpinTimer_start(SpinTimer* self, uint32_t timeMicros);
void SpinTimer_cancel(SpinTimer* self);
bool SpinTimer_isRunning(SpinTimer* self);
bool SpinTimer_isExpired(SpinTimer* self);
void SpinTimer_tick(SpinTimer* self);
void SpinTimer_notifyExpired(SpinTimer* self);
void SpinTimer_assignTimeExpiredCallback(SpinTimer* self, void (*timeExpired)());
void SpinTimer_assignUptimeInfoCallout(SpinTimer* self, uint32_t (*tMillis)());
void SpinTimer_assignHwHandler(SpinTimer* self, SpinTimerHwHandler* hwTimerHandler);

#endif
