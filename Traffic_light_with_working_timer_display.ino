// Lab #1: Traffic Light Controller
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a controller for 2 traffic lights with 4 lights each: Red, Yellow, Green, Green-Left-Arrow.
// The R-GLA-G-Y pattern continues until the system is powered off, an active buzzer beeps for 3 seconds before a light is changed.

// 7 segment library
#include "SevSeg.h"

// Set up our display
SevSeg sevseg; 

// Number to display

// How long to delay
#define delayTime 1000
// constants won't change. They're used here to set pin numbers:
const int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
const int clockPin = 9;       // (9) SH_CP [SCK] on 74HC595
const int dataPin = 12;       // (12) DS [S1] on 74HC595
const int buzzer = 13;        // pin of the active buzzer
const int buttonPin = 10;  // the number of the pushbutton pin

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup()
{
  // Set the three shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //initialize the buzzer pin as an output
  pinMode(buzzer, OUTPUT);

  // My display has 4 digits
  byte numDigits = 2;
  
  // These are the arduino pins that switch
  // between these 4 digits
  byte digitPins[] = {42,45};
  
  // These refer to the segments/LEDs of 
  // each digit
  byte segmentPins[] = {43,47,51,49,48,44,52};
  
  // Strictly speaking should be using 330ohm resistors 
  bool resistorsOnSegments = false;
  
  // Your display will either be common anode or cathode
  byte hardwareConfig = COMMON_CATHODE;
  
  // Start the display
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  
  // Set brightness between 0 and 100
  sevseg.setBrightness(30);
  
  // Blank the display
  sevseg.blank();
}

void turnOnLED(int pinOnLED)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, pinOnLED); // specifying which LED will turn on by specifying the register
  digitalWrite(latchPin, HIGH); // turning on the specified LED
}

void blinkLED(int pinBlinkLED, int pinBlinkRedLED, int counterBlink){
  unsigned long startTime;
  unsigned long endTime;
  int loopcount;
  startTime = millis();
  endTime = startTime;
  while ((endTime - startTime) < 3000)
  { // loops for 3 seconds
    timerCountdown(&counterBlink);
    if ((millis() - startTime) % 1000 < 500) 
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, pinBlinkLED); // Turn on the LED
      digitalWrite(latchPin, HIGH);
    } 
    else 
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, pinBlinkRedLED); // Turn off the LED
      digitalWrite(latchPin, HIGH);
    }
    loopcount = loopcount+1;
    endTime = millis();
  }
}

// Function to be called when turning on LED
void timerDisplay(int pinLED, int pinRedLED, unsigned long lightTime) 
{ // led will be on for a set lightTime specified in the void loop
  unsigned long startTime = millis(); // Record the start time
  int counter = lightTime / 1000; // timer time
    while ((millis() - startTime) < (lightTime - 3000)) 
  { // led is fully on for the lightTime minus 3 seconds which will cover the blinking
    timerCountdown(&counter); // calling the function to display the timer time on the 7 segment display
    turnOnLED(pinLED);
  }
  counter = 3; // Timer value in seconds
  while ((millis() - startTime) < lightTime) 
  { // led blinks for the last 3 seconds
    // Display the timer on the 7 segment display
    timerCountdown(&counter);
    // Toggle LED based on a 0.5-second interval
    if ((millis() - startTime) % 1000 < 500) 
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, pinLED); // Turn on the LED
      digitalWrite(latchPin, HIGH);
    } 
    else
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, pinRedLED); // Turn off the LED
      digitalWrite(latchPin, HIGH);
    }
  }
}



void turnOnYellowLED(int pinYellowLED, int pinRedLED) // the pin parameter determines which LED will turn on
{
  int startTime = millis(); // Record the start time
  int counter = 3; // timer time
  while (millis() - startTime < 3000)
  { // led blinks for the last 3 seconds
     // Display the timer on the 7 segment display
    timerCountdown(&counter);
    // Toggle LED based on a 0.5-second interval
    if ((millis() - startTime) % 1000 < 500)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, pinYellowLED); // Turn on the LED
      digitalWrite(latchPin, HIGH);
    } 
    else
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, pinRedLED); // yellow LED doesn't blink while red LED does blink
      digitalWrite(latchPin, HIGH);
    }
  }
}

void timerCountdown(int *counter) 
{ // display the timer countdown on the 7 segment display
  static unsigned long timer = millis();
  if (millis() - timer >= delayTime) 
  {
    if (*counter % 2 == 0) 
    {
      sevseg.setNumber(*counter, 0);
    } 
      else 
      {
        sevseg.setNumber(*counter, -1);
      }
      if(*counter > 0 )
      {
        (*counter)--; // Decrement the value pointed to by counter
      }
      else
      {
        // Reset back to start
        counter = 0;
        return;
      }
    timer = millis();
  }
  sevseg.refreshDisplay();
}



// Traffic lights loop

void loop()
{
  // Both red lights are initially flashing until the button is pressed to initiate the R-GLA-G-Y pattern   
  buttonState = digitalRead(buttonPin); // read the state of the pushbutton value
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) // button default state at the start of the system (power up)
  {
    // both red lights flash (0.5 second on then 0.5 second off) until a button is pressed
    blinkLED(0b10000001, 0b00000000, 0);// blink yellow turn on red
  } 
  else if (buttonState == HIGH) // button has been pressed
  {
    do 
    {
      unsigned long startTime = millis(); // record the start time
      // First traffic light (TL1) GLA->yellow->green pattern begins while the other traffic light's red light sequence begins
      // TL1's GLA is on while keeping TL2's red light on
      timerDisplay(0b10001000, 0b10000000, 5000); // calling turnOnLED to turn on the GLA LED for 2 seconds and then blinking for 3 seconds
      // TL1's green is on while keeping TL2's red light on
      timerDisplay(0b10000100, 0b10000000, 12000); // calling turnOnLED to turn on the green LED for 9 seconds and then blinking for 3 seconds
      // TL1's yellow is on while keeping TL2's red light on which will flash for the last three seconds
      turnOnYellowLED(0b10000010, 0b00000010); // calling turnOnYellowLED to turn yellow LED for three seconds
  
      // TL2
      int counter;
      counter = 20;
      startTime = millis();
      while (millis() - startTime < 2000) { // 12 seconds for GLA full on
        timerCountdown(&counter);
        turnOnLED(0b00010001);
      }
      blinkLED(0b00010001, 0b00000001, 18); // blink GLA
      counter = 15;
      startTime = millis();
      while (millis() - startTime < 9000) { // 9 seconds for green full on
        turnOnLED(0b00100001);
        timerCountdown(&counter);
      }
      blinkLED(0b00100001, 0b00000001, 6); // blink green
      blinkLED(0b01000001, 0b01000000, 3);// blink yellow turn on red
    } while (buttonState == HIGH); // to keep repeating the pattern after the button has been pressed
  }
}

// Sources:
// Elegoo Super Starter Kit forUNO, Lesson 6 Active buzzer
// Elegoo Super Starter Kit forUNO, Lesson 16 Eight LED with 74HC595
// https://docs.arduino.cc/built-in-examples/digital/Button/
