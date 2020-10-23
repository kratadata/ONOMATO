#include <LSS.h>
#include <SoftwareSerial.h>

#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
#include <ESP8266WiFi.h>
#endif


SoftwareSerial servoSerial(8, 9);
#define LSS_ID    (0)
#define LSS_ID2   (1)
#define LSS_BAUD  (LSS_DefaultBaud)

#define ledPin 6
#define speakerR 3
#define speakerL 4

//wifi settings

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.shiftr.io";
int        port     = 1883;
const char topic[]  = "/soundVolume";


// Create one LSS object
LSS myLSS = LSS(LSS_ID);
LSS myLSS2 =  LSS(LSS_ID2);

int motorValue;
int processingValue;


void setup() {
 
  Serial.begin(LSS_DefaultBaud);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
  // MQTT CONNECTION
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  
  mqttClient.setUsernamePassword("311b2d0d", "a1617db7c1ade904");
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }
   // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  // subscribe to a topic
  mqttClient.subscribe(topic);

  servoSerial.begin(LSS_BAUD);
  LSS::initBus(servoSerial, LSS_BAUD);
  pinMode(LED_BUILTIN, OUTPUT);
  myLSS.wheelRPM(0);
  myLSS2.wheelRPM(0);
  delay(200);
}

void loop() {
  mqttClient.poll();
}


void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  // use the Stream interface to print the contents
  String payload = "";
  while (mqttClient.available()) {
    payload = payload + (char)mqttClient.read();
  }
  
  Serial.println(payload);
  processingValue= payload.toInt();
  
  if (processingValue > 5) {
      processingValue = map(processingValue,0,10,0,255);
      motorValue =  map(processingValue, 5, 30, 60, 120);
      analogWrite(ledPin, processingValue);
      myLSS.wheelRPM(motorValue);
      myLSS2.wheelRPM(motorValue);

    } else {
      analogWrite(ledPin, 0);
      myLSS.wheelRPM(0);
      myLSS2.wheelRPM(0);

    }
}
