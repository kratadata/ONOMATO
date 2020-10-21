
#define ledPin A0


const int numReadings = 10;
int readings[numReadings]; 
  
int readIndex = 0;
int weightedAverage = 0; 
int total = 0;          
int oldWeighted;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  int oldWeighted = 0;
  
}

void loop() {
 
  int sensorValue = round(Serial.read());
  
  //weightedAverage = weightedAverage*0.7;
  //weightedAverage += sensorValue*0.3;

  total = total - readings[readIndex];
  readings[readIndex] = sensorValue;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  
    if (readIndex >= numReadings) {
    readIndex = 0;
  }
  
  weightedAverage = total / numReadings;

  if (weightedAverage > oldWeighted){
  weightedAverage = map(sensorValue,0, 100, 0,255);
  analogWrite(ledPin, weightedAverage);
  } else {
   analogWrite(ledPin, 0); 
  }
  
  oldWeighted = weightedAverage;
 
  
  Serial.print("sensor: ");
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(weightedAverage);
  delay(1);
  
}
