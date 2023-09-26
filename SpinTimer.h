#ifndef __Timer_H
#define __Timer_H

#include <stdbool.h>
#include <stdint.h>

typedef enum SpinTimerMode
{
    SpinTimerMode_oneShot = 0,
    SpinTimerMode_continuous = 1
} SpinTimerMode;
    
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
    uint32_t maxUptimeValue;
    uint32_t microsPerTick;
    SpinTimerAction* action;
    SpinTimer* next;
} SpinTimerAttributes;

struct SpinTimer
{
    SpinTimerAttributes attr;
    SpinTimerMode (*getMode)(SpinTimer* me);
    void (*start)(SpinTimer* me, uint32_t timeMicros);
    void (*cancel)(SpinTimer* me);
    bool (*isRunning)(SpinTimer* me);
    bool (*isExpired)(SpinTimer* me);
    void (*tick)(SpinTimer* me);
    void (*notifyExpired)(SpinTimer* me);
    void (*assignAction)(SpinTimer* me, SpinTimerAction* action);
    SpinTimerAction* (*action)(SpinTimer* me);
    SpinTimer* (*next)(SpinTimer* me);
    void (*setNext)(SpinTimer* me, SpinTimer* next);
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

/**
 * Get next SpinTimer object pointer out of the linked list containing timers.
 * @return SpinTimer object pointer or 0 if current object is the trailing list element.
 */
SpinTimer* SpinTimer_next(SpinTimer* me);

/**
 * Set next SpinTimer object of the linked list containing timers.
 * @param timer SpinTimer object pointer to be set as the next element of the list.
 */
void SpinTimer_setNext(SpinTimer* me, SpinTimer* next);

#endif
