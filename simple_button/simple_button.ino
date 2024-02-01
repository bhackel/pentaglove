// Test reading of the glove connections

#define BUTTON_PIN 4

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}
void loop()
{
  Serial.println(digitalRead(BUTTON_PIN)); // when 4 is connected to ground, this returns 0
  delay(10);
}