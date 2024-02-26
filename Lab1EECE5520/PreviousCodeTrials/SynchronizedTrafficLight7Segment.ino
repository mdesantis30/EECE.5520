// Lab #1: Traffic Light Controller
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a controller for 2 traffic lights with 4 lights each: Red, Yellow, Green, Green-Left-Arrow.
// The R-GLA-G-Y pattern continues until the system is powered off, an active buzzer beeps for 3 seconds before a light is changed.

#include <math.h>

#define DIGIT_ON LOW
#define DIGIT_OFF HIGH

// constants won't change. They're used here to set pin numbers:
const int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
const int clockPin = 13;       // (9) SH_CP [SCK] on 74HC595
const int dataPin = 12;       // (12) DS [S1] on 74HC595
const int buzzer = 10;        // pin of the active buzzer
const int buttonPin = 52;  // the number of the pushbutton pin
// countdown timer modification buttons (start, reset, add, subtract)
const int cdButton1 = 47;
const int cdButton2 = 49;
const int cdButton3 = 51;
const int cdButton4 = 53;  // would need to add code for adding and subtracting from the countdown timer

// 7 segment pin declarations - corresponds to the pins of the 7 segment device 
const int pinA = 33;
const int pinB = 41;
const int pinC = 6;
const int pinD = 8;
const int pinE = A0;
const int pinF = 35;
const int pinG = 5;
// pin numbers of each digit
const int digit1 = 31;
const int digit2 = 37;
const int digit3 = 39;
const int digit4 = 4;
// array with each digit pin assigned to respective digit
int digitPins[] = {digit1, digit2, digit3, digit4};
int countdown = 12; // standard countdown of a green light in seconds
int countdownYellow = 3; // countdown of yellow light
int countdownRed = 20; // countdown of red light
int countdownGLA = 5; // countdown of green left arrow light

// variables will change:
int buttonState = LOW;  // variable for reading the pushbutton status

// structure containing 4 digits 
struct digitStruct {
  int digitNum[4];
};

void setup()
{
  // Set the three shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // initialize the buzzer pin as an output
  pinMode(buzzer, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // initialize the 7-segment display LEDs for each indidual digit
  pinMode(pinA, OUTPUT); 
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);

  /* 

      A
   ---
 F|   | B
  | G |
   ---
 E|   | C
  |   |
   ---
    D
  
  */

  // declare each whole digit as an output
  for(int i = 0; i < 4; i++) 
  {
    pinMode(digitPins[i], OUTPUT);
  }

  pinMode(cdButton1,INPUT_PULLUP);
  pinMode(cdButton2,INPUT_PULLUP);
  pinMode(cdButton3,INPUT_PULLUP);
  pinMode(cdButton4,INPUT_PULLUP);

}

