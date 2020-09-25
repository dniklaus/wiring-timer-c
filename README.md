wiring-timer-c
===================

Universal Timer with 1 millisecond resolution, originally based on Arduino millis() function, C implementation (single instance only).


# Features

* configurable to be either recurring (timer automatically restarts after the interval) or non-recurring (timer stops after timeout period is over)
* timer interval/timeout time configurable
* originally based on Arduino millis() function (number of milliseconds since the Arduino board began running the current program), the source of the uptime info [ms] can be overridden by injecting a platform specific implementation when working with other frameworks than with Arduino
* handles unsigned long int overflows correctly


# Integration

Here the integration of a Timer is shown with a simple Arduino Sketch toggling the Arduino board's built-in LED (blink):

* include the library

  ```C
  #include <SpinTimer.h>
  ```

* timer interval constant definition

  ```C
  const unsigned int BLINK_TIME_MILLIS = 200;
  ```

* specific `SpinTimerAdapter` implementation, periodically toggling the Arduino built-in LED

  ```C
  void BlinkTimerAdapter_timeExpired()
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  ```

* setup: set LED pin to output; create recurring Timer, inject specific TimerAdapter

  ```C
  // The setup function is called once at startup of the sketch
  void setup()
  {
    pinMode(LED_BUILTIN, OUTPUT);

    SpinTimer_create(SpinTimer_IS_RECURRING);
    SpinTimer_assignTimeExpiredCallback(&BlinkTimerAdapter_timeExpired);
    SpinTimer_start(BLINK_TIME_MILLIS);
  }
  ```

* loop: call `SpinTimer_tick()` function

  ```C
  // The loop function is called in an endless loop
  void loop()
  {
    SpinTimer_tick();
  }
  ```

## Platform specific Uptime Info Adapter injection

When using the Timer library with an Arduino Framework environment the uptime milliseconds counter info default implementation is automatically engaged and nothing has to be done.

If you are using other environments (i.e. when running in an STM32Cube system), a specific `UptimeInfoAdapter` implementation has to be used and injected. The following example shows the specific `STM32UptimeInfoAdapter`implementation of `UptimeInfoAdapter`as to be found in the Examples folder:

```C
/*
 * STM32UptimeInfoAdapter.h
 */

#ifndef STM32UPTIMEINFOADAPTER_H_
#define STM32UPTIMEINFOADAPTER_H_

#include "stm32l4xx_hal.h"
#include "UptimeInfo.h"

unsigned long STM32UptimeInfoAdapter_tMillis()
{
  unsigned long ms = HAL_GetTick();
  return ms;
}

#endif /* STM32UPTIMEINFOADAPTER_H_ */

```

Here is how the `STM32UptimeInfoAdapter` implementation is injected into the `SpinTimer` instance:

```C
// ..
#include "STM32UptimeInfoAdapter.h"

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  
  // ..

  SpinTimer_assignUptimeInfoCallout(&STM32UptimeInfoAdapter_tMillis);

  // ..
    
  while (1)
  {
    SpinTimer_tick();
  }    
}
```

# API

This section describes the Timer library Application Programming Interface.
## SpinTimer
* *Constructor*: `SpinTimer_create(bool isRecurring)`
  * Parameter `isRecurring`: Operation mode, true: recurring, false: non-recurring
* Assign specific `timeExpired` callback function, acts as dependency injection. `void SpinTimer_assignTimeExpiredCallback(void (*timeExpired)())`
  * Parameter `void (*timeExpired)()`: Function Pointer to specific `timeExpired` function implementation
* Assign specific `tMillis` callout function, acts as dependency injection. `void SpinTimer_assignUptimeInfoCallout(unsigned long (*tMillis)())`
  * Parameter `unsigned long (*tMillis)()`: Function Pointer to specific `tMillis` function implementation

* *Start or restart the timer* with a specific time out or interval time. `void SpinTimer_start(unsigned long timeMillis)`
   * Parameter `timeMillis`: Time out or interval time to be set for the timer [ms]; 0 will make the timer expire as soon as possible.
* *Cancel the timer and stop*. `void SpinTimer_cancel()`
  * No time expired event will be sent out after the specified time would have been elapsed.
  * Subsequent `SpinTimer_isExpired()` queries will return false.

* Poll method to *get the timer expired status*, recalculates whether the timer has expired before. `bool SpinTimer_isExpired()`
  * This method could be used in a pure polling mode, where `SpinTimer_tick()` has not to get called by the `SpinTimer_tick()` function, but also a mixed operation in combination with calling `SpinTimer_tick()` periodically is possible.
  * Subsequent `isTimerExpired()` queries will return false after the first one returned true, as long as the time did not expire again in case of a recurring timer.
  * Returns `true` if the timer has expired, `false` otherwise.
  
* Indicates whether the timer is currently *running*. `bool SpinTimer_isRunning()`
  * Returns `true` if timer is running, `false` otherwise.

* Kick the Timer. `void SpinTimer_tick()`
   * Recalculates whether the timer has expired.

* Constant for `isRecurring` parameter of the constructor to create a one shot timer.
  `static const bool SpinTimer_IS_NON_RECURRING = false`

* Constant for `isRecurring` parameter of the constructor to create a recurring timer.
  `static const bool SpinTimer_IS_RECURRING = true`


## SpinTimerAdapter
* Adapter Interface, contains `void (*timeExpired)()` function pointer prototype declaration.
* *Time expired event*. To be implemented by specific call back function with the same signature as `void (*timeExpired)()`.

## UptimeInfo
* Uptime Info Adapter Interface, contains `unsigned long (*tMillis)()` function pointer prototype declaration.
* Call out to get current milliseconds. To be implemented by specific call back function with the same signature as `unsigned long (*tMillis)()`.


# Notes
This repository is a C language adaptation of https://github.com/dniklaus/wiring-timer.
