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
    void (*destroy)(SpinTimerHwHandler* base);
    void (*start)(uint32_t timeMicros);                           /* pure virtual */
    void (*stop)();                                               /* pure virtual */
    void (*intControl)(SpinTimerHwHandlerIntAction intAction);    /* pure virtual */
    SpinTimer* (*spinTimer)(SpinTimerHwHandler* base);
};

/**
 * @brief 
 * @param spinTimer 
 * @return SpinTimerHwHandler* 
 */
SpinTimerHwHandler* SpinTimerHwHandler_create(SpinTimer* spinTimer);

//-----------------------------------------------------------------------------
// implementation function prototypes
//-----------------------------------------------------------------------------
/**
 * @brief 
 * @param base 
 */
void SpinTimerHwHandler_destroy(SpinTimerHwHandler* base);

/**
 * @brief 
 * @param base 
 * @return SpinTimer* 
 */
SpinTimer* SpinTimerHwHandler_spinTimer(SpinTimerHwHandler* base);


#endif
