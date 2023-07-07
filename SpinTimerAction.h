#ifndef __SpinTimerAction_H
#define __SpinTimerAction_H

typedef struct SpinTimerAction SpinTimerAction;

struct SpinTimerAction
{
    void (*destroy)(SpinTimerAction* base);
    void (*timeExpired)(SpinTimerAction* self); /* pure virtual */
};

SpinTimerAction* SpinTimerAction_create();
void SpinTimerAction_destroy(SpinTimerAction* base);

#endif
