int led = 9;           
int brightness = 0;    
int fadeAmount = 5; 
//int ledValue = 0;
//int noiseValue = 0;   

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  //fading
  analogWrite(led, brightness);

  brightness = brightness + fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(50);
  // noise map for led
  // ledValue = map(noiseValue, 0, 1260, 0, 255);
  //digitalWrite(led, ledValue);
}
