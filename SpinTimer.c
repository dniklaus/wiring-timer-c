#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SpinTimer.h"

const bool SpinTimer_IS_NON_RECURRING = false;
const bool SpinTimer_IS_RECURRING     = true;

struct SpinTimer_t
{
  bool m_isRunning;
  bool m_isRunning;
  bool m_isExpiredFlag;
  bool m_willOverflow;
  unsigned long m_delayMillis;
  unsigned long m_currentTimeMillis;
  unsigned long m_triggerTimeMillis;
  unsigned long m_triggerTimeMillisUpperLimit;
  void (*m_funcTimeExpired)();
  unsigned long (*m_funcTMillis)();
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
  SpinTimer self = (SpinTimer)calloc(1, sizeof(SpinTimer_t));

  self->m_funcTimeExpired = 0;
  self->m_funcTMillis = 0;
  self->m_isRecurring = isRecurring;
  self->m_isRunning = false;
  self->m_isExpiredFlag = false;
  self->m_willOverflow = false;
  self->m_delayMillis = 0;
  self->m_currentTimeMillis = 0;
  self->m_triggerTimeMillis = 0;
  self->m_triggerTimeMillisUpperLimit = 0;

  return self;
}

void SpinTimer_destroy(SpinTimer self)
{
  self->m_funcTimeExpired = 0;
  self->m_funcTMillis = 0;
  self->m_isRecurring = SpinTimer_IS_NON_RECURRING;
  self->m_isRunning = false;
  self->m_isExpiredFlag = false;
}


// TODO: also provide interface to set timeMicros!
void SpinTimer_start(SpinTimer self, unsigned long timeMillis)
{
  self->m_isRunning = true;
  self->m_delayMillis = timeMillis;

  if (0 != self->m_hwTimerHandler)
  {
    self->m_hwTimerHandler->setMode(self->m_hwTimerHandler, aMode);
    self->m_hwTimerHandler->setIntervallMicros(self->m_hwTimerHandler, self->m_delayMillis * 1000); // TODO: deal with delayMicros and delayMillis correctly!
    self->m_hwTimerHandler->start(self->m_hwTimerHandler);
  }
  else
  {
    if (0 != self->m_funcTMillis)
    {
      self->m_currentTimeMillis = m_funcTMillis();
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
    self->m_hwTimerHandler->stop(self->m_hwTimerHandler);
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

void SpinTimer_assignUptimeInfoCallout(SpinTimer self, unsigned long (*tMillis)())
{
  if (0 != tMillis)
  {
    // mutual exclusion of HwTimerHandler operation as soon as uptime lookup callout is assigned
    self->m_hwTimerHandler = 0;
  }
  self->m_funcTMillis = tMillis;
}

void SpinTimer_assignHwTimerHandler(SpinTimer self, HwTimerHandler hwTimerHandler)
{
  if (0 != hwTimerHandler)
  {
    // mutual exclusion of uptime lookup mode as soon as HwTimerHandler is assigned
    self->m_funcTMillis = 0;
  }
  self->m_hwTimerHandler = hwTimerHandler;
}

// TODO : be aware of not running this when HwTimerHandler is assigned !!
void startInterval(SpinTimer self)
{
  unsigned long deltaTime = ULONG_MAX - self->m_currentTimeMillis;
  self->m_willOverflow = (deltaTime < self->m_delayMillis);
  if (self->m_willOverflow)
  {
    // overflow will occur
    self->m_triggerTimeMillis = self->m_delayMillis - deltaTime - 1;
    self->m_triggerTimeMillisUpperLimit = self->m_currentTimeMillis;
  }
  else
  {
    self->m_triggerTimeMillis = self->m_currentTimeMillis + self->m_delayMillis;
    self->m_triggerTimeMillisUpperLimit = ULONG_MAX - deltaTime;
  }
}


// TODO : be aware of not running this when HwTimerHandler is assigned !!
void internalTick(SpinTimer self)
{
  bool intervalIsOver = false;

  if (0 != self->m_funcTMillis)
  {
    self->m_currentTimeMillis = self->m_funcTMillis();

    // check if interval is over as long as the timer shall be running
    if (self->m_isRunning)
    {
      if (self->m_willOverflow)
      {
        intervalIsOver = ((self->m_triggerTimeMillis <= self->m_currentTimeMillis) && (self->m_currentTimeMillis < self->m_triggerTimeMillisUpperLimit));
      }
      else
      {
        intervalIsOver = ((self->m_triggerTimeMillis <= self->m_currentTimeMillis) || (self->m_currentTimeMillis < self->m_triggerTimeMillisUpperLimit));
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
