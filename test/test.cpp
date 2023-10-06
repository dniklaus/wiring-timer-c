#include <gtest/gtest.h>
#include <stdbool.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif
  #include "SpinTimer.h"
  #include "MockTimerAction.h"
  #include "SpinTimerContext.h"
  #include "SpinTimerUptimeInfo.h"
  #include "SpinTimerUptimeInfoAdapter.h"
  #include "StubSpinTimersUptimeInfoAdapter.h"
#ifdef __cplusplus
}
#endif

const uint32_t c_countMax = ((1 << 24) - 3);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
#if 0
TEST(SpinTimer, timer_create_allDefaults_cont_test)
{
  SpinTimer* timer = SpinTimer_create(SpinTimerMode_continuous);
  EXPECT_TRUE(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()) == 0);
  EXPECT_TRUE(SpinTimer_getMode(timer) == SpinTimerMode_continuous);
  EXPECT_FALSE(SpinTimer_isRunning(timer));
  EXPECT_FALSE(SpinTimer_isExpired(timer));
  EXPECT_TRUE(timer->action(timer) == 0);
}

TEST(SpinTimer, timer_create_allDefaults_oneShot_test)
{
  SpinTimer* timer = SpinTimer_create(SpinTimerMode_oneShot);
  EXPECT_TRUE(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()) == 0);
  EXPECT_TRUE(SpinTimer_getMode(timer) == SpinTimerMode_oneShot);
  EXPECT_FALSE(SpinTimer_isRunning(timer));
  EXPECT_FALSE(SpinTimer_isExpired(timer));
  EXPECT_TRUE(timer->action(timer) == 0);
}
#endif

TEST(SpinTimer, timer_polling_startZero_test)
{
  StubSpinTimersUptimeInfoAdapter* stubSpinTimersUptimeInfoAdapter = StubSpinTimersUptimeInfoAdapter_create();
  SpinTimerUptimeInfo_instance()->setAdapter(SpinTimerUptimeInfo_instance(), (SpinTimerUptimeInfoAdapter*)stubSpinTimersUptimeInfoAdapter);
  
  const uint32_t delayMicros = 200;
  const uint32_t startMicros = 0;
  const uint32_t expEndMicros = startMicros + delayMicros;
  uint8_t i = 0;

  SpinTimer* spinTimer = SpinTimer_create(SpinTimerMode_oneShot);
  spinTimer->start(spinTimer, delayMicros);
  EXPECT_EQ(spinTimer->isRunning(spinTimer), true);
  EXPECT_EQ(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()), startMicros);
  while (!spinTimer->isExpired(spinTimer)) 
  {
    i++;
    EXPECT_EQ(spinTimer->isRunning(spinTimer), true);
    stubSpinTimersUptimeInfoAdapter->incrementCount(stubSpinTimersUptimeInfoAdapter);
    EXPECT_EQ(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()), startMicros + i * 50);
  }

  EXPECT_EQ(spinTimer->isExpired(spinTimer), false);
  EXPECT_EQ(spinTimer->isRunning(spinTimer), false);
  EXPECT_EQ(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()), expEndMicros);
}

