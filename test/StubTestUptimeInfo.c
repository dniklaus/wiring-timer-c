#include <sys/time.h>
#include "StubTestUptimeInfo.h"

static unsigned long m_millis;

void StubTestUptimeInfo_setTMillis(unsigned long millis)
{
  m_millis = millis;
}

void StubTestUptimeInfo_incrementTMillis()
{
  m_millis = m_millis + 1;
}

unsigned long StubTestUptimeInfo_tMillis()
{
  return m_millis;
}