// Function to be called when turning on LED
void turnOnLED(int pinLED) // the pin parameter determines which LED will turn on
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, pinLED); // 0b00010001-> other GLA
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
  //digitalWrite(buzzer, HIGH);
  while((endTime - startTime) <= 3000)
  { // loops for 3 seconds
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
  while((endTime - startTime) <= 3000)
  { // loops for 3 seconds
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

// the following switch statement cycles through all the cases in which 
// the 7 segment display LEDs can turn on
// different LED patterns create different numbers, and these are all accounted for
void dispNum(int displayedNumber) {

  #define PIN_ON HIGH
  #define PIN_OFF LOW
  // PIN_ON and PIN_OFF represent the individual LEDs of each digit

  switch(displayedNumber) {
    case 0:
      digitalWrite(pinA, PIN_ON);
      digitalWrite(pinB, PIN_ON);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_ON);
      digitalWrite(pinE, PIN_ON);
      digitalWrite(pinF, PIN_ON);
      digitalWrite(pinG, PIN_OFF);
      break;
      // creates the number 0

    case  1:
      digitalWrite(pinA, PIN_OFF);
      digitalWrite(pinB, PIN_ON);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_OFF);
      digitalWrite(pinE, PIN_OFF);
      digitalWrite(pinF, PIN_OFF);
      digitalWrite(pinG, PIN_OFF);
      break;
      // creates the number 1

    case 2:
      digitalWrite(pinA, PIN_ON);
      digitalWrite(pinB, PIN_ON);
      digitalWrite(pinC, PIN_OFF);
      digitalWrite(pinD, PIN_ON);
      digitalWrite(pinE, PIN_ON);
      digitalWrite(pinF, PIN_OFF);
      digitalWrite(pinG, PIN_ON);
      break;
      // creates the number 2

    case  3:
      digitalWrite(pinA, PIN_ON);
      digitalWrite(pinB, PIN_ON);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_ON);
      digitalWrite(pinE, PIN_OFF);
      digitalWrite(pinF, PIN_OFF);
      digitalWrite(pinG, PIN_ON);
      break;
      // creates the number 3

    case 4:
      digitalWrite(pinA, PIN_OFF);
      digitalWrite(pinB, PIN_ON);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_OFF);
      digitalWrite(pinE, PIN_OFF);
      digitalWrite(pinF, PIN_ON);
      digitalWrite(pinG, PIN_ON);
      break;
      // creates the number 4

    case  5:
      digitalWrite(pinA, PIN_ON);
      digitalWrite(pinB, PIN_OFF);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_ON);
      digitalWrite(pinE, PIN_OFF);
      digitalWrite(pinF, PIN_ON);
      digitalWrite(pinG, PIN_ON);
      break;
      // creates the number 5

    case 6:
      digitalWrite(pinA, PIN_ON);
      digitalWrite(pinB, PIN_OFF);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_ON);
      digitalWrite(pinE, PIN_ON);
      digitalWrite(pinF, PIN_ON);
      digitalWrite(pinG, PIN_ON);
      break;
      // creates the number 6

    case 7:
      digitalWrite(pinA, PIN_ON);
      digitalWrite(pinB, PIN_ON);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_OFF);
      digitalWrite(pinE, PIN_OFF);
      digitalWrite(pinF, PIN_OFF);
      digitalWrite(pinG, PIN_OFF);
      break;
      // creates the number 7

    case  8:
      digitalWrite(pinA, PIN_ON);
      digitalWrite(pinB, PIN_ON);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_ON);
      digitalWrite(pinE,  PIN_ON);
      digitalWrite(pinF, PIN_ON);
      digitalWrite(pinG, PIN_ON);
      break;
      // creates the number 8

    case 9:
      digitalWrite(pinA, PIN_ON);
      digitalWrite(pinB,  PIN_ON);
      digitalWrite(pinC, PIN_ON);
      digitalWrite(pinD, PIN_ON);
      digitalWrite(pinE, PIN_OFF);
      digitalWrite(pinF, PIN_ON);
      digitalWrite(pinG, PIN_ON);
      break;
      // creates the number 9

    case 10:
      digitalWrite(pinA,  PIN_OFF);
      digitalWrite(pinB, PIN_OFF);
      digitalWrite(pinC,  PIN_OFF);
      digitalWrite(pinD, PIN_OFF);
      digitalWrite(pinE,  PIN_OFF);
      digitalWrite(pinF, PIN_OFF);
      digitalWrite(pinG,  PIN_OFF);
      break;  
      // does not display any number
    }
}

// function to cycle through an entire digit (of 4 digits)
// switchDigit function relies on the switch cases of individual LEDs turning on in order to establish that whole digit as being "ON" or "OFF"
void switchDigit(int digitNum) {
  for(int i = 0; i < 4; i++) {
    if(i == digitNum) 
    {
      digitalWrite(digitPins[i], DIGIT_ON);
    } 
    else 
    {
      digitalWrite(digitPins[i], DIGIT_OFF);
    }
  }
}

// for digitStruct: declare a structure type intDigConversion and pass "n" to it
// integer to digit conversion
struct digitStruct intDigConversion(int n) {
struct digitStruct dig; // declare structure type "dig"
  int zeros = 0;
  int d;
  for(int i = 0; i < 4; i++) { // cycle through the number of digits
    d = n / pow(10, 3 - i);
    zeros = zeros + d;
    n = n - d * pow(10, 3 - i);
    if(zeros != 0 || i == 3) 
    {
      dig.digitNum[i] = d;
    } 
    else 
    {
      dig.digitNum[i] = 10;
    }
  }
  return dig;
}

void printNum(int n, int time) {
  struct digitStruct dig;
  // structure object of type digitStruct "dig" representing specific digit
  dig = intDigConversion(n);
  
  for(int i = 0; i <= time / 20; i++) {
    if(digitalRead(cdButton2) == LOW) 
    {
      return; 
    }
    for(int j = 0; j < 4; j++) 
    {
      switchDigit(j);
      dispNum(dig.digitNum[j]);
      // display the digit that the function switches on
      delay(5);
    }
  }
}

// executes the countdown timer for the 7 segment display
bool countdownFunct(int n, int del) { // pass loop iteration reference and delay variables
  for(int q = n; q > 0; q--) 
  {
    printNum(q, delay);
    if(digitalRead(cdButton2) == LOW) 
    {
      return false;
    }
  // if new iteration variable "q" is equal to "n" (passed from above):
  // repeatedly print the number that is remaining on the countdown timer
  // if not button is pressed do not execute the countdown function
  }
  printNum(0,0);
  return true;
}

void reset() {
  // initialize button press variables as having LOW input
  // initialize all other variables 
  int m = countdown;
  int zeros;
  int d;
  struct digitStruct dig;
  dig = intDigConversion(countdown);
  
  // while the first button is pressed (AKA the countdown timer started) set the countdown timer
  while(digitalRead(cdButton1) == LOW)  {   // changing this to LOW from HIGH will automatically start the countdown timer
    for(int j = 0; j < 4; j++) {
      switchDigit(j);
      dispNum(dig.digitNum[j]);
      delay(5);
      // the first loop reads the countdown timer value onto the display
    }
  }
}

