#ifndef HwTimerHandler_H
#define HwTimerHandler_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "SpinTimer.h"

typedef enum SpinTimerHwHandlerIntAction
{
    SpinTimerHwHandlerIntAction_disable,
    SpinTimerHwHandlerIntAction_enable
} SpinTimerHwHandlerIntAction;

typedef struct SpinTimer SpinTimer;
typedef struct SpinTimerHwHandler SpinTimerHwHandler;

struct SpinTimerHwHandler 
{
    struct SpinTimerHwHandlerAttributes* attr;
    void (*destroy)(SpinTimerHwHandler* self);
    void (*start)(uint32_t timeMicros);                           /* pure virtual */
    void (*stop)();                                               /* pure virtual */
    void (*intControl)(SpinTimerHwHandlerIntAction intAction);    /* pure virtual */
    SpinTimer* (*spinTimer)(SpinTimerHwHandler* self);
};

/**
 * @brief 
 * 
 * @param self 
 * @param spinTimer 
 * @return SpinTimerHwHandler* 
 */
SpinTimerHwHandler* SpinTimerHwHandler_create(SpinTimer* spinTimer);

//-----------------------------------------------------------------------------
// implementation function prototypes
//-----------------------------------------------------------------------------
/**
 * @brief 
 * 
 * @param self 
 */
void SpinTimerHwHandler_destroy(SpinTimerHwHandler* self);

/**
 * @brief 
 * 
 * @param self 
 * @return SpinTimer* 
 */
SpinTimer* SpinTimerHwHandler_spinTimer(SpinTimerHwHandler* self);


#endif
