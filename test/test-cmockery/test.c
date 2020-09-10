#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmockery.h>
#include <SpinTimer.h>
#include <SpinTimerAdapter.h>
#include <UptimeInfo.h>
#include <StubTestUptimeInfo.h>

void timer_create_allDefaults_test(void **state)
{
  SpinTimer_create(false);
  
  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
}

void timer_polling_startZero_test(void **state)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = 0;
  // const unsigned long int startMillis = ULONG_MAX;
  // const unsigned long int startMillis = ULONG_MAX-1;
  // const unsigned long int startMillis = ULONG_MAX-delayMillis+1;
  // const unsigned long int startMillis = ULONG_MAX-delayMillis;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

void timer_polling_startMax_test(void **state)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX;
  // const unsigned long int startMillis = ULONG_MAX-1;
  // const unsigned long int startMillis = ULONG_MAX-delayMillis+1;
  // const unsigned long int startMillis = ULONG_MAX-delayMillis;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

void timer_polling_startMaxMinus1_test(void **state)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX-1;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

void timer_polling_startMaxMinusDelayPlus1_test(void **state)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX-delayMillis+1;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

void timer_polling_startMaxMinusDelay_test(void **state)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX-delayMillis;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

void timer_polling_zeroDelay_startZero_test(void **state)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = 0;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

void timer_polling_zeroDelay_startMax_test(void **state)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = ULONG_MAX;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

void timer_polling_zeroDelay_startMaxMinus1_test(void **state)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = ULONG_MAX-1;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

void timer_polling_zeroDelay_startMaxMinusDelay_test(void **state)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = ULONG_MAX-delayMillis;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  assert_int_equal(SpinTimer_isRunning(), true);
  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
  while (!SpinTimer_isExpired()) 
  {
    assert_int_equal(SpinTimer_isRunning(), true);
    // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());
    StubTestUptimeInfo_incrementTMillis();
  }

  // printf("tMillis: 0x%lx (%lu)\n", StubTestUptimeInfo_tMillis(), StubTestUptimeInfo_tMillis());

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), expEndMillis);
}

int main(int argc, char* argv[]) {
    const UnitTest tests[] = {
      unit_test(timer_create_allDefaults_test),
      unit_test(timer_polling_startZero_test),
      unit_test(timer_polling_startMax_test),
      unit_test(timer_polling_startMaxMinus1_test),
      unit_test(timer_polling_startMaxMinusDelay_test),
      unit_test(timer_polling_startMaxMinusDelayPlus1_test),
      unit_test(timer_polling_zeroDelay_startZero_test),
      unit_test(timer_polling_zeroDelay_startMax_test),
      unit_test(timer_polling_zeroDelay_startMaxMinus1_test),
      unit_test(timer_polling_zeroDelay_startMaxMinusDelay_test),

    };
    return run_tests(tests);
}