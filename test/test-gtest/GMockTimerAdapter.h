#ifndef __GMockTimerAdapter_H
#define __GMockTimerAdapter_H

#include <gmock/gmock.h>

void gMockTimerAdapter_timeExpired();

class TimerAdapter
{
public:
  virtual ~TimerAdapter() { }
  virtual void timeExpired() = 0;
};

class GMockTimerAdapter : public TimerAdapter
{
public:
  virtual ~GMockTimerAdapter() { }
  MOCK_METHOD0(timeExpired, void());
};

extern GMockTimerAdapter GMockTimerAdapterObj;

#endif