#ifndef __MockTimerAction_H
#define __MockTimerAction_H

#include <stdint.h>
#include "SpinTimerAction.h"

typedef struct MockTimerAction MockTimerAction;
typedef struct MockTimerActionAttr
{
    uint32_t calls;
} MockTimerActionAttr;

struct MockTimerAction
{
    SpinTimerAction super; /* inherited MockTimerAction */
    MockTimerActionAttr attr;
    void (*resetNumberOfCalls)(MockTimerAction* me);
    uint32_t (*getNumberOfCalls)(MockTimerAction const* const me);
};

/* MockTimerAction operatios */
MockTimerAction* MockTimerAction_create();
void MockTimerAction_init(MockTimerAction* const me);
void MockTimerAction_destroy(MockTimerAction* me);

void MockTimerAction_timeExpired(MockTimerAction* me);

void MockTimerAction_resetNumberOfCalls(MockTimerAction* me);
uint32_t MockTimerAction_getNumberOfCalls(MockTimerAction const* const me);

#endif
