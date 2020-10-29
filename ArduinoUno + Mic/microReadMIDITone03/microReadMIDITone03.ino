#include <SPI.h>
#include <LSS.h>
#include <SoftwareSerial.h>

SoftwareSerial servoSerial(12, 13);

#define LSS_ID    (0)
#define LSS_ID2   (1)
#define ledPin 10

LSS myLSS =   LSS(LSS_ID);
LSS myLSS2 =  LSS(LSS_ID2);

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
uint8_t result;

int treshhold = 350;
int treshholdMax = 400;

int motorValue;
int lightValue;
bool playing = false;
bool BPMTiming = false;

int toneDuration;

#include "pitches.h"
#include "tones.h"

int melody[] = {
  b5, b5b , b5, b5b , b5, b5b, b5, b5b, b5, b5b, b5, b5b , b5, b5b, b5, b5b
};

int noteDurations[] = {
  128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128
};

void setup()
{
  servoSerial.begin(LSS_DefaultBaud);
  LSS::initBus(servoSerial, LSS_DefaultBaud);
  Serial.begin(115200);

}

void loop() {
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
    }
  }
  Serial.println(signalMax);

  if (signalMax > treshhold) {

    //lightValue = map(signalMax, treshhold, treshholdMax, 0, 255);
    motorValue = map(signalMax, treshhold, treshholdMax, 50, 75);
    toneDuration = map(signalMax, treshhold, treshholdMax, 5000, 2000);
    analogWrite(ledPin, random(0,255));
    delay(5);
    myLSS.wheelRPM(motorValue);
    delay(100);
    myLSS2.wheelRPM(motorValue);

    if (!playing)
    {

      for (int thisNote = 0; thisNote < sizeof(melody) - 1 ; thisNote++) {
        analogWrite(ledPin, random(0,255));
        int noteDuration = toneDuration / noteDurations[thisNote];
        tone(11, melody[thisNote], noteDuration);
        tone(5, melody[thisNote], noteDuration);
        delay(30);
        noTone(11);
        noTone(5);
      }

      BPMTiming = true;
      playing = true;
    }

    if (BPMTiming)
    {
      for (int i = 0; i < 2; i++) {
        for (int thisNote = 0; thisNote < sizeof(melody) - 1 ; thisNote++) {
          int noteDuration = toneDuration / noteDurations[thisNote];
          tone(11, melody[thisNote], noteDuration);
          tone(5, melody[thisNote], noteDuration);
          delay(30);
          noTone(11);
          noTone(5);
        }
      }
      BPMTiming = false;
    }

  } else {
    analogWrite(ledPin, 255);
    myLSS.wheelRPM(0);
    delay(100);
    myLSS2.wheelRPM(0);

    if (playing)  {
      noTone(11);
      noTone(5);
      playing = false;
    }
  }
  
  delay(100);
}
