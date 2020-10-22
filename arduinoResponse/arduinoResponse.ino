#include <LSS.h>
#include <SoftwareSerial.h>
SoftwareSerial servoSerial(8, 9);

#define ledPin 6
#define LSS_ID    (0)
#define LSS_BAUD  (LSS_DefaultBaud)

// Create one LSS object
LSS myLSS = LSS(LSS_ID);
int ServoPosition = 0;


void setup() {
  servoSerial.begin(LSS_BAUD);
  LSS::initBus(servoSerial, LSS_BAUD);
  Serial.begin(LSS_DefaultBaud);
  pinMode(LED_BUILTIN, OUTPUT);
  
  myLSS.wheelRPM(0);
  delay(300);

}

void loop() {
  int sensorValue = 0;
  delay(1);
  if (Serial.available() > 0)
  {
    // read string
    String str = Serial.readStringUntil('\n');
    sensorValue = str.toInt();
    delay(5);
    
    if (sensorValue >= 15) {
      sensorValue = map(sensorValue,0,10,0,255);
      analogWrite(ledPin, sensorValue);
      myLSS.wheelRPM(60);
    } else {
      analogWrite(ledPin, 0);
      myLSS.wheelRPM(0);
    }
  }
}
