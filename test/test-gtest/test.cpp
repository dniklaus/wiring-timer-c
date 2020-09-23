#include <gtest/gtest.h>
#include <stdbool.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif
  #include <SpinTimer.h>
  #include <UptimeInfo.h>
  #include <StubTestUptimeInfo.h>
  #include <MockTimerAdapter.h>
#ifdef __cplusplus
}
#endif

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}

TEST(SpinTimer, timer_create_allDefaults_test)
{
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  EXPECT_FALSE(SpinTimer_isRunning());
  EXPECT_FALSE(SpinTimer_isExpired());
}

TEST(SpinTimer, timer_polling_startZero_test)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = 0;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_polling_startMax_test)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_polling_startMaxMinus1_test)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX-1;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_polling_startMaxMinusDelayPlus1_test)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX-delayMillis+1;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_polling_startMaxMinusDelay_test)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX-delayMillis;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_polling_zeroDelay_startZero_test)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = 0;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_polling_zeroDelay_startMax_test)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = ULONG_MAX;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_polling_zeroDelay_startMaxMinus1_test)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = ULONG_MAX-1;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_polling_zeroDelay_startMaxMinusDelay_test)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = ULONG_MAX-delayMillis;
  const unsigned long int expEndMillis = startMillis + delayMillis;
  
  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  
  StubTestUptimeInfo_setTMillis(startMillis);

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  while (!SpinTimer_isExpired()) 
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_testTickAndCallback_test)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = ULONG_MAX-delayMillis;
  const unsigned long int expEndMillis = startMillis + delayMillis;

  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  SpinTimer_assignTimeExpiredCallback(&MockTimerAdapter_timeExpired);

  StubTestUptimeInfo_setTMillis(startMillis);
  MockTimerAdapter_resetNumberOfCalls();

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  EXPECT_EQ(SpinTimer_isExpired(), false);

  while(StubTestUptimeInfo_tMillis() < expEndMillis)
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    SpinTimer_tick();
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), true);
  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);

  EXPECT_EQ(MockTimerAdapter_getNumberOfCalls(), 1);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_testTickAndCallback_zeroDelay_test)
{
  const unsigned long int delayMillis = 0;
  const unsigned long int startMillis = ULONG_MAX-delayMillis;
  const unsigned long int expEndMillis = startMillis + delayMillis;

  SpinTimer_create(SpinTimer_IS_NON_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  SpinTimer_assignTimeExpiredCallback(&MockTimerAdapter_timeExpired);

  StubTestUptimeInfo_setTMillis(startMillis);
  MockTimerAdapter_resetNumberOfCalls();

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);

  while(StubTestUptimeInfo_tMillis() < expEndMillis)
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    SpinTimer_tick();
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), true);
  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), false);

  EXPECT_EQ(MockTimerAdapter_getNumberOfCalls(), 1);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}

TEST(SpinTimer, timer_testRecurringTimer_test)
{
  const unsigned long int delayMillis = 10;
  const unsigned long int startMillis = 0;
  const unsigned long int expEndMillis = startMillis + 2*delayMillis;

  SpinTimer_create(SpinTimer_IS_RECURRING);
  SpinTimer_assignUptimeInfoCallout(&StubTestUptimeInfo_tMillis);
  SpinTimer_assignTimeExpiredCallback(&MockTimerAdapter_timeExpired);

  StubTestUptimeInfo_setTMillis(startMillis);
  MockTimerAdapter_resetNumberOfCalls();

  SpinTimer_start(delayMillis);
  EXPECT_EQ(SpinTimer_isRunning(), true);
  EXPECT_EQ(SpinTimer_isExpired(), false);

  while(StubTestUptimeInfo_tMillis() < expEndMillis)
  {
    EXPECT_EQ(SpinTimer_isRunning(), true);
    SpinTimer_tick();
    StubTestUptimeInfo_incrementTMillis();
  }

  EXPECT_EQ(SpinTimer_isExpired(), true);
  EXPECT_EQ(SpinTimer_isExpired(), false);
  EXPECT_EQ(SpinTimer_isRunning(), true);

  EXPECT_EQ(MockTimerAdapter_getNumberOfCalls(), 2);
  EXPECT_EQ(StubTestUptimeInfo_tMillis(), expEndMillis);
}