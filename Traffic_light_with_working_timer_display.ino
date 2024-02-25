// Lab #1: Traffic Light Controller
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a controller for 2 traffic lights with 4 lights each: Red, Yellow, Green, Green-Left-Arrow.
// The R-GLA-G-Y pattern continues until the system is powered off, an active buzzer beeps for 3 seconds before a light is changed.

// 7 segment library
#include "SevSeg.h"

// Set up our 7 segment display according to SevSeg library
SevSeg sevseg; 

// How long to delay
#define delayTime 1000
// constants won't change. They're used here to set pin numbers:
const int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
const int clockPin = 9;       // (9) SH_CP [SCK] on 74HC595
const int dataPin = 12;       // (12) DS [S1] on 74HC595
const int buzzer = 13;        // pin of the active buzzer
const int buttonPin = 10;  // the number of the pushbutton pin

// variables that will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup()
{
  // Set the three shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //Initialize the buzzer pin as an output
  pinMode(buzzer, OUTPUT);

  // 2 digits of the display will be used
  byte numDigits = 2;
  
  // These are the arduino pins that switch between the 2 digits
  byte digitPins[] = {42,45};
  
  // These refer to the segments/LEDs of each digit
  byte segmentPins[] = {43,47,51,49,48,44,52};
  
  // The 5641AS 7 segment 4 digit display is a common cathode display
  byte hardwareConfig = COMMON_CATHODE;
  
  // Start the display
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins);
  
  // Set brightness between 0 and 100
  sevseg.setBrightness(30);
  
  // Blank the display
  sevseg.blank();
}

// Function to control LEDs
void controlLED(int mainLEDpin, int pinBlinkLED, int counterTime, int totalTime){
  unsigned long startTime = millis(); // Record the start time
  while (millis() - startTime < totalTime) { // totalTime determines how long the led will either be on or blink
    timerCountdown(&counterTime);
    if ((millis() - startTime) % 1000 < 500) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, mainLEDpin); // Turn on the LED
      digitalWrite(latchPin, HIGH);
    } else {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, pinBlinkLED); // Turn off the LED -->0b00000000
      digitalWrite(latchPin, HIGH);
    }
  }
}

// Display the timer countdown on the 7 segment display
void timerCountdown(int *counter) 
{ 
  static unsigned long timer = millis(); // Static variable to store the last time the function was called
  if (millis() - timer >= delayTime) // Check if the specified delay time has elapsed
  {
    if (*counter % 2 == 0) // If counter is even
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
      counter = 0; // displays 0
      return; // Reset back to start
    }
    timer = millis(); // Update the timer to the current time
  }
  sevseg.refreshDisplay();
} 

// Traffic lights main loop
void loop()
{  
  buttonState = digitalRead(buttonPin); // read the state of the pushbutton value
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  // Both red lights are initially flashing until the button is pressed to initiate the R-GLA-G-Y pattern for the traffic lights
  if (buttonState == LOW) // button default state at the start of the system (power up)
  {
    // both red lights flash (0.5 second on then 0.5 second off) until a button is pressed
    controlLED(0b10000001, 0b00000000, 0, 3000);// calling the blink LED function to blink the red lights
  } 
  else if (buttonState == HIGH) // button has been pressed
  {
    do // loop to repeat the R-GLA-G-Y pattern for the traffic lights after the button has been pressed
    {
      // First traffic light (TL1) GLA->yellow->green pattern begins while the other traffic light's red light sequence begins
      // TL1's GLA is on while keeping TL2's red light on for a total of 5 seconds
      controlLED(0b10001000, 0b10001000, 5, 2000); // // calling controlLED to turn on the GLA LED for 2 seconds and then blinking for 3 seconds
      digitalWrite(buzzer, HIGH);
      controlLED(0b10001000, 0b10000000, 3, 3000); // blink green
      digitalWrite(buzzer, LOW);
      // TL1's green is on while keeping TL2's red light on for a total of 12 seconds
      controlLED(0b10000100, 0b10000100, 12, 9000); // calling controlLED to turn on TL1's green light for 9 seconds
      digitalWrite(buzzer, HIGH);
      controlLED(0b10000100, 0b10000000, 3, 3000); // calling controlLED to blink TL1's green light for 3 seconds
      digitalWrite(buzzer, LOW);
      // TL1's yellow is on while flashing TL2's red light for a total of 3 seconds
      digitalWrite(buzzer, HIGH);
      controlLED(0b10000010, 0b00000010, 3, 3000); // calling controlLED to turn on TL1's yellow light and blink TL2's red light
      digitalWrite(buzzer, LOW);

      // Second traffic light (TL2) GLA->yellow->green pattern begins while TL1's red light sequence begins
      // TL2's GLA is on while keeping TL1's red light on
      controlLED(0b00010001, 0b00010001, 20, 2000); // // calling controlLED to turn on the GLA LED for 2 seconds and then blinking for 3 seconds
      digitalWrite(buzzer, HIGH);
      controlLED(0b00010001, 0b00000001, 18, 3000); // blink green
      digitalWrite(buzzer, LOW);
      // TL1's green is on while keeping TL2's red light on for a total of 12 seconds
      controlLED(0b00100001, 0b00100001, 15, 9000); // calling controlLED to turn on TL1's green light for 9 seconds
      digitalWrite(buzzer, HIGH);
      controlLED(0b00100001, 0b00000001, 6, 3000); // calling controlLED to blink TL1's green light for 3 seconds
      digitalWrite(buzzer, LOW);
      // TL1's yellow is on while flashing TL2's red light for a total of 3 seconds
      digitalWrite(buzzer, HIGH);
      controlLED(0b01000001, 0b01000000, 3, 3000); // calling controlLED to turn on TL1's yellow light and blink TL2's red light
      digitalWrite(buzzer, LOW);

    } while (buttonState == HIGH); // to keep repeating the pattern after the button has been pressed
  }
}

// Sources:
// Elegoo Super Starter Kit forUNO, Lesson 6 Active buzzer
// Elegoo Super Starter Kit forUNO, Lesson 16 Eight LED with 74HC595
// https://docs.arduino.cc/built-in-examples/digital/Button/
// https://forum.arduino.cc/t/six-digit-seven-segment-display-countdown-timer/1169656
// https://github.com/DeanIsMe/SevSeg
