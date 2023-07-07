#include <stdlib.h>
#include "SpinTimerAction.h"

SpinTimerAction* SpinTimerAction_create()
{
    SpinTimerAction* base = malloc(sizeof(SpinTimerAction));

    base->destroy = &SpinTimerAction_destroy;
    base->timeExpired = 0;

    return base;
}

void SpinTimerAction_destroy(SpinTimerAction* base)
{
    free(base);
}
