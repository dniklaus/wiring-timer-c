#ifndef HwTimerHandler_H
#define HwTimerHandler_H

#include <stdlib.h>
#include <stdint.h>
#include "SpinTimer.h"

/**
 * @brief <<Interface>> HwTimerHandler
 * 
 * @details Interface to a HW Timer Handler, used by a Spin Timer.
 */
typedef struct HwTimerHandler HwTimerHandler;
typedef struct SpinTimer SpinTimer;

struct HwTimerHandler
{
    void (*setup)(SpinTimer* spinTimer);
    void (*start)(uint32_t timeMicros);
    void (*stop)();
};

HwTimerHandler* HwTimerHandler_create(
    SpinTimer* spinTimer,
    void (*setup)(SpinTimer* spinTimer),
    void (*start)(uint32_t timeMicros),
    void (*stop)());

void HwTimerHandler_destroy(HwTimerHandler* self);

#endif
