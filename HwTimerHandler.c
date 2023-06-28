#include "HwTimerHandler.h"
#include "externals/lib/spintimer/SpinTimer.h"

HwTimerHandler HwTimerHandler_create(
    SpinTimer spinTimer,
    void (*setup)(SpinTimer spinTimer),
    void (*setMode)(HwTimerMode mode),
    void (*setIntervallMicros)(uint32_t timeMicros),
    void (*start)(),
    void (*stop)())
{
    HwTimerHandler instance = (HwTimerHandler) calloc(1, sizeof(struct HwTimerHandler_t));

    instance->setup                 = setup;
    instance->setMode               = setMode;
    instance->setIntervallMicros    = setIntervallMicros;
    instance->start                 = start;
    instance->stop                  = stop;

    if (0 != setup)
    {
        instance->setup(spinTimer);
    }
    return instance;
}

void HwTimerHandler_destroy(HwTimerHandler self)
{
  free(self);
}
