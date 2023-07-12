#ifndef __SpinTimerAction_H
#define __SpinTimerAction_H

typedef struct SpinTimerAction SpinTimerAction;
struct SpinTimerAction
{
    struct SpinTimerActionVtable const* vptr; /* virtual pointer */
    void (*timeExpired)(SpinTimerAction const* const me); /* pure virtual */
};

struct SpinTimerActionVtable
{
    void (*timeExpired)(SpinTimerAction const* const me);
};

/* SpinTimerAction's operations... */
SpinTimerAction* SpinTimerAction_create();
void SpinTimerAction_init(SpinTimerAction* const me);
void SpinTimerAction_destroy(SpinTimerAction* me);

/* virtual calls (late binding) */
static inline void SpinTimerAction_timeExpired_vcall(SpinTimerAction const* const me)
{
    (*me->vptr->timeExpired)(me);
}

#endif
