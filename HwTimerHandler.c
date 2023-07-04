#include "HwTimerHandler.h"
#include "SpinTimer.h"

struct SpinTimer;

HwTimerHandler* HwTimerHandler_create(
    SpinTimer* spinTimer,
    void (*setup)(SpinTimer* spinTimer),
    void (*start)(uint32_t timeMicros),
    void (*stop)())
{
    HwTimerHandler* instance = malloc(sizeof(HwTimerHandler));

    instance->setup                 = setup;
    instance->start                 = start;
    instance->stop                  = stop;

    if (0 != setup)
    {
        instance->setup(spinTimer);
    }
    return instance;
}

void HwTimerHandler_destroy(HwTimerHandler* self)
{
  free(self);
}
