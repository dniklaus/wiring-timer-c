#include <limits.h>
#include <stdbool.h>
#include "SpinTimer.h"

extern const bool SpinTimer_IS_NON_RECURRING = false;
extern const bool SpinTimer_IS_RECURRING     = true;

bool m_isRecurring;
bool m_isRunning;
bool m_isExpiredFlag;
bool m_willOverflow;
unsigned long m_delayMillis;
unsigned long m_currentTimeMillis;
unsigned long m_triggerTimeMillis;
unsigned long m_triggerTimeMillisUpperLimit;

void (*m_funcTimeExpired)();
unsigned long (*m_funcTMillis)();

void internalTick();
void startInterval();

void SpinTimer_create(bool isRecurring)
{
  m_funcTimeExpired = 0;
  m_funcTMillis = 0;
  m_isRecurring = isRecurring;
  m_isRunning = false;
  m_isExpiredFlag = false;
  m_willOverflow = false;
  m_delayMillis = 0;
  m_currentTimeMillis = 0;
  m_triggerTimeMillis = 0;
  m_triggerTimeMillisUpperLimit = 0;
}

void SpinTimer_destroy()
{
  m_funcTimeExpired = 0;
  m_funcTMillis = 0;
}

void SpinTimer_start(unsigned long timeMillis)
{
  m_isRunning = true;
  m_delayMillis = timeMillis;
  if (0 != m_funcTMillis)
  {
    m_currentTimeMillis = m_funcTMillis();
  }
  startInterval();
}

void SpinTimer_cancel()
{
  m_isRunning = false;
  m_isExpiredFlag = false;
}

bool SpinTimer_isRunning()
{
  return m_isRunning;
}

bool SpinTimer_isExpired()
{
  internalTick();
  bool isExpired = m_isExpiredFlag;
  m_isExpiredFlag = false;
  return isExpired;
}

void SpinTimer_tick()
{
  internalTick();
}

void SpinTimer_assignTimeExpiredCallback(void (*timeExpired)())
{
  m_funcTimeExpired = timeExpired;
}

void SpinTimer_assignUptimeInfoCallout(unsigned long (*tMillis)())
{
  m_funcTMillis = tMillis;
}

void startInterval()
{
  unsigned long deltaTime = ULONG_MAX - m_currentTimeMillis;
  m_willOverflow = (deltaTime < m_delayMillis);
  if (m_willOverflow)
  {
    // overflow will occur
    m_triggerTimeMillis = m_delayMillis - deltaTime - 1;
    m_triggerTimeMillisUpperLimit = m_currentTimeMillis;
  }
  else
  {
    m_triggerTimeMillis = m_currentTimeMillis + m_delayMillis;
    m_triggerTimeMillisUpperLimit = ULONG_MAX - deltaTime;
  }
}

void internalTick()
{
  bool intervalIsOver = false;

  if (0 != m_funcTMillis)
  {
    m_currentTimeMillis = m_funcTMillis();

    // check if interval is over as long as the timer shall be running
    if (m_isRunning)
    {
      if (m_willOverflow)
      {
        intervalIsOver = ((m_triggerTimeMillis <= m_currentTimeMillis) && (m_currentTimeMillis < m_triggerTimeMillisUpperLimit));
      }
      else
      {
        intervalIsOver = ((m_triggerTimeMillis <= m_currentTimeMillis) || (m_currentTimeMillis < m_triggerTimeMillisUpperLimit));
      }
      
      if (intervalIsOver)
      {
        // interval is over
        if (m_isRecurring)
        {
          // start next interval
          startInterval();
        }
        else
        {
          m_isRunning = false;
        }

        m_isExpiredFlag = true;
        if (0 != m_funcTimeExpired)
        {
          m_funcTimeExpired();
        }
      }
    }
  }
}
