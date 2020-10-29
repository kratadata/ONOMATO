#include <SPI.h>
#include <LSS.h>
#include <SoftwareSerial.h>

SoftwareSerial servoSerial(12, 13);

#define LSS_ID    (0)
#define LSS_ID2   (1)
#define ledPin 11

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

#include "pitches.h"
int speakerRight = 10; 
int speakerLeft = 5; 

int pause = 1000; 
int notes = 0;
int beat  = 0;
long duration = 0;
long tempo = 10000;
int rest_count = 100;

int melody[] = {c5, f5, c5, f4, c5, f5, c5, c5, f5, c5, a5, g5, f5, e5, d5, d5b, c5, f5, c5, f4, c5, f5, c5, f5, d5, c5, b4b, a4, g4, f4 }; //Tones/notes
int beats[] = { 64, 64, 64, 64, 64, 64, 128, 64, 64, 64, 96, 32, 32, 32, 32, 32, 64, 64, 64, 64, 64, 64, 128, 96, 32, 64, 64, 64, 64, 64 }; //Tone/note lengths
int MAX_COUNT = sizeof(melody)/2;

void setup()
{
  servoSerial.begin(LSS_DefaultBaud);
  LSS::initBus(servoSerial, LSS_DefaultBaud);
  Serial.begin(115200);
  pinMode(speakerRight, OUTPUT); 
  pinMode(speakerLeft, OUTPUT); 
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
    
    lightValue = map(signalMax, treshhold, 500, 0, 255);
    motorValue = map(signalMax, treshhold, 400, 60, 120);
    analogWrite(ledPin, lightValue);
    myLSS.wheelRPM(motorValue);
    delay(100);
    myLSS2.wheelRPM(motorValue);

    if (!playing)
    {
       for (int i=0; i<MAX_COUNT; i++) { 
          notes = melody[i]; //Defining which notes to play
          beat = beats[i]; //Defining the note lengths to use
          duration = beat * tempo; //Set up timing
          playTone(); 
          delayMicroseconds(pause);
    }
      playing = true;
    }

  } else {

    analogWrite(ledPin, 0);
    myLSS.wheelRPM(0);

    delay(100);
    myLSS2.wheelRPM(0);

    if (playing) {
      noTone(10);
      noTone(5);
      playing = false;
    }

  }
  
  delay(250);

}

void playTone() { 
  long elapsed_time = 0;
  if (melody > 0) { 
    while (elapsed_time < duration) {
      digitalWrite(speakerRight,HIGH);
       digitalWrite(speakerLeft,HIGH);
       delayMicroseconds(notes/ 2);
       digitalWrite(speakerRight,LOW);
       digitalWrite(speakerLeft,LOW);
      delayMicroseconds(notes / 2);
      elapsed_time += (notes);
    } 
  }
  else {
    for (int j = 0; j < rest_count; j++) {
      delayMicroseconds(duration);  
    }                                
  }                                 
}
