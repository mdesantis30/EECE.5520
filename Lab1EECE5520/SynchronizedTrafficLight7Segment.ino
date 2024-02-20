// Lab #1: Traffic Light Controller
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a controller for 2 traffic lights with 4 lights each: Red, Yellow, Green, Green-Left-Arrow.
// The R-GLA-G-Y pattern continues until the system is powered off, an active buzzer beeps for 3 seconds before a light is changed.

int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
int clockPin = 9;       // (9) SH_CP [SCK] on 74HC595
int dataPin = 12;       // (12) DS [S1] on 74HC595
int buzzer = 13;        // pin of the active buzzer

void setup()
{
  // Set the three shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //initialize the buzzer pin as an output
  pinMode(buzzer, OUTPUT);
}

// Function to be called when turning on LED
void turnOnLED(int pinLED) // the pin parameter determines which LED will turn on
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, pinLED);
  digitalWrite(latchPin, HIGH);
}

// Function to be called in the traffic lights loop to blink LED and buzzer beep for 3 seconds before light change in the first traffic light (TL1),
// while the other traffic's light (TL2) red light remains on
void blinkLED1(int pinBlink1) // the pinBlink1 parameter determines which LED will blink
{
  int startTime;
  int endTime;
  int loopcount;
  startTime = millis();
  endTime = startTime;
  digitalWrite(buzzer, HIGH);
  while ((endTime - startTime) <= 3000) { // loops for 3 seconds
    digitalWrite(latchPin, LOW);  // flashing specified LED repeatedly by first turning it on for 0.5 seconds then off for 0.5 seconds
    shiftOut(dataPin, clockPin, MSBFIRST, pinBlink1); // LED is specified when calling blinkLED1 function in traffic lights loop
    digitalWrite(latchPin, HIGH); // LED on
    delay(500); // 0.5 second delay while LED is on
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b10000000); // cross traffic light's (TL2) red light remains on, while the specified LED blinks
    digitalWrite(latchPin, HIGH); // LED off
    delay(500); // 0.5 second delay while LED is off
    loopcount = loopcount+1;
    endTime = millis();
  }
  digitalWrite(buzzer, LOW);
}

// Function to be called in the traffic lights loop to blink LED and buzzer beep for 3 seconds before light change in the second traffic light (TL2),
// while the other traffic's light (TL1) red light remains on
void blinkLED2(int pinBlink2) // the pin parameter determines which LEDs will blink
{
  int startTime;
  int endTime;
  int loopcount;
  startTime = millis();
  endTime = startTime;
  digitalWrite(buzzer, HIGH);
  while ((endTime - startTime) <= 3000) { // loops for 3 seconds
    digitalWrite(latchPin, LOW);  // flashing specified LED repeatedly by first turning it on for 0.5 seconds then off for 0.5 seconds
    shiftOut(dataPin, clockPin, MSBFIRST, pinBlink2); // LED is specified when calling blinkLED1 function in traffic lights loop
    digitalWrite(latchPin, HIGH); // LED on
    delay(500); // 0.5 second delay while LED is on
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000001); // cross traffic light's (TL1) red light remains on, while the specified LED blinks
    digitalWrite(latchPin, HIGH); // LED off
    delay(500); // 0.5 second delay while LED is off
    loopcount = loopcount+1;
    endTime = millis();
  }
  digitalWrite(buzzer, LOW);
}

// Traffic lights loop
void loop()
{
  // First traffic light (TL1) GLA->yellow->green pattern begins while the other traffic light's red light sequence begins
  // TL1's GLA is on
  turnOnLED(0b10001000); // calling turnOnLED to turn on TL1's GLA while keeping TL2's red light on
  delay(2000); // TL1's GLA stays on full for 2 seconds before blinking for 3 seconds
  blinkLED1(0b10001000);
  // TL1's green is on
  turnOnLED(0b10000100); // calling turnOnLED to turn on TL1's green light while keeping TL2's red light on 
  delay(9000); TL1's green stays on full for 9 seconds before blinking for 3 seconds
  blinkLED1(0b10000100); // calling blinkLED1 to blink TL1's green while keeping TL2's red light on and buzzer beep for 3 seconds
  // TL1's yellow is on
  digitalWrite(buzzer, HIGH);
  turnOnLED(0b10000010); // calling turnOnLED to turn on TL1's yellow light while keeping TL2's red light on 
  delay(3000); TL1's yellow stays on full and buzzer beeps for 3 seconds
  digitalWrite(buzzer, LOW);

  // Second traffic light (TL2) GLA->yellow->green pattern begins while the other traffic light's red light sequence begins
  // TL2's GLA is on
  turnOnLED(0b00010001); // calling turnOnLED to turn on TL2's GLA while keeping TL1's red light on
  delay(2000); // TL2's GLA stays on full for 2 seconds before blinking for 3 seconds
  blinkLED2(0b00010001); // calling blinkLED1 to blink TL2's GLA while keeping TL1's red light on and buzzer beep for 3 seconds
  // TL2's green is on
  turnOnLED(0b00100001); // calling turnOnLED to turn on TL2's green light while keeping TL1's red light on
  delay(9000); TL2's green stays on full for 9 seconds before blinking for 3 seconds
  blinkLED2(0b00100001); // calling blinkLED1 to blink TL2's green while keeping TL1's red light on and buzzer beep for 3 seconds
  // TL2's yellow is on
  digitalWrite(buzzer, HIGH);
  turnOnLED(0b01000001); // calling turnOnLED to turn on TL2's yellow light while keeping TL1's red light on 
  delay(3000); TL2's yellow stays on full and buzzer beeps for 3 seconds
  digitalWrite(buzzer, LOW);
}

// Sources:
// Elegoo Super Starter Kit forUNO, Lesson 6 Active buzzer
// Elegoo Super Starter Kit forUNO, Lesson 16 Eight LED with 74HC595