void trafficBuzzer() {
  tone(buzzer, 1000); // 1000Hz signal
  delay(100);        // 1/10 second
  noTone(buzzer);     // Stop sound
  delay(100);        // for 1/10 second
}

// TRAFFIC LIGHT LOOP
void loop()
{  
  // Both red lights are initially flashing until the button is pressed to initiate the R-GLA-G-Y pattern   
  buttonState = digitalRead(buttonPin); // read the state of the pushbutton value
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) // button default state at the start of the system (power up)
  {
    // both red lights flash (0.5 second on then 0.5 second off) until a button is pressed
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00100010);
    digitalWrite(latchPin, HIGH); // LED on
    delay(500); // 0.5 second delay while LED is on
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    digitalWrite(latchPin, HIGH); // LED off
    delay(500); // 0.5 second delay while LED is off
  } 
  if (buttonState == HIGH) // button has been pressed
  {
    //do 
    //{
      /*7h595 IC notes
        * right red LED is QF => 00000100
        * right yellow LED is QA => 10000000
        * right bottom green LED is QG => 00000010
        * right top green LED is QH => 00000001 (GLA)
        * left red LED is QB => 010000000 
        * left yellow LED is QC => 00100000
        * left bottom green LED is QD => 00010000
        * left top green LED is QE => 00001000 (GLA)
      */
      
      // start traffic cycle with constant red lights
      turnOnLED(0b00100010); // keep lights red for the allowed time
      countdownFunct(countdownRed, 1000);
      delay(2000);
      //reset();
      // First traffic light (TL1) GLA->yellow->green pattern begins while the other traffic light's red light sequence begins
      // TL1's GLA is on
      // start the countdowns AFTER the LEDs turn on
      trafficBuzzer();
      turnOnLED(0b10000010); // calling turnOnLED to turn on TL1's GLA while keeping TL2's red light on
      countdownFunct(countdownGLA, 1000);
      delay(2000); // TL1's GLA stays on full for 2 seconds before blinking for 3 seconds
      countdownFunct(countdownYellow, 1000);
      blinkLED1(0b10000001);
      trafficBuzzer();
      
      // TL1's green is on
      turnOnLED(0b01000010); // calling turnOnLED to turn on TL1's green light while keeping TL2's red light on 
      countdownFunct(countdown, 1000);
      delay(9000); // TL1's green stays on full for 9 seconds before blinking for 3 seconds
      blinkLED1(0b10000100); // calling blinkLED1 to blink TL1's green while keeping TL2's red light on and buzzer beep for 3 seconds
      trafficBuzzer();
      
      // TL1's yellow is on
      turnOnLED(0b11000000); // calling turnOnLED to turn on TL1's yellow light while keeping TL2's red light on 
      countdownFunct(countdownYellow, 1000);
      delay(3000); // TL1's yellow stays on full and buzzer beeps for 3 seconds
      trafficBuzzer();
      
      // Second traffic light (TL2) GLA->yellow->green pattern begins while the other traffic light's red light sequence begins
      // TL2's GLA is on
      turnOnLED(0b00001100); // calling turnOnLED to turn on TL2's GLA while keeping TL1's red light on
      countdownFunct(countdownGLA, 1000);
      delay(2000); // TL2's GLA stays on full for 2 seconds before blinking for 3 seconds
      blinkLED2(0b00001000); // calling blinkLED1 to blink TL2's GLA while keeping TL1's red light on and buzzer beep for 3 seconds
      trafficBuzzer();

      // TL2's green is on
      turnOnLED(0b00010100); // calling turnOnLED to turn on TL2's green light while keeping TL1's red light on
      countdownFunct(countdown, 1000); 
      delay(9000); // TL2's green stays on full for 9 seconds before blinking for 3 seconds
      blinkLED2(0b00010000); // calling blinkLED1 to blink TL2's green while keeping TL1's red light on and buzzer beep for 3 seconds
      trafficBuzzer();

      // TL2's yellow is on
      turnOnLED(0b00100100); // calling turnOnLED to turn on TL2's yellow light while keeping TL1's red light on 
      countdownFunct(countdownYellow, 1000);
      delay(3000); // TL2's yellow stays on full and buzzer beeps for 3 seconds
      trafficBuzzer();

    //} while (buttonState == HIGH); // to keep repeating the pattern after the button has been pressed
  }
}
// Sources:
// Elegoo Super Starter Kit forUNO, Lesson 6 Active buzzer
// Elegoo Super Starter Kit forUNO, Lesson 16 Eight LED with 74HC595
// https://docs.arduino.cc/built-in-examples/digital/Button/
// https://projecthub.arduino.cc/dmytrosavchuk/adjustable-countdown-timer-382ea8 
