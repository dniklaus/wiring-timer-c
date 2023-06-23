#include <limits.h>
#include <stdlib.h>
#include "SpinTimer.h"

const bool SpinTimer_IS_NON_RECURRING = false;
const bool SpinTimer_IS_RECURRING     = true;

struct SpinTimer_t
{
  bool m_isRunning;
  bool m_isRecurring;
  bool m_isExpiredFlag;
  bool m_willOverflow;
  uint32_t m_delayMicros;
  uint32_t m_currentTimeMicros;
  uint32_t m_triggerTimeMicros;
  uint32_t m_triggerTimeMicrosUpperLimit;
  void (*m_funcTimeExpired)();
  uint32_t (*m_funcTMicros)();
  HwTimerHandler m_hwTimerHandler;
};

/**
 * @brief Evaluate time expired condition (private local function)
 */
void internalTick(SpinTimer self);

/**
 * @brief Start the (new) interval (private local function)
 */
void startInterval(SpinTimer self);

SpinTimer SpinTimer_create(bool isRecurring)
{
  SpinTimer self = (SpinTimer)calloc(1, sizeof(struct SpinTimer_t));

  self->m_isRecurring = isRecurring;
  self->m_isRunning = false;
  self->m_isExpiredFlag = false;
  self->m_willOverflow = false;
  self->m_delayMicros = 0;
  self->m_currentTimeMicros = 0;
  self->m_triggerTimeMicros = 0;
  self->m_triggerTimeMicrosUpperLimit = 0;
  self->m_funcTimeExpired = 0;
  self->m_funcTMicros = 0;
  self->m_hwTimerHandler = 0;

  return self;
}

void SpinTimer_destroy(SpinTimer self)
{
  self->m_funcTimeExpired = 0;
  self->m_funcTMicros = 0;
  self->m_isRecurring = SpinTimer_IS_NON_RECURRING;
  self->m_isRunning = false;
  self->m_isExpiredFlag = false;

  free(self);
}

void SpinTimer_start(SpinTimer self, uint32_t timeMicros)
{
  self->m_isRunning = true;
  self->m_delayMicros = timeMicros;

  if (0 != self->m_hwTimerHandler)
  {
    if (0 != self->m_hwTimerHandler->setMode)
    {
        self->m_hwTimerHandler->setMode(self->m_isRecurring ? HwTimerMode_continuous : HwTimerMode_oneShot);
    }
    if (0 != self->m_hwTimerHandler->setIntervallMicros)
    {    
        self->m_hwTimerHandler->setIntervallMicros(self->m_delayMicros);
    }
    if (0 != self->m_hwTimerHandler->start)
    {    
        self->m_hwTimerHandler->start();
    }
  }
  else
  {
    if (0 != self->m_funcTMicros)
    {
      self->m_currentTimeMicros = self->m_funcTMicros();
    }
    startInterval(self);
  }
}

void SpinTimer_cancel(SpinTimer self)
{
  self->m_isRunning = false;
  self->m_isExpiredFlag = false;

  if (0 != self->m_hwTimerHandler)
  {
    if (0 != self->m_hwTimerHandler->stop)
    {
        self->m_hwTimerHandler->stop();
    }
  }
}

bool SpinTimer_isRunning(SpinTimer self)
{
  return self->m_isRunning;
}

bool SpinTimer_isExpired(SpinTimer self)
{
  internalTick(self); // TODO : be aware of not running this when HwTimerHandler is assigned !!
  bool isExpired = self->m_isExpiredFlag;
  self->m_isExpiredFlag = false;
  return isExpired;
}

// TODO : be aware of not running this when HwTimerHandler is assigned !!
void SpinTimer_tick(SpinTimer self)
{
  internalTick(self);
}

void SpinTimer_assignTimeExpiredCallback(SpinTimer self, void (*timeExpired)())
{
  self->m_funcTimeExpired = timeExpired;
}

void SpinTimer_assignUptimeInfoCallout(SpinTimer self, uint32_t (*tMillis)())
{
  if (0 != tMillis)
  {
    // mutual exclusion of HwTimerHandler operation as soon as uptime lookup callout is assigned
    self->m_hwTimerHandler = 0;
  }
  self->m_funcTMicros = tMillis;
}

void SpinTimer_assignHwTimerHandler(SpinTimer self, HwTimerHandler hwTimerHandler)
{
  if (0 != hwTimerHandler)
  {
    // mutual exclusion of uptime lookup mode as soon as HwTimerHandler is assigned
    self->m_funcTMicros = 0;
  }
  self->m_hwTimerHandler = hwTimerHandler;
}

// TODO : be aware of not running this when HwTimerHandler is assigned !!
void startInterval(SpinTimer self)
{
  uint32_t deltaTime = ULONG_MAX - self->m_currentTimeMicros;
  self->m_willOverflow = (deltaTime < self->m_delayMicros);
  if (self->m_willOverflow)
  {
    // overflow will occur
    self->m_triggerTimeMicros = self->m_delayMicros - deltaTime - 1;
    self->m_triggerTimeMicrosUpperLimit = self->m_currentTimeMicros;
  }
  else
  {
    self->m_triggerTimeMicros = self->m_currentTimeMicros + self->m_delayMicros;
    self->m_triggerTimeMicrosUpperLimit = ULONG_MAX - deltaTime;
  }
}


// TODO : be aware of not running this when HwTimerHandler is assigned !!
void internalTick(SpinTimer self)
{
  bool intervalIsOver = false;

  if (0 != self->m_funcTMicros)
  {
    self->m_currentTimeMicros = self->m_funcTMicros();

    // check if interval is over as long as the timer shall be running
    if (self->m_isRunning)
    {
      if (self->m_willOverflow)
      {
        intervalIsOver = ((self->m_triggerTimeMicros <= self->m_currentTimeMicros) && (self->m_currentTimeMicros < self->m_triggerTimeMicrosUpperLimit));
      }
      else
      {
        intervalIsOver = ((self->m_triggerTimeMicros <= self->m_currentTimeMicros) || (self->m_currentTimeMicros < self->m_triggerTimeMicrosUpperLimit));
      }
      
      if (intervalIsOver)
      {
        // interval is over
        if (self->m_isRecurring)
        {
          // start next interval
          startInterval(self);
        }
        else
        {
          self->m_isRunning = false;
        }

        self->m_isExpiredFlag = true;
        if (0 != self->m_funcTimeExpired)
        {
          self->m_funcTimeExpired();
        }
      }
    }
  }
}
