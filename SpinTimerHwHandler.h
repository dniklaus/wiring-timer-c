#ifndef HwTimerHandler_H
#define HwTimerHandler_H

#include <stdlib.h>
#include <stdint.h>
#include "SpinTimer.h"

/**
 * @brief <<Interface>> SpinTimerHwHandler
 * 
 * @details Interface to a HW Handler used by a SpinTimer.
 */
typedef struct SpinTimerHwHandler SpinTimerHwHandler;
typedef struct SpinTimer SpinTimer;

struct SpinTimerHwHandler
{
    void (*setup)(SpinTimer* spinTimer);
    void (*start)(uint32_t timeMicros);
    void (*stop)();
};

SpinTimerHwHandler* SpinTimerHwHandler_create(
    SpinTimer* spinTimer,
    void (*setup)(SpinTimer* spinTimer),
    void (*start)(uint32_t timeMicros),
    void (*stop)());

void SpinTimerHwHandler_destroy(SpinTimerHwHandler* self);

#endif
