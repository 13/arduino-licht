#include <Arduino.h>
#include <LowPower.h>

// #define VERBOSE

#define SLPTIME 24 // seconds

#if defined(__AVR_ATtiny85__)
#define ledPin1 0
#define ldrPin1 A2
#define pirPin 2
#else
#define ledPin1 10
#define ldrPin1 A0
#define pirPin 2
#endif

int ldrThreshold = 100;
int ldrValue = 0;

void blinkLED(int numBlinks)
{
  for (int i = 0; i < numBlinks; ++i)
  {
    digitalWrite(ledPin1, HIGH);
    delay(200);
    digitalWrite(ledPin1, LOW);
    delay(200);
  }
  delay(500);
}

#ifdef SLPTIME
void sleepDeep()
{
#ifdef VERBOSE
  Serial.println(F("> SleepDeep Forever"));
  delay(1000);
#endif
  delay(100);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}
#endif

void motionDetected()
{
  ldrValue = analogRead(ldrPin1);
#ifdef VERBOSE
  Serial.print(F("> LDR: "));
  Serial.println(ldrValue);
#endif
  if (ldrValue < ldrThreshold)
  {
    digitalWrite(ledPin1, HIGH);
  }
  else
  {
    digitalWrite(ledPin1, LOW);
#ifdef SLPTIME
    sleepDeep();
#endif
  }
}

void setup()
{
#ifdef VERBOSE
  Serial.begin(9600);
  delay(3000);
  Serial.println(F("> "));
  Serial.println(F("> Booting..."));
#endif
  blinkLED(3);
  pinMode(ldrPin1, INPUT);
  pinMode(pirPin, INPUT_PULLUP); // Set PIR pin as input with internal pull-up resistor

  attachInterrupt(digitalPinToInterrupt(pirPin), motionDetected, RISING);
}

void loop()
{
  ldrValue = analogRead(ldrPin1);
#ifdef VERBOSE
  Serial.print(F("> LDR: "));
  Serial.println(ldrValue);
#endif
  if (ldrValue < ldrThreshold)
  {
    digitalWrite(ledPin1, HIGH);
  }
  else
  {
    digitalWrite(ledPin1, LOW);
#ifdef SLPTIME
    sleepDeep();
#endif
  }
}