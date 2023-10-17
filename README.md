lib.emb.spintimer
=================

Universal Timer with 1 microsecond resolution, running on system tick (or other clock sources).


# Features

* implementation based on OOP principles
* configurable to be either running in continuous mode (timer automatically restarts after each interval) or in one-shot mode (timer stops after timeout period is over)
* timer interval/timeout time configurable
* the source of the uptime info [us] can be overridden by injecting a design specific implementation that counts system ticks or other clock info
* handles uptime counter overflows correctly


# Integration

Here the integration of a SpinTimer is shown with a simple embedded application FW toggling a board's built-in LED (blink):

```C
#include <stdint.h>
#include "SpinTimer.h"
#include "SpinTimerContext.h"
#include "SpinTimerSysTickUptimeInfoAdapter.h"
#include "HeartBeatTimerAction.h"

// timer interval constant definition
const uint32_t heartBeatTimeMicros = 200000;  /// [us]

int main(void)
{
  // SpinTimer Uptime Info (10us tick)
  SpinTimerUptimeInfo* spinTimerUptimeInfo = SpinTimerUptimeInfo_instance();
  spinTimerUptimeInfo->assignAdapter(spinTimerUptimeInfo, (SpinTimerUptimeInfoAdapter*)SpinTimerSysTickUptimeInfoAdapter_create());
      
  // SpinTimer HeartBeat LED
  SpinTimer* heartBeatTimer = SpinTimer_create(SpinTimerMode_continuous);
  heartBeatTimer->assignAction(heartBeatTimer, (SpinTimerAction*)HeartBeatTimerAction_create());
  heartBeatTimer->start(heartBeatTimer, heartBeatTimeMicros);

  while(true)
  {
    SpinTimerContext_instance()->handleTick(SpinTimerContext_instance());
  }

  return 1;
}
```
For SpinTimerSysTickUptimeInfoAdapter implementation example see in [Examples/UptimeInfoAdapter/PSoC5](Examples/UptimeInfoAdapter/PSoC5).

# API

This section describes the SpinTimer library Application Programming Interface.
## SpinTimer
* *__Constructor__*: `SpinTimer* SpinTimer_create(SpinTimerMode mode)`

  Allocates memory for a SpinTimer object on the heap.
  * Parameter `mode`: Operation mode
    * `SpinTimerMode_oneShot` (0): one-shot
    * `SpinTimerMode_continuous` (1): countinuous
  * Returns a pointer to the created `SpinTimer` object

* *__Initializer__*: `void SpinTimer_init(SpinTimer* me, SpinTimerMode mode)`
  
  Initializes the SpinTimer object. To be called on SpinTimer objects created on stack. Called by the constructor too.

  * Parameter `me`: Pointer to this SpinTimer object.

  * Parameter `mode`: Operation mode
    * `SpinTimerMode_oneShot` (0): one-shot, the timer stops after the interval is over.
    * `SpinTimerMode_continuous` (1): countinuous, the timer runs continuously interval after interval.

* *__Destructor__*: `void SpinTimer_destroy(SpinTimer* me)`
  
  Detaches this object from the list of timer objects held by `SpinTimerContext`. Deallocates memory assigned for this object. 
  To be called on SpinTimer objects having been created by the constructor (if not used anymore). 
  Never call this on SpinTimer objects created on the stack!
  
  * Parameter `me`: Pointer to this SpinTimer object.

* *__Finalizer__*: `void SpinTimer_finalize(SpinTimer* me)` 
  
  Detaches this object from the list of timer objects held by `SpinTimerContext`. 
  To be called on SpinTimer objects created on stack (if not used anymore). 
  Called by the destructor too.
  
  * Parameter `me`: Pointer to this SpinTimer object.

* *__Mode get accessor__*: `SpinTimerMode SpinTimer_getMode(SpinTimer* me)`
  
  * Parameter `me`: Pointer to this SpinTimer object.
  * Returns the `SpinTimerMode`
    * `SpinTimerMode_oneShot` (0): one-shot, the timer stops after the interval is over.
    * `SpinTimerMode_continuous` (1): countinuous, the timer runs continuously interval after interval.

