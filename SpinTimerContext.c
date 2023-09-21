/*
 * SpinTimerContext.c
 *
 *  Created on: 25.09.2013
 *      Author: niklausd
 */

#include <stdlib.h>
#include "SpinTimerContext.h"
#include "SpinTimer.h"

// Private functions
/**
 * Constructor.
 * @return Pointer to SpinTimerContext object.
 */
SpinTimerContext* SpinTimerContext_create();

/**
 * @brief Object Initializer.
 * @param me Pointer to SpinTimerContext object.
 */
void SpinTimerContext_init(SpinTimerContext* me);

// Singleton
static SpinTimerContext* s_spinTimerContext = 0; /// SpinTimerContext singleton instance variable.

SpinTimerContext* SpinTimerContext_instance()
{
    if (0 == s_spinTimerContext)
    {
        s_spinTimerContext = SpinTimerContext_create();
    }
    return s_spinTimerContext;
}

SpinTimerContext* SpinTimerContext_create()
{
    SpinTimerContext* me = malloc(sizeof(SpinTimerContext));
    SpinTimerContext_init(me);
    return me;
}

void SpinTimerContext_init(SpinTimerContext* me)
{
    me->attr.timer = 0;

    me->attach = &SpinTimerContext_attach;
    me->detach = &SpinTimerContext_detach;
    me->handleTick = &SpinTimerContext_handleTick;
}

void SpinTimerContext_destroy(SpinTimerContext* me)
{
    s_spinTimerContext = 0;
    free(me);
}

void SpinTimerContext_attach(SpinTimerContext* me, SpinTimer* timer)
{
    if (0 == me->attr.timer)
    {
        me->attr.timer = timer;
    }
    else
    {
        SpinTimer* next = me->attr.timer;
        while (next->next(next) != 0)
        {
            next = next->next(next);
        }
        next->setNext(next, timer);
    }
}

void SpinTimerContext_detach(SpinTimerContext* me, SpinTimer* timer)
{
    if (me->attr.timer == timer)
    {
        me->attr.timer = timer->next(timer);
    }
    else
    {
        SpinTimer* next = me->attr.timer;
        while ((next != 0) && (next->next(next) != timer))
        {
            next = next->next(next);
        }
        if (next != 0)
        {
            next->setNext(next, timer->next(next));
        }
    }
  }

void SpinTimerContext_handleTick(SpinTimerContext* me)
{
  SpinTimer* timer = me->attr.timer;
  while (timer != 0)
  {
    timer->tick(timer);
    timer = timer->next(timer);
  }
}