TEST(SpinTimer, timer_polling_startMax_test)
{
  StubSpinTimersUptimeInfoAdapter* stubSpinTimersUptimeInfoAdapter = StubSpinTimersUptimeInfoAdapter_create();
  SpinTimerUptimeInfo_instance()->setAdapter(SpinTimerUptimeInfo_instance(), (SpinTimerUptimeInfoAdapter*)stubSpinTimersUptimeInfoAdapter);

  const uint32_t delayMicros = 400;
  const uint32_t startMicros = SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance());
  const uint32_t expEndMicros = delayMicros - (SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance()) - startMicros) - 50;
  uint8_t i = 0;

  stubSpinTimersUptimeInfoAdapter->setTimeMicros(stubSpinTimersUptimeInfoAdapter, startMicros);
  EXPECT_EQ(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()), c_countMax*50);

  SpinTimer* spinTimer = SpinTimer_create(SpinTimerMode_oneShot);
  spinTimer->start(spinTimer, delayMicros);
  EXPECT_EQ(spinTimer->attr.delayMicros, delayMicros);
  EXPECT_EQ(spinTimer->attr.triggerTimeMicros, delayMicros - 50);
  EXPECT_EQ(spinTimer->attr.currentTimeMicros, SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance()));
  EXPECT_EQ(spinTimer->attr.maxUptimeValue, SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance()));
  EXPECT_EQ(spinTimer->isRunning(spinTimer), true);
  EXPECT_NE(SpinTimerUptimeInfo_instance(), (SpinTimerUptimeInfo*)0);
  EXPECT_NE(spinTimer->isExpired(spinTimer), true);
  while (!spinTimer->isExpired(spinTimer)) 
  {
    i++;
    EXPECT_EQ(spinTimer->isRunning(spinTimer), true);
    stubSpinTimersUptimeInfoAdapter->incrementCount(stubSpinTimersUptimeInfoAdapter);
    EXPECT_EQ(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()), (i-1) * 50);
  }

  EXPECT_EQ(spinTimer->isExpired(spinTimer), false);
  EXPECT_EQ(spinTimer->isRunning(spinTimer), false);
  EXPECT_EQ(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()), expEndMicros);
}

#if 0
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
#endif


TEST(SpinTimer, timer_testTickAndCallback_test)
{
  StubSpinTimersUptimeInfoAdapter* stubSpinTimersUptimeInfoAdapter = StubSpinTimersUptimeInfoAdapter_create();
  SpinTimerUptimeInfo_instance()->setAdapter(SpinTimerUptimeInfo_instance(), (SpinTimerUptimeInfoAdapter*)stubSpinTimersUptimeInfoAdapter);

  const uint32_t delayMicros = 300;
  const uint32_t startMicros = SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance());
  const uint32_t expEndMicros = delayMicros - (SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance()) - startMicros) - 50;
  uint8_t i = 0;

  stubSpinTimersUptimeInfoAdapter->setTimeMicros(stubSpinTimersUptimeInfoAdapter, startMicros);
  EXPECT_EQ(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()), c_countMax*50);

  MockTimerAction* mockTimerAction = MockTimerAction_create();
  SpinTimer* spinTimer = SpinTimer_create(SpinTimerMode_oneShot);
  spinTimer->assignAction(spinTimer, (SpinTimerAction*)mockTimerAction);

  EXPECT_NE(SpinTimerUptimeInfo_instance(), (SpinTimerUptimeInfo*)0);

  spinTimer->start(spinTimer, delayMicros);
  EXPECT_EQ(spinTimer->attr.delayMicros, delayMicros);
  EXPECT_EQ(spinTimer->attr.triggerTimeMicros, delayMicros - 50);
  EXPECT_EQ(spinTimer->attr.currentTimeMicros, startMicros);
  EXPECT_EQ(spinTimer->attr.maxUptimeValue, SpinTimerUptimeInfo_instance()->maxTimeValue(SpinTimerUptimeInfo_instance()));
  EXPECT_EQ(spinTimer->isRunning(spinTimer), true);
  EXPECT_NE(SpinTimerUptimeInfo_instance(), (SpinTimerUptimeInfo*)0);
  EXPECT_EQ(spinTimer->attr.currentTimeMicros, SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()));
  EXPECT_EQ(spinTimer->isExpired(spinTimer), false);

  while((i<10) && (SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()) != expEndMicros))
  {
    EXPECT_EQ(spinTimer->isRunning(spinTimer), true);
    i++;
    stubSpinTimersUptimeInfoAdapter->incrementCount(stubSpinTimersUptimeInfoAdapter);
    SpinTimerContext_instance()->handleTick(SpinTimerContext_instance());
  }

  EXPECT_EQ(spinTimer->isRunning(spinTimer), false);
  EXPECT_EQ(spinTimer->isExpired(spinTimer), true);
  EXPECT_EQ(spinTimer->isExpired(spinTimer), false);

  EXPECT_EQ(mockTimerAction->getNumberOfCalls(mockTimerAction), 1);
  EXPECT_EQ(SpinTimerUptimeInfo_instance()->currentTimeMicros(SpinTimerUptimeInfo_instance()), expEndMicros);
}

#if 0
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
#endif
