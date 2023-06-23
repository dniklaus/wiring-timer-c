#ifndef HwTimerHandler_H
#define HwTimerHandler_H

#include <stdlib.h>

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
  void (*setMode)(HwTimerHandler self, HwTimerMode mode);
  void (*setIntervallMicros)(HwTimerHandler self);
  void (*start)(HwTimerHandler self);
  void (*stop)(HwTimerHandler self);
};

HwTimerHandler HwTimerHandler_create(
  void (*setMode)(HwTimerHandler self, HwTimerMode mode),
  void (*setIntervallMicros)(HwTimerHandler self),
  void (*start)(HwTimerHandler self),
  void (*stop)(HwTimerHandler self))
{
  HwTimerHandler instance = (HwTimerHandler) calloc(1, sizeof(HwTimerHandler_t));

  instance->setMode             = setMode;
  instance->setIntervallMicros  = setIntervallMicros;
  instance->start               = start;
  instance->stop                = stop;
}

void HwTimerHandler_destroy(HwTimerHandler self)
{
  free(self);
}

#endif