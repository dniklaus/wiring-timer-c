/*
 * SpinTimerContext.h
 *
 *  Created on: 25.09.2013
 *      Author: niklausd
 */

#ifndef SPINTIMERCONTEXT_H_
#define SPINTIMERCONTEXT_H_

typedef struct SpinTimer SpinTimer;
typedef struct SpinTimerContext SpinTimerContext;

typedef struct SpinTimerContextAttr
{
    SpinTimer* timer;       /// Root node of single linked list containing the timers to be kicked.
} SpinTimerContextAttr;

/**
 * Spin Timer Context.
 *
 * Features:
 * - is like a very simple scheduler.
 * - has to be kicked (by calling SpinTimerContext_handleTick()) as often as possible and/or on regular intervals,
 *   i.e. in the bare metal main loop() function:
 *
 *       #include "SpinTimerContext.h"
 *       #include "SpinTimer.h"
 * 
 *       // .. init stuff
 *
 *       while (1)
 *       {
 *           // Kick the timer(s)
 *           SpinTimerContext_instance()->handleTick(SpinTimerContext_instance());
 *
 *           // .. do something
 *       }
 *
 * - holds a single linked list of registered SpinTimer objects,
 *   the SpinTimers automatically attach themselves to this on their creation
 *   and automatically detach themselves on their destruction.
 * - is a Singleton
 */
struct SpinTimerContext
{
    SpinTimerContextAttr attr;
    void (*attach)(SpinTimerContext* me, SpinTimer* timer);
    void (*detach)(SpinTimerContext* me, SpinTimer* timer);
    void (*handleTick)(SpinTimerContext* me);
};

/**
 * Create and/or return singleton instance of SpinTimerContext.
 * @return Pointer to singleton SpinTimerContext object.
 */
SpinTimerContext* SpinTimerContext_instance();

/**
 * Destructor.
 * @param me Pointer to SpinTimerContext object. 
 */
void SpinTimerContext_destroy(SpinTimerContext* me);

/**
 * Add a SpinTimer object to the single linked list.
 * @param me Pointer to SpinTimerContext object. 
 * @param timer SpinTimer object pointer.
 */
void SpinTimerContext_attach(SpinTimerContext* me, SpinTimer* timer);

/**
 * Remove specified SpinTimer object from the single linked list.
 * @param me Pointer to SpinTimerContext object. 
 * @param timer SpinTimer object pointer.
 */
void SpinTimerContext_detach(SpinTimerContext* me, SpinTimer* timer);

/**
 * Kick all attached SpinTimer objects (calls the SpinTimer_tick() method on each list member).
 * @param me Pointer to SpinTimerContext object. 
 */
void SpinTimerContext_handleTick(SpinTimerContext* me);

#endif /* SPINTIMERCONTEX_H_ */
