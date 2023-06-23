#ifndef HwTimerHandler_H
#define HwTimerHandler_H

#include <stdlib.h>
#include <stdint.h>

/**
 * @brief <<Interface>> HwTimerHandler
 * 
 * @details Interface to a HW Timer Handler, used by a Spin Timer.
 */
typedef struct HwTimerHandler_t* HwTimerHandler;

typedef enum HwTimerMode_t
{
  HwTimerMode_oneShot = 0,
  HwTimerMode_continuous = 1
} HwTimerMode;

struct HwTimerHandler_t
{
  void (*setMode)(HwTimerMode mode);
  void (*setIntervallMicros)(uint32_t timeMicros);
  void (*start)();
  void (*stop)();
};

HwTimerHandler HwTimerHandler_create(
  void (*setMode)(HwTimerMode mode),
  void (*setIntervallMicros)(uint32_t timeMicros),
  void (*start)(),
  void (*stop)());

void HwTimerHandler_destroy(HwTimerHandler self);

#endif
