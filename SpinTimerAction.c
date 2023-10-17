#include <stdlib.h>
#include "SpinTimerAction.h"

/* pure virtual functions: fwd. decl. */
/**
 * @brief SpinTimer object's expiry action.
 * @details Implementation of this will be called on the SpinTimer object's expiry when injected. Pure virtual function. 
 * @param me 
 */
static void SpinTimerAction_timeExpired(SpinTimerAction const * const me);

SpinTimerAction* SpinTimerAction_create()
{
    SpinTimerAction* me = malloc(sizeof(SpinTimerAction));
    SpinTimerAction_init(me);

    return me;
}

void SpinTimerAction_destroy(SpinTimerAction* me)
{
    free(me);
}

void SpinTimerAction_init(SpinTimerAction* const me)
{
    static const struct SpinTimerActionVtable vtable = {
        &SpinTimerAction_timeExpired
    };
    me->vptr = &vtable;
    me->timeExpired = &SpinTimerAction_timeExpired_vcall;
}

/* pure virtual functions: dummy impl. */
static void SpinTimerAction_timeExpired(SpinTimerAction const* me)
{
    (void)me;   /* unused parameter */
}