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
int motorValue;
int lightValue;
bool playing = false;
unsigned long startTrackMillis;
const unsigned long minTrackLength = 30000;

void setup()
{
  servoSerial.begin(LSS_DefaultBaud);
  LSS::initBus(servoSerial, LSS_DefaultBaud);
  Serial.begin(115200);
  //setupMp3();
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

    delay(1);
    lightValue = map(signalMax, treshhold, 500, 0, 255);
    motorValue = map(signalMax, treshhold, 400, 60, 120);
    analogWrite(ledPin, lightValue);
    myLSS.wheelRPM(motorValue);
    delay(100);
    myLSS2.wheelRPM(motorValue);

  } else {

    Serial.println("stopping led");
    analogWrite(ledPin, 0);
    Serial.println("stopping servo1");
    myLSS.wheelRPM(0);

    delay(100);
    Serial.println("stopping servo2");
    myLSS2.wheelRPM(0);

  }

  if (signalMax > 360) {
    if (!playing)
    {
      Serial.println("play once");
      startTrackMillis = millis();
      fadeInMp3();
      playing = true;
     
    }

    if (playing && (millis() - startTrackMillis > minTrackLength) )
    {
      resumeThisTrack();
    }

  } else {

    if (playing)
    {
      
      startTrackMillis = millis();
      fadeOutMp3();
      playing = false;
    }

    if (!playing && (millis() - startTrackMillis > minTrackLength) )
    {
      stopThisTrack();
    }
  }

  delay(50);
}