* *__Start the timer__*: `void SpinTimer_start(SpinTimer* me, uint32_t timeMicros)`

  Start or restart the timer with a specific time out or interval time. 

  * Parameter `me`: Pointer to this SpinTimer object.
  * Parameter `timeMicros`: Time out or interval time to be set for the timer [us]; 0 will make the timer expire as soon as possible.

* *__Cancel the timer and stop__*: `void SpinTimer_cancel(SpinTimer* me)`

  Cancel the timer and stop. No time expired event will be sent out after the specified time would have been elapsed. Subsequent `SpinTimer_isExpired()` queries will return false.

  * Parameter `me`: Pointer to this SpinTimer object.

* *__Sync mode set accessor__*: `void SpinTimer_setSyncNextStartOnLastExpiry(SpinTimer* me, bool syncNextStartOnLastExpiry)`

   Set the sync mode.

  * Parameter `me`: Pointer to this SpinTimer object.
  * Parameter `syncNextStartOnLastExpiry`: 
    * `true`: the beginning of next interval that gets started will be aligned to the last expiry time; 
    * `false`: the next interval starts independantly at the current time.

* *__Sync mode get accessor__*: `bool SpinTimer_doesNextStartSyncOnLastExpiry(SpinTimer const* const me)`

  Get the sync mode.

  * Parameter `me`: Pointer to this SpinTimer object.
  * Returns
    * `true`: The interval start will be aligned to the last expiry time when the timer gets started.
    * `false`: The interval starts independantly at the current time when the timer gets started.

* *__Run status get accessor__* : `bool SpinTimer_isRunning(SpinTimer* me)`
  
  Indicates whether the timer is currently running.

  * Parameter `me`: Pointer to this SpinTimer object.
  * Returns
    * `true`: the timer is running.
    * `false`: the timer isn't running.

* *__Expiry status get accessor (trap) and poll function__*: `bool SpinTimer_isExpired(SpinTimer* me)`

  Poll function to get the timer expire status, recalculates whether the timer has expired before.

  This method could be used in a pure polling mode, where SpinTimer_tick() has not to get called (by the SpinTimerContext_handleTick() method), but also a mixed operation in combination with calling SpinTimer_tick() periodically is possible.

  Subsequent `SpinTimer_isExpired()` queries will return false after the first one returned true, as long as the time did not expire again in case of a continuous timer.

  * Parameter `me`: Pointer to this SpinTimer object.
  * Returns
    * `true`: the timer has expired since last call of this function.
    * `false`: the timer hasn't expired since last call of this function.


* *__Poll function that kicks the Timer__*: `void SpinTimer_tick(SpinTimer* me)`

  Recalculate whether the timer has expired. 
  
  Intended to be used by `SpinTimerContext_handleTick()` while going through all registered timers.

  * Parameter `me`: Pointer to this SpinTimer object.

* *__Assign specific timer expiry action object__*: `void SpinTimer_assignAction(SpinTimer* me, SpinTimerAction* action)`
  
  Dependency injection, see `SpinTimerAction` interface.

  * Parameter `me`: Pointer to this SpinTimer object.
  * Parameter `action`: Pointer to specific timer expiry action object.

* *__SpinTimer Action get accessor__*: `SpinTimerAction* SpinTimer_action(SpinTimer* me)`
  
  * Parameter `me`: Pointer to this SpinTimer object.
  * Returns `SpinTimerAction*`: 
    * Pointer to specific timer expiry action object.
    * 0 if no action is attached.

* *__Get next SpinTimer object in the list__*: `SpinTimer* SpinTimer_next(SpinTimer* me)`
  
  Get next SpinTimer object pointer out of the linked list containing timers.

  * Parameter `me`: Pointer to this SpinTimer object.
  * Returns:
    * Pointer to the next SpinTimer object in the list.
    * 0 if current object is the trailing list element.

