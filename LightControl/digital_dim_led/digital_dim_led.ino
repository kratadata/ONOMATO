int LEDPin= 9;
void setup() {
  pinMode(LEDPin, OUTPUT);

}

void loop() {
for (int i=0; i<= 255; i++)
{
  analogWrite(LEDPin, i);
  delay(20);
}
}
