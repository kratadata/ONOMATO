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

int treshhold = 370;
int tresholdMax = 400;
int motorValue;
int lightValue;
bool playing = false;
bool BPMTiming = false;
int prevTime = 0;
int BPM = 0;
int toneDuration;

#include "pitches.h"
#include "tones.h"

int melody[] = {
  c5, f5, c5, f4, c5, f5, c5, c5, f5, c5, a5, g5, f5, e5, d5, d5b, c5, f5, c5, f4, c5, f5, c5, f5, d5, c5, b4b, a4, g4, f4
};

int noteDurations[] = { 64, 64, 64, 64, 64, 64, 128, 64, 64, 64, 96, 32, 32, 32, 32, 32, 64, 64, 64, 64, 64, 64, 128, 96, 32, 64, 64, 64, 64, 64
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

    lightValue = map(signalMax, treshhold, tresholdMax, 0, 255);
    motorValue = map(signalMax, treshhold, tresholdMax, 60, 120);
    analogWrite(ledPin, lightValue);
    myLSS.wheelRPM(motorValue);
    delay(100);
    myLSS2.wheelRPM(motorValue);

    if (!playing)
    {

      for (int thisNote = 0; thisNote < sizeof(melody) - 1 ; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(11, melody[thisNote], noteDuration);
        tone(5, melody[thisNote], noteDuration);
        delay(20);
        noTone(11);
        noTone(5);
      }

    }


  } else {
    analogWrite(ledPin, 0);
    myLSS.wheelRPM(0);
    delay(100);
    myLSS2.wheelRPM(0);

    if (playing) {
      noTone(11);
      noTone(5);
      playing = false;
    }
  }

  delay(50);

}
