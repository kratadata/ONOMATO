#include <LSS.h>
#include <SoftwareSerial.h>
SoftwareSerial servoSerial(8, 9);

#define ledPin 6
#define LSS_ID    (0)
#define LSS_ID2   (1)
#define LSS_BAUD  (LSS_DefaultBaud)

// Create one LSS object
LSS myLSS = LSS(LSS_ID);
LSS myLSS2 =  LSS(LSS_ID2);
int ServoPosition = 0;
int motorValue;

void setup() {
  servoSerial.begin(LSS_BAUD);
  LSS::initBus(servoSerial, LSS_BAUD);
  Serial.begin(LSS_DefaultBaud);
  pinMode(LED_BUILTIN, OUTPUT);
  
  myLSS.wheelRPM(0);
  myLSS2.wheelRPM(0);
  delay(300);

}

void loop() {
  int processingValue = 0;
  delay(1);
  
  if (Serial.available() > 0)
  {
    // read string
    String str = Serial.readStringUntil('\n');
    processingValue= str.toInt();
    delay(5);
    
    if (processingValue > 20) {
      processingValue = map(processingValue,0,10,0,255);
      motorValue = map(processingValue, 0, 10, 0, 120);
      analogWrite(ledPin, processingValue);
      myLSS.wheelRPM(motorValue);
      delay(5);
      myLSS2.wheelRPM(motorValue);
      
    } else {
      analogWrite(ledPin, 0);
      myLSS.wheelRPM(0);
      delay(5);
      myLSS2.wheelRPM(0);
    }
  }
}
