#ifndef HwTimerHandler_H
#define HwTimerHandler_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum SpinTimerHwHandlerIntAction
{
    SpinTimerHwHandlerIntAction_disable,
    SpinTimerHwHandlerIntAction_enable
} SpinTimerHwHandlerIntAction;

typedef struct SpinTimer SpinTimer;

typedef struct SpinTimerHwHandler SpinTimerHwHandler;
struct SpinTimerHwHandler
{
    struct SpinTimerHwHandlerVtable const* vptr; /* virtual pointer */

    SpinTimer* mySpinTimer;

    /* infrastructure functions*/
    void (*init)(SpinTimerHwHandler* const me);
    void (*destroy)(SpinTimerHwHandler* me);

    /* final function */
    SpinTimer* (*spinTimer)(SpinTimerHwHandler const* const me);

    /* pur virtual functions */
    void (*start)(SpinTimerHwHandler const* const me, uint32_t timeMicros);
    void (*stop)(SpinTimerHwHandler const* const me);
    void (*intControl)(SpinTimerHwHandler const* const me, SpinTimerHwHandlerIntAction intAction);
};

struct SpinTimerHwHandlerVtable
{
    void (*start)(SpinTimerHwHandler const* const me, uint32_t timeMicros);
    void (*stop)(SpinTimerHwHandler const* const me);
    void (*intControl)(SpinTimerHwHandler const* const me, SpinTimerHwHandlerIntAction intAction);
};

//-----------------------------------------------------------------------------
// implementation function prototypes
//-----------------------------------------------------------------------------
/**
 * @brief 
 * @param spinTimer 
 * @return SpinTimerHwHandler* 
 */
SpinTimerHwHandler* SpinTimerHwHandler_create(SpinTimer* spinTimer);

/**
 * @brief 
 * @param me 
 */
void SpinTimerHwHandler_init(SpinTimerHwHandler* const me, SpinTimer* spinTimer);

/**
 * @brief 
 * @param me 
 */
 void SpinTimerHwHandler_destroy(SpinTimerHwHandler* me);

/**
 * @brief 
 * @param me
 * @return SpinTimer* 
 */
SpinTimer* SpinTimerHwHandler_spinTimer(SpinTimerHwHandler const* const me);

//-----------------------------------------------------------------------------
// inline virtual calls (late binding)
//-----------------------------------------------------------------------------
static inline void SpinTimerHwHandler_start_vcall(SpinTimerHwHandler const* const me, uint32_t timeMicros)
{
    me->vptr->start(me, timeMicros);
}

static inline void SpinTimerHwHandler_stop_vcall(SpinTimerHwHandler const* const me)
{
    me->vptr->stop(me);
}

static inline void SpinTimerHwHandler_intControl_vcall(SpinTimerHwHandler const* const me, SpinTimerHwHandlerIntAction intAction)
{
    me->vptr->intControl(me, intAction);
}

#endif
