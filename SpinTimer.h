#ifndef __Timer_H
#define __Timer_H

#include <stdbool.h>
#include "SpinTimerAdapter.h"
#include "UptimeInfo.h"

extern const bool SpinTimer_IS_NON_RECURRING;
extern const bool SpinTimer_IS_RECURRING;

void SpinTimer_create(bool isRecurring);
void SpinTimer_destroy();

void SpinTimer_start(unsigned long timeMillis);
void SpinTimer_cancel();
bool SpinTimer_isRunning();
bool SpinTimer_isExpired();
void SpinTimer_tick();
void SpinTimer_assignTimeExpiredCallback(void (*timeExpired)());
void SpinTimer_assignUptimeInfoCallout(unsigned long (*tMillis)());

#endif
