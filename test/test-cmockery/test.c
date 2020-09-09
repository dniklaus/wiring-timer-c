#include <stdbool.h>
#include <limits.h>
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

void timer_polling_test(void **state)
{
  SpinTimer_create(false);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(ULONG_MAX-1);

  SpinTimer_start(100);
  assert_int_equal(SpinTimer_isRunning(), true);
  assert_int_equal(SpinTimer_isExpired(), false); 

  while (!SpinTimer_isExpired()) 
  {
    StubTestUptimeInfo_incrementTMillis();
    assert_int_equal(SpinTimer_isRunning(), true);
  }

  assert_int_equal(SpinTimer_isExpired(), false);
  assert_int_equal(SpinTimer_isRunning(), false);
  assert_int_equal(StubTestUptimeInfo_tMillis(), 100-1);
}

int main(int argc, char* argv[]) {
    const UnitTest tests[] = {
      unit_test(timer_create_allDefaults_test),
      unit_test(timer_polling_test),
    };
    return run_tests(tests);
}