// Lab #1: Traffic Light Controller
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a controller for 2 traffic lights with 4 lights each: Red, Yellow, Green, Green-Left-Arrow.
// The R-GLA-G-Y pattern continues until the system is powered off, an active buzzer beeps for 3 seconds before a light is changed.

#define digit1   5
#define digit2   4 
#define digit3   3
#define digit4   2
#define button   A0

int count = 0;
byte currentDigit; 
void disp(byte number, bool decimalPoint = 0);

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

  //initialize the 4 digits of the 7-segment display as separate outputs 
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  //initialize the push button as an input to the pullup resistor
  pinMode(button, INPUT_PULLUP);

  disp_off();  // turn off the display by calling the display off function

  // Timer1 module overflow interrupt configuration for periodic display refreshing
  TCCR1A = 0;
  TCCR1B = 1;  // enable Timer1 with prescaler = 1 ( 16 ticks each 1 µs)
  TCNT1  = 0;  // set Timer1 preload value to 0 (reset)
  TIMSK1 = 1;  // enable Timer1 overflow interrupt
}

void disp_off() 
{
   digitalWrite(digit1, HIGH);
   digitalWrite(digit2, HIGH);
   digitalWrite(digit3, HIGH);
   digitalWrite(digit4, HIGH);
}

//ISR funtion to be called when the timer interrupts the microcontroller
ISR(TIMER1_OVF_vect)   // Timer1 interrupt service routine (ISR)
{
  disp_off();  // turn off the display to start from scratch
 
  // switch cases in order to prepare which digits to turn on of the 4 digit display
  // for this lab, only 2 digits should need to be turned on at once at most
  switch (currentDigit)
  {
    case 1:
      disp(count / 1000);   // display digit 1 (most left)
      digitalWrite(digit1, LOW);  // turn on first digit
      break;
 
    case 2:
      disp((count / 100) % 10);   // display digit 2
      digitalWrite(digit2, LOW);     // turn on second digit
      break;
 
    case 3:
      disp((count / 10) % 10);   // display digit 3
      digitalWrite(digit3, LOW);    // turn on third digit
      break;
 
    case 4:
      disp(count % 10);   // display digit 4 (most right)
      digitalWrite(digit4, LOW);  // turn on fourth digit
  }
 
  currentDigit = (currentDigit % 4) + 1;
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
  turnOnLED(0b10001000);
  delay(2000);
  blinkLED1(0b10001000);
  // TL1's green is on
  turnOnLED(0b10000100);
  delay(9000);
  blinkLED1(0b10000100); // calling function to blink green LEDs and buzzer beep for 3 seconds
  // TL1's yellow is on
  digitalWrite(buzzer, HIGH);
  turnOnLED(0b10000010);
  delay(3000);
  digitalWrite(buzzer, LOW);

  // Second traffic light (TL2) GLA->yellow->green pattern begins while the other traffic light's red light sequence begins
  // TL2's GLA is on
  turnOnLED(0b00010001);
  delay(2000);
  blinkLED2(0b00010001);
  // TL2's green is on
  turnOnLED(0b00100001);
  delay(9000);
  blinkLED2(0b00100001); // calling function to blink green LEDs and buzzer beep for 3 seconds
  // TL2's yellow is on
  digitalWrite(buzzer, HIGH);
  turnOnLED(0b01000001);
  delay(3000);
  digitalWrite(buzzer, LOW);

  // 7-segment display 
  if(digitalRead(button) == 0)
  {
    count++;  // increment count by 1
    if(count > 9999) // absolute limit for count to wait for button reading
    {
      count = 0;
    }
    delay(200);  // wait 200 milliseconds
  }
}

void disp(byte number, bool decimalPoint)
{
// the following switch cases correspond to all the different combinations of digits that can be formed
// each digit is synonymous to a hexadecimal value
// shiftOut will send serial data of the hexadecimal value to the data and clock pins 
  switch (number)
  {
    case 0:  // print 0
      shiftOut(dataPin, clockPin, MSBFIRST, 0x02 | !decimalPoint);
      // shift the most significant bit by corresponding hexadecimal value
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
 
    case 1:  // print 1
      shiftOut(dataPin, clockPin, MSBFIRST, 0x9E | !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
 
    case 2:  // print 2
      shiftOut(dataPin, clockPin, MSBFIRST, 0x24 | !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
 
    case 3:  // print 3
      shiftOut(dataPin, clockPin, MSBFIRST, 0x0C | !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
 
    case 4:  // print 4
      shiftOut(dataPin, clockPin, MSBFIRST, 0x98 | !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
 
    case 5:  // print 5
      shiftOut(dataPin, clockPin, MSBFIRST, 0x48 | !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
 
    case 6:  // print 6
      shiftOut(dataPin, clockPin, MSBFIRST, 0x40 | !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
    
    case 7:  // print 7
      shiftOut(dataPin, clockPin, MSBFIRST, 0x1E | !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
 
    case 8:  // print 8
      shiftOut(dataPin, clockPin, MSBFIRST, !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      break;
 
    case 9:  // print 9
      shiftOut(dataPin, clockPin, MSBFIRST, 0x08 | !decimalPoint);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
  }
}
 


// Sources:
// Elegoo Super Starter Kit forUNO, Lesson 6 Active buzzer
// Elegoo Super Starter Kit forUNO, Lesson 16 Eight LED with 74HC595
// 7-Segment display with 74HC595 shift register | Arduino Projects 
    // Link: https://simple-circuit.com/arduino-7-segment-74hc595-shift-register/
