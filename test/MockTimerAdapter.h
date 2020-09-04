#ifndef __MockTimerAdapter_H
#define __MockTimerAdapter_H

void MockTimerAdapter_timeExpired();
void MockTimerAdapter_resetNumberOfCalls();
unsigned int MockTimerAdapter_getNumberOfCalls();

#endif
