#include "SpinTimerHwHandler.h"
#include "SpinTimer.h"

struct SpinTimer;

SpinTimerHwHandler* SpinTimerHwHandler_create(
    SpinTimer* spinTimer,
    void (*setup)(SpinTimer* spinTimer),
    void (*start)(uint32_t timeMicros),
    void (*stop)(),
    void (*intControl)(SpinTimerHwHandlerIntAction intAction))
{
    SpinTimerHwHandler* instance = malloc(sizeof(SpinTimerHwHandler));

    instance->setup      = setup;
    instance->start      = start;
    instance->stop       = stop;
    instance->intControl = intControl;

    if (0 != setup)
    {
        instance->setup(spinTimer);
    }
    return instance;
}

void SpinTimerHwHandler_destroy(SpinTimerHwHandler* self)
{
    if (0 != self)
    {
        self->intControl(false);
    }
    free(self);
}
