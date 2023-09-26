#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "MockTimerAction.h"

MockTimerAction* MockTimerAction_create()
{
    MockTimerAction* me = malloc(sizeof(MockTimerAction));
    MockTimerAction_init(me);
    return me;
}

void MockTimerAction_init(MockTimerAction* const me)
{
    static const struct SpinTimerActionVtable vtable = {
        (void (*)(SpinTimerAction const* const me))&MockTimerAction_timeExpired
    };
    SpinTimerAction_init(&me->super);
    me->super.vptr = &vtable;    
    me->attr.calls = 0;
    me->resetNumberOfCalls = &MockTimerAction_resetNumberOfCalls;
    me->getNumberOfCalls = &MockTimerAction_getNumberOfCalls;
}

void MockTimerAction_destroy(MockTimerAction* me)
{
    free(me);
}

void MockTimerAction_timeExpired(MockTimerAction* me)
{
    me->attr.calls += 1;
    printf("NOTE: MockTimerAdapter_timeExpired()!\n");

}

void MockTimerAction_resetNumberOfCalls(MockTimerAction* me)
{
    me->attr.calls = 0;
}

uint32_t MockTimerAction_getNumberOfCalls(MockTimerAction const* const me)
{
    return me->attr.calls;
}
