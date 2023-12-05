#include <Arduino.h>
#include <LowPower.h>

// #define VERBOSE

#define SLPTIME 24 // seconds

#if defined(__AVR_ATtiny85__)
#define ledPin1 0
#define ldrPin1 A2
#define pirPin 1 // INTERRUPT: PB1 / EXT. INTERRUPT: PB2
#else
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
    digitalWrite(ledPin1, HIGH);
  }
  else
  {
    digitalWrite(ledPin1, LOW);
#ifdef SLPTIME
    motionDetected = true;
    sleepDeep();
#endif
  }
}
