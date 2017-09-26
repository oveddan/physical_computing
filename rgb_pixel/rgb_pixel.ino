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
 
void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
}

int pulseSourceX = -1;
int pulseSourceY = -1;
int distanceFromPulse = 0;

void loop() {
  if (Serial.available() > 0) {
    Serial.read();
    Serial.println("pulsing");

    pulse(0, 1);
  }

  advanceAnimation();

  delay(100);
}

void pulse(int startX, int startY) {
  pulseSourceX = startX;
  pulseSourceY = startY;

  distanceFromPulse = 0;
}

void advanceAnimation() {
  if (pulseSourceX >= 0 && pulseSourceY >= 0) {
    //Serial.println(distanceFromPulse);
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

    float distance = sqrt(sq(x - pulseSourceX) + sq(y - pulseSourceY));

    //Serial.println(x);
    /* Serial.println(y); */
    /* Serial.println(ceil(distance == distanceFromPulse)); */

    /* if(floor(distance == distanceFromPulse)) */
    if(x == distanceFromPulse)
      setLed(i, 1);
    else
      setLed(i, 0);
  }
  distanceFromPulse++;
}

void endPulse() {
  distanceFromPulse = 0;
  pulseSourceX = -1;
  pulseSourceY = -1;
  leds[0] = 0;
  leds[1] = 0;
}

void setLed(int led, int onOrOff) {
  int shifter = led / LEDS_PER_SHIFTER;

  int ledToSet = led % LEDS_PER_SHIFTER;
  if (onOrOff == 1)
    bitSet(leds[shifter], ledToSet);
  else
    bitClear(leds[shifter], ledToSet);
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
