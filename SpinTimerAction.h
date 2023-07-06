#ifndef __SpinTimerAction_H
#define __SpinTimerAction_H

typedef struct SpinTimerAction SpinTimerAction;

struct SpinTimerAction
{
    void (*timeExpired)(SpinTimerAction* self); /* pure virtual */
};

void SpinTimerAction_timeExpired(SpinTimerAction* self);

#endif
