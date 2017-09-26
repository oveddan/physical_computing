#define switchPin 2
#define ledA 3
#define ledB 4

int ledAVal = 0;
int ledBVal = 1;

void setup() {
  Serial.begin(9600);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);

  digitalWrite(ledA, ledAVal);
  digitalWrite(ledB, ledBVal);

}

bool buttonOn;

void loop() {
  int value = digitalRead(switchPin);

  if (value == 1) {
    buttonOn = true;
  }

  if (value == 0 && buttonOn) {
    buttonOn = false;
    ledAVal = 1 - ledAVal;
    ledBVal = 1 - ledBVal;
    digitalWrite(ledA, ledAVal);
    digitalWrite(ledB, ledBVal);
  }
}
