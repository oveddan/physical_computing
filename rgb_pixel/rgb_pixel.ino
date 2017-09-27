/*
Adafruit Arduino - Lesson 4. 8 LEDs and a Shift Register
*/
 
int latchPin = 5;
int clockPin = 6;
int dataPin = 4;

byte leds[2];

#define NUM_LEDS 16
#define LEDS_PER_COLUMN 3
#define LEDS_PER_SHIFTER 8
#define SWITCH_PIN 2
#define SPEED_PENTIOMETER_PIN A0
#define RESISTOR_PIN A1


int pulseSourceX = -1;
int distanceFromPulse = 0;
unsigned long lastMovement = 0;
int threshold = 620;
 
void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(RESISTOR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT);
  pinMode(SPEED_PENTIOMETER_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // if button pressed
  if (digitalRead(SWITCH_PIN) == 1) {
    endPulse();
    setLed(NUM_LEDS -1, 1);
    updateShiftRegister();
  } else {
    // if light resistor surpassed the threshold
    if (analogRead(RESISTOR_PIN) > threshold) {
      Serial.read();

      startPulse();
    }

    advanceAnimation();
  }

  delay(10);
}

void startPulse() {
  pulseSourceX = 0;

  lastMovement = millis();
  distanceFromPulse = 0;
}

void advanceAnimation() {
  if (pulseSourceX >= 0) {

    if (distanceFromPulse > floor(NUM_LEDS / LEDS_PER_COLUMN)) {
      endPulse();
    } else {
      advancePulse();
    }
  }

  updateShiftRegister();
}

void advancePulse() {
  for(int i = 0; i < NUM_LEDS; i++) {
    int x = floor(i / LEDS_PER_COLUMN);
    int y = i % LEDS_PER_COLUMN;

    int distance = x - pulseSourceX;

    if(x == distanceFromPulse)
      setLed(i, 1);
    else
      setLed(i, 0);
  }

  int speed = analogRead(SPEED_PENTIOMETER_PIN);

  if (millis() - lastMovement > speed) {
    distanceFromPulse++;
    lastMovement = millis();
  }
}

void endPulse() {
  distanceFromPulse = 0;
  pulseSourceX = -1;
  leds[0] = 0;
  leds[1] = 0;
}

void setLed(int led, int onOrOff) {
  int shifterIndex = led / LEDS_PER_SHIFTER;

  int ledToSet = led % LEDS_PER_SHIFTER;
  if (onOrOff == 1)
    bitSet(leds[shifterIndex], ledToSet);
  else
    bitClear(leds[shifterIndex], ledToSet);
}

void printBits(byte myByte){
 for(byte mask = 0x80; mask; mask >>= 1){
   if(mask  & myByte)
       Serial.print('1');
   else
       Serial.print('0');
 }

  Serial.println("");
}

// This method sends bits to the shift register:
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, leds[1]);
   shiftOut(dataPin, clockPin, MSBFIRST, leds[0]);
   digitalWrite(latchPin, HIGH);
}
