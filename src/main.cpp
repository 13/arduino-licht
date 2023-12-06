#include <Arduino.h>
#include <LowPower.h>

#define LEDON 25

#define SLPTIME

#if defined(__AVR_ATtiny85__)
#define ledPin1 0
#define ldrPin1 A1
#define pirPin 1 // INTERRUPT: PB1 / EXT. INTERRUPT: PB2
#else
#define VERBOSE
#ifdef LEDON
#undef LEDON
#define LEDON 10
#endif
#define ledPin1 10
#define ldrPin1 A0
#define pirPin 2 // 2 or 3
#endif

int ldrThreshold = 100;
int ldrValue = 0;
int pirValue = 0;
volatile bool motionDetected = false;

void blinkLED(int numBlinks)
{
#ifdef VERBOSE
  Serial.print(F("> LED: BLINK "));
  Serial.println(numBlinks);
#endif
  for (int i = 0; i < numBlinks; ++i)
  {
    digitalWrite(ledPin1, HIGH);
    delay(200);
    digitalWrite(ledPin1, LOW);
    delay(200);
  }
  delay(500);
}

void turnOnLED(int seconds)
{
#ifdef VERBOSE
  Serial.print(F("> LED: ON "));
  Serial.println(seconds);
#endif
  digitalWrite(ledPin1, HIGH);
  delay(seconds * 1000);
  digitalWrite(ledPin1, LOW);
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

void wakeUp()
{
  motionDetected = true;
#ifdef VERBOSE
  pirValue = digitalRead(pirPin);
  Serial.println(F("> WakeUp... "));
  Serial.print(F("> PIR: "));
  Serial.println(pirValue);
  Serial.print(F("> MOTION: "));
  Serial.println(motionDetected);
#endif
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
  pinMode(pirPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pirPin), wakeUp, RISING);
}

void loop()
{
  ldrValue = analogRead(ldrPin1);
  pirValue = digitalRead(pirPin);
#ifdef VERBOSE
  Serial.print(F("> LDR: "));
  Serial.println(ldrValue);
  Serial.print(F("> PIR: "));
  Serial.println(pirValue);
  Serial.print(F("> MOTION: "));
  Serial.println(motionDetected);
#endif
  if (ldrValue < ldrThreshold && pirValue == HIGH) // motionDetected
  {
    turnOnLED(LEDON);
  }
  else
  {
    digitalWrite(ledPin1, LOW);
#ifdef SLPTIME
    motionDetected = false;
    sleepDeep();
#endif
  }
}
