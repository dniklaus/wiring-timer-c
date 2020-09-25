#include <SpinTimer.h>

void toggleLed(int ledPin)
{
  bool isLedOn = digitalRead(ledPin);
  digitalWrite(ledPin, !isLedOn);
}

const unsigned int  BLINK_TIME_MILLIS = 200;

void timeExpired()
{
  toggleLed(LED_BUILTIN);
}

// The setup function is called once at startup of the sketch
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  SpinTimer_create(SpinTimer_IS_RECURRING);
  SpinTimer_assignTimeExpiredCallback(&timeExpired);
  SpinTimer_start(BLINK_TIME_MILLIS);
}

// The loop function is called in an endless loop
void loop()
{
  SpinTimer_tick();
}
  
