
#include <sys/time.h>
#include "LinuxUptimeInfo.h"

unsigned long LinuxUptimeInfo_tMillis()
{
  struct timeval tp;
  gettimeofday(&tp, 0);
  unsigned long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  return ms;
}
