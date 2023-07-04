#ifndef HwTimerHandler_H
#define HwTimerHandler_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "SpinTimer.h"

typedef enum SpinTimerHwHandlerIntAction
{
    SpinTimerHwHandlerIntAction_disable,
    SpinTimerHwHandlerIntAction_enable
} SpinTimerHwHandlerIntAction;

typedef struct SpinTimerHwHandler SpinTimerHwHandler;
typedef struct SpinTimer SpinTimer;

/**
 * @brief <<Interface>> SpinTimerHwHandler
 * 
 * @details Interface to a HW Handler used by a SpinTimer.
 */
struct SpinTimerHwHandler
{
    void (*setup)(SpinTimer* spinTimer);
    void (*start)(uint32_t timeMicros);
    void (*stop)();
    void (*intControl)(SpinTimerHwHandlerIntAction intAction);
};

SpinTimerHwHandler* SpinTimerHwHandler_create(
    SpinTimer* spinTimer,
    void (*setup)(SpinTimer* spinTimer),
    void (*start)(uint32_t timeMicros),
    void (*stop)(),
    void (*intControl)(SpinTimerHwHandlerIntAction intAction));

void SpinTimerHwHandler_destroy(SpinTimerHwHandler* self);

#endif
