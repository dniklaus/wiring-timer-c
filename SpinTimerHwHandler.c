
#include <stdlib.h>
#include "SpinTimer.h"
#include "SpinTimerHwHandler.h"

/* Prototypes of SpinTimerHwHandler's (base class) virtual functions */
static void SpinTimerHwHandler_start(SpinTimerHwHandler const* const me, uint32_t timeMicros);
static void SpinTimerHwHandler_stop(SpinTimerHwHandler const* const me);
static void SpinTimerHwHandler_intControl(SpinTimerHwHandler const* const me, SpinTimerHwHandlerIntAction intAction);

/* Implementations for infrastructure functions */
SpinTimerHwHandler* SpinTimerHwHandler_create(SpinTimer* spinTimer)
{
    SpinTimerHwHandler* me = malloc(sizeof(SpinTimerHwHandler));
    SpinTimerHwHandler_init(me, spinTimer);

    return me;
}

void SpinTimerHwHandler_init(SpinTimerHwHandler* const me, SpinTimer* spinTimer)
{
    static const struct SpinTimerHwHandlerVtable vtable = {
        &SpinTimerHwHandler_start,
        &SpinTimerHwHandler_stop,
        &SpinTimerHwHandler_intControl
    };
    me->vptr = &vtable;
    me->mySpinTimer = spinTimer;

    me->start       = &SpinTimerHwHandler_start_vcall;
    me->stop        = &SpinTimerHwHandler_stop_vcall;
    me->intControl  = &SpinTimerHwHandler_intControl_vcall;
    me->spinTimer   = &SpinTimerHwHandler_spinTimer;
}

void SpinTimerHwHandler_destroy(SpinTimerHwHandler* me)
{
    free(me);
}

/* Implementation for final function*/
SpinTimer* SpinTimerHwHandler_spinTimer(SpinTimerHwHandler const* const me)
{
    return me->mySpinTimer;
}

/* Dummy implementation of virtual functions */
static void SpinTimerHwHandler_start(SpinTimerHwHandler const* const me, uint32_t timeMicros)
{
    (void)me;
    (void)timeMicros;
}

static void SpinTimerHwHandler_stop(SpinTimerHwHandler const* const me)
{
    (void)me;
}

static void SpinTimerHwHandler_intControl(SpinTimerHwHandler const* const me, SpinTimerHwHandlerIntAction intAction)
{
    (void)me;
    (void)intAction;
}
