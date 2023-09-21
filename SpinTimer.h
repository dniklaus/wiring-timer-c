#ifndef __Timer_H
#define __Timer_H

#include <stdbool.h>
#include <stdint.h>

typedef enum SpinTimerMode
{
    SpinTimerMode_oneShot = 0,
    SpinTimerMode_continuous = 1
} SpinTimerMode;
    
typedef struct SpinTimerHwHandler SpinTimerHwHandler;
typedef struct SpinTimerAction SpinTimerAction;
typedef struct SpinTimer SpinTimer;

typedef struct SpinTimerAttributes
{
    SpinTimerMode mode;
    bool isRunning;
    bool isExpiredFlag;
    bool willOverflow;
    uint32_t delayMicros;
    uint32_t currentTimeMicros;
    uint32_t triggerTimeMicros;
    uint32_t triggerTimeMicrosUpperLimit;
    uint32_t maxUptimeValue;
    SpinTimerHwHandler* hwHandler;
    SpinTimerAction* action;
} SpinTimerHwHandlerAttributes;

struct SpinTimer
{
    struct SpinTimerAttributes attr;
    SpinTimerMode (*getMode)(SpinTimer* me);
    void (*start)(SpinTimer* me, uint32_t timeMicros);
    void (*cancel)(SpinTimer* me);
    bool (*isRunning)(SpinTimer* me);
    bool (*isExpired)(SpinTimer* me);
    void (*tick)(SpinTimer* me);
    void (*notifyExpired)(SpinTimer* me);
    void (*assignAction)(SpinTimer* me, SpinTimerAction* action);
    SpinTimerAction* (*action)(SpinTimer* me);
    void (*assignHwHandler)(SpinTimer* me, SpinTimerHwHandler* hwTimerHandler);
};

SpinTimer* SpinTimer_create(SpinTimerMode mode);
void SpinTimer_init(SpinTimer* me, SpinTimerMode mode);
void SpinTimer_destroy(SpinTimer* me);

SpinTimerMode SpinTimer_getMode(SpinTimer* me);
void SpinTimer_start(SpinTimer* me, uint32_t timeMicros);
void SpinTimer_cancel(SpinTimer* me);
bool SpinTimer_isRunning(SpinTimer* me);
bool SpinTimer_isExpired(SpinTimer* me);
void SpinTimer_tick(SpinTimer* me);
void SpinTimer_notifyExpired(SpinTimer* me);
void SpinTimer_assignAction(SpinTimer* me, SpinTimerAction* action);
SpinTimerAction* SpinTimer_action(SpinTimer* me);
void SpinTimer_assignHwHandler(SpinTimer* me, SpinTimerHwHandler* hwTimerHandler);

#endif
