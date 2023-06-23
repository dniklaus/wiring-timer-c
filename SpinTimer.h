#ifndef __Timer_H
#define __Timer_H

#include <stdbool.h>
#include "SpinTimerAdapter.h"
#include "UptimeInfo.h"
#include "HwTimerHandler.h"

extern const bool SpinTimer_IS_NON_RECURRING;
extern const bool SpinTimer_IS_RECURRING;

typedef struct SpinTimer_t* SpinTimer;
typedef struct HwTimerHandler_t* HwTimerHandler;

void SpinTimer_create(SpinTimer self, bool isRecurring);
void SpinTimer_destroy(SpinTimer self);

void SpinTimer_start(SpinTimer self, unsigned long timeMillis);
void SpinTimer_cancel(SpinTimer self);
bool SpinTimer_isRunning(SpinTimer self);
bool SpinTimer_isExpired(SpinTimer self);
void SpinTimer_tick(SpinTimer self);
void SpinTimer_assignTimeExpiredCallback(SpinTimer self, void (*timeExpired)());
void SpinTimer_assignUptimeInfoCallout(SpinTimer self, unsigned long (*tMillis)());
void SpinTimer_assignHwTimerHandler(SpinTimer self, HwTimerHandler hwTimerHandler);

#endif
