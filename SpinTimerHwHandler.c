#include "SpinTimerHwHandler.h"
#include "SpinTimer.h"

typedef struct SpinTimerHwHandlerAttributes
{
    SpinTimer* spinTimer;
} SpinTimerHwHandlerAttributes;

SpinTimerHwHandler* SpinTimerHwHandler_create(SpinTimer* spinTimer)
{
    SpinTimerHwHandler* base = malloc(sizeof(SpinTimerHwHandler));
    SpinTimerHwHandlerAttributes* attr = malloc(sizeof(SpinTimerHwHandlerAttributes));

    base->attr = attr;

    base->attr->spinTimer = spinTimer;

    base->destroy     = &SpinTimerHwHandler_destroy;
    base->start       = 0;
    base->stop        = 0;
    base->intControl  = 0;
    base->spinTimer   = &SpinTimerHwHandler_spinTimer;

    return base;
}

void SpinTimerHwHandler_destroy(SpinTimerHwHandler* self)
{
    free(self);
}

SpinTimer* SpinTimerHwHandler_spinTimer(SpinTimerHwHandler* self)
{
    return self->attr->spinTimer;
}
