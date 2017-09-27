#define RESISTOR_PIN A1
#define SWITCH_PIN 2
void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(RESISTOR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("reset");
}

void loop() {
  Serial.println(digitalRead(SWITCH_PIN));
}