* *__Set next SpinTimer object in the list__*: `void SpinTimer_setNext(SpinTimer* me, SpinTimer* next)`
  
  Set next SpinTimer object of the linked list containing timers.

  * Parameter `me`: Pointer to this SpinTimer object.


## SpinTimerAction

* Interface for a `SpinTimer` expiry action object
* *__time expired action__*: `void SpinTimerAction_timeExpired(SpinTimerAction const * const me)`
  * Parameter `me`: Pointer to this SpinTimerAction object.


## SpinTimerUptimeInfo

* Uptime Info (singleton) object
* Mediation of platform and product specific information about system time to the SpinTimer objects:
  * current time [us]
  * maximum of current time value [us]
  * duration of a system tick [us]
* *__Instance accessor__*: `SpinTimerUptimeInfo* SpinTimerUptimeInfo_instance()`
  
  Allocates memory for the singleton instance of this Uptime Info object (on the heap).
  
  * Return `SpinTimerUptimeInfo*`: pointer to the singleton SpinTimerUptimeInfo object .

* *__Destructor__*: `void SpinTimerUptimeInfo_destroy()`
  
  Deallocates memory assigned for this singleton object.
  
* *__Assign specific SpinTimerUptimeInfoAdapter__*: `void SpinTimerUptimeInfo_assignAdapter(SpinTimerUptimeInfo* me, SpinTimerUptimeInfoAdapter* adapter)`
  
  Dependency injection, see `SpinTimerUptimeInfoAdapter` interface.
  
  __This function needs to be called before creating/initializing any instances of `SpinTimer` objects!__
  
  * Paramter `me`: Pointer to this SpinTimerUptimeInfo object.
  * Parameter `adapter`: Pointer to specific SpinTimerUptimeInfoAdapter object.

* *__SpinTimerUptimeInfoAdapter get accessor__*: `SpinTimerUptimeInfoAdapter* SpinTimerUptimeInfo_adapter(SpinTimerUptimeInfo* me)`
  
  Parameter `me`: Pointer to this SpinTimerUptimeInfo object.
  Return `SpinTimerUptimeInfoAdapter*`: 
  * pointer to specific UptimeInfoAdapter object
  * 0 if no adapter is assigned.


## SpinTimerUptimeInfoAdapter

* SpinTimer Uptime Info Adapter *Interface*
* Application and platform specific interface functions to be implemented:
  * *__Get current time__*: `uint32_t SpinTimerUptimeInfoAdapter_currentTimeMicros(SpinTimerUptimeInfoAdapter const* const me)`
    
    Current time [us] since start or wrap around.

  * *__Maximum of time value__*: `uint32_t SpinTimerUptimeInfoAdapter_maxTimeValue(SpinTimerUptimeInfoAdapter const* const me)`

    Info about the wrap around value.

  * *__Duration of system tick__*: `uint32_t SpinTimerUptimeInfoAdapter_microsPerTick(SpinTimerUptimeInfoAdapter const* const me)`
    
    System tick time, resolution of the system [us].

## SpinTimerContext

Features:
- is a singleton
- is like a very simple scheduler.
- has to be kicked (by calling `SpinTimerContext_handleTick()`) as often as possible and/or on regular intervals,
i.e. in the bare metal main loop() function:

  ```C
  #include "SpinTimerContext.h"
  #include "SpinTimer.h"
    
  // .. init stuff
    
  while (1)
  {
    // Kick the timer(s)
    SpinTimerContext_instance()->handleTick(SpinTimerContext_instance());
      
    // .. do something
  }
  ```
     
- holds a single linked list of registered SpinTimer objects,
  the SpinTimers automatically attach themselves to this on their creation
  and automatically detach themselves on their destruction.

# Notes
This repository is a C language adaptation of https://github.com/dniklaus/spin-timer.
