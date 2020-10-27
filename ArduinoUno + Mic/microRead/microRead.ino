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

int motorValue;
int lightValue;
bool playing = false;
unsigned long currentMillis;
const unsigned long period = 1000;

void setup()
{
  servoSerial.begin(LSS_DefaultBaud);
  LSS::initBus(servoSerial, LSS_DefaultBaud);
  Serial.begin(115200);
 // setupMp3();
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

  if (signalMax > 360) {
    delay(1);
    lightValue = map(signalMax, 400, 500, 0, 255);
    motorValue = map(signalMax, 350, 500, 30, 120);
    analogWrite(ledPin, lightValue);
    myLSS.wheelRPM(motorValue);
    myLSS2.wheelRPM(motorValue);
  } else {
    analogWrite(ledPin, 0);
    myLSS.wheelRPM(0);
    myLSS2.wheelRPM(0);
  }

  if (signalMax < 390) {
    if (currentMillis - startMillis >= period) {
      if (playing) {
        fadeOutMp3();
        playing = false;
        startMillis = currentMillis;
        Serial.println("stop once");
      }
    }
  } else {
    if (currentMillis - startMillis >= period) {
      if (!playing) {
        fadeInMp3();
        Serial.println("play once");
        playing = true;
       startMillis= currentMillis;
      }
    }
    delay(5);
  }
}
