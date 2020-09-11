
#include <stdio.h>
#include "MockTimerAdapter.h"

static unsigned int calls;

void MockTimerAdapter_timeExpired()
{
    calls = calls + 1;
    printf("NOTE: MockTimerAdapter_timeExpired()!\n");
}

void MockTimerAdapter_resetNumberOfCalls()
{
    calls = 0;
}

unsigned int MockTimerAdapter_getNumberOfCalls()
{
    return calls;
}
