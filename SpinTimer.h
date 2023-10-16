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
    bool syncNextStartOnLastExpiry;
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
    void (*setSyncNextStartOnLastExpiry)(SpinTimer* me, bool syncNextStartOnLastExpiry);
    bool (*isRunning)(SpinTimer* me);
    bool (*isExpired)(SpinTimer* me);
    bool (*doesNextStartSyncOnLastExpiry)(SpinTimer const* const me);
    void (*tick)(SpinTimer* me);
    void (*assignAction)(SpinTimer* me, SpinTimerAction* action);
    SpinTimerAction* (*action)(SpinTimer* me);
    SpinTimer* (*next)(SpinTimer* me);
    void (*setNext)(SpinTimer* me, SpinTimer* next);
};

/**
 * @brief Constructor.
 * @details Allocates memory for a SpinTimer object on the heap and attaches itself to the list of timer objects held by SpinTimerContext.
 * @param mode Operation mode {SpinTimerMode_oneShot|SpinTimerMode_continuous}
 * @return SpinTimer* Pointer to the created SpinTimer object.
 */
SpinTimer* SpinTimer_create(SpinTimerMode mode);

/**
 * @brief Initializer.
 * @details Initializes the SpinTimer object and attaches itself to the list of timer objects held by SpinTimerContext. To be called on SpinTimer objects created on stack. Called by the constructor too.
 * @param me Pointer to this SpinTimer object.
 * @param mode Operation mode {SpinTimerMode_oneShot|SpinTimerMode_continuous}
 */
void SpinTimer_init(SpinTimer* me, SpinTimerMode mode);

/**
 * @brief Destructor.
 * @details Detaches this object from the list of timer objects held by SpinTimerContext. Deallocates memory assigned for this object. To be called on SpinTimer objects having been created by the constructor (if not used anymore). Never call this on SpinTimer objects created on the stack.
 * @param me Pointer to this SpinTimer object.
 */
void SpinTimer_destroy(SpinTimer* me);

/**
 * @brief Finalizer.
 * @details Detaches this object from the list of timer objects held by SpinTimerContext. To be called on SpinTimer objects created on stack (if not used anymore). Called by the destructor too.
 * @param me Pointer to this SpinTimer object.
 */
void SpinTimer_finalize(SpinTimer* me);

/**
 * @brief Mode get accessor.
 * @details Returns the mode of the SpinTimer object.
 * @param me Pointer to this SpinTimer object.
 * @return SpinTimerMode {SpinTimerMode_oneShot|SpinTimerMode_continuous}
 */
SpinTimerMode SpinTimer_getMode(SpinTimer* me);

/**
 * @brief Start the timer.
 * @details Start or restart the timer with a specific time out or interval time. The timer will expire after the specified time.
 * @param me Pointer to this SpinTimer object.
 * @param timeMicros Time out or interval time to be set for the timer [us]; 0 will make the timer expire as soon as possible.
 */
void SpinTimer_start(SpinTimer* me, uint32_t timeMicros);

/**
 * @brief Cancel the timer.
 * @details Cancel the timer and stop. No time expired event will be sent out after the specified time would have been elapsed. Subsequent isExpired() queries will return false.
 * @param me Pointer to this SpinTimer object.
 */
void SpinTimer_cancel(SpinTimer* me);

/**
 * @brief Sync mode set accessor.
 * @details Set the sync mode.
 * @param me Pointer to this SpinTimer object.
 * @param syncNextStartOnLastExpiry true: the beginning of next interval that gets started will be aligned to the last expiry time; false: the next interval starts independantly at the current time.
 */
void SpinTimer_setSyncNextStartOnLastExpiry(SpinTimer* me, bool syncNextStartOnLastExpiry);

/**
 * @brief Sync mode get accessor.
 * @details Get the sync mode.
 * @param me Pointer to this SpinTimer object.
 * @return true The interval start will be aligned to the last expiry time when the timer gets started.
 * @return false The interval starts independantly at the current time when the timer gets started.
 */
bool SpinTimer_doesNextStartSyncOnLastExpiry(SpinTimer const* const me);

/**
 * @brief Run status get accessor.
 * @details Indicates whether the timer is currently running.
 * @param me Pointer to this SpinTimer object.
 * @return true The timer is running.
 * @return false The timer isn't running.
 */
bool SpinTimer_isRunning(SpinTimer* me);

/**
 * @brief Expiry status get accessor (trap) and poll function.
 * @details Poll function to get the timer expire status, recalculates whether the timer has expired before.
 * This method could be used in a pure polling mode, where SpinTimer_tick() has not to get called
 * (by the SpinTimerContext_handleTick() method), but also a mixed operation in combination with
 * calling SpinTimer_tick() periodically is possible.
 * Subsequent SpinTimer_isExpired() queries will return false after the first one returned true,
 * as long as the time did not expire again in case of a continuous timer.
 * @param me Pointer to this SpinTimer object.
 * @return true The timer has expired since last call of this function.
 * @return false The timer hasn't expired since last call of this function.
 */
bool SpinTimer_isExpired(SpinTimer* me);

/**
 * @brief Poll function that kicks the Timer.
 * @details Recalculate whether the timer has expired. Intended to be used by SpinTimerContext_handleTick() while going through all registered timers.
 * @param me Pointer to this SpinTimer object.
 */
void SpinTimer_tick(SpinTimer* me);

/**
 * @brief Assign specific timer expiry action object.
 * @details Dependency injection. @see SpinTimerAction interface.
 * @param me Pointer to this SpinTimer object.
 * @param action Pointer to specific timer expiry action object.
 */
void SpinTimer_assignAction(SpinTimer* me, SpinTimerAction* action);

/**
 * @brief SpinTimer Action get accessor.
 * @param me Pointer to this SpinTimer object.
 * @return SpinTimerAction* SpinTimerAction object pointer or 0 if no action is attached.
 */
SpinTimerAction* SpinTimer_action(SpinTimer* me);

/**
 * @brief Get next SpinTimer object pointer out of the linked list containing timers.
 * @param me Pointer to this SpinTimer object.
 * @return SpinTimer object pointer or 0 if current object is the trailing list element.
 */
SpinTimer* SpinTimer_next(SpinTimer* me);

/**
 * @brief Set next SpinTimer object of the linked list containing timers.
 * @param me Pointer to this SpinTimer object.
 * @param next SpinTimer object pointer to be set as the next element of the list.
 */
void SpinTimer_setNext(SpinTimer* me, SpinTimer* next);

#endif
