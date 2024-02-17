// EECE.5520 Microprocessors II
// Matthew DeSantis and Ryan Mouhib
// Lab 1 - Traffic Lights
// This program is designed to simulate the timing and execution of a traffic light system.
// For the graduate section, this program will execute faster light blinking, intersection traffic lights, and a 7 segment display for the timer.

//Sources used
// https://projecthub.arduino.cc/dmytrosavchuk/adjustable-countdown-timer-382ea8
// https://docs.arduino.cc/built-in-examples/digital/Debounce
// https://www.ardumotive.com/how-to-use-a-buzzer-en.html
// https://projecthub.arduino.cc/SAnwandter1/programming-4-digit-7-segment-led-display-5c4617
// https://projecthub.arduino.cc/FalcomDigital/2-way-intersection-with-pedestrian-walk-cycle-ad2ef0
// Elegoo BASIC STARTERKIT TUTORIAL FOR MEGA 2560 tutorials and lessons PDF document

#include <math.h>

//timer setup for timer0, timer1, and timer2.
//this code will enable one arduino timer interrupt
//timer1 will interrupt at 1Hz

//storage variables
boolean toggle1 = 0;
//digital pin and constants for button debounce
const int buttonPin = 10;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

//these variables will change
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin
int iLEDState = HIGH;

//variable setup for first set of traffic lights
const int greenLED1Pin = 41;
const int yellowLED2Pin = 39;
const int redLED3Pin = 37;

//variable setup for second set of traggic lights
const int greenLED4Pin = 43;
const int yellowLED5Pin = 45;
const int redLED6Pin = 47;
//nearest LED to the buzzer is pin 47, farthest from the buzzer is pin 41
const int buzzer = 13;

//countdown timer and 7-segment display setups
int digit_pin[] = {6, 9, 10, 11}; // PWM Display  digit pins from left to right

int speakerPin = 15;

#define DIGIT_ON  LOW
#define DIGIT_OFF  HIGH

int segA = 2; 
int segB = 3; 
int  segC = 4; 
int segD = 5; 
int segE = A0; //pin 6 is used bij display 1 for  its pwm function
int segF = 7; 
int segG = 8;  

int button1=13;
int button2=12;
int button3=16;
int button4=17;

int  countdown_time = 60;

struct struct_digits {
    int digit[4];
};

void setup(){
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 13 as an output
  pinMode(buttonPin, INPUT);
  //button mode should only work for the red lights
  pinMode(redLED3Pin, OUTPUT); //first red light
  pinMode(redLED6Pin, OUTPUT); //second red light

  pinMode(yellowLED2Pin, OUTPUT); //first yellow light
  pinMode(yellowLED5Pin, OUTPUT); //second yellow light

  pinMode(greenLED1Pin, OUTPUT); //first green light
  pinMode(greenLED4Pin, OUTPUT); //second green light
  
  digitalWrite(redLED3Pin, iLEDState);
  digitalWrite(redLED6Pin, iLEDState);

  //attachInterrupt(0, pin_ISR, CHANGE);  //  "Watches" in the background for a  button press
  /* Attach an interrupt to the ISR vector to monitor Push Button.
    Number 0 (for digital pin 2) or number 1 (for digital pin 3) are used.
    Interrupts  are useful for making things happen automatically in
    microcontroller programs,and  can help solve timing problems.
    Good tasks for using an interrupt may include  reading
    a rotary encoder, or monitoring user input */
  // Set Initial  state of all red LED to HIGH
  //digitalWrite (redLED3Pin, HIGH);
  //digitalWrite  (redLED6Pin, HIGH);

  //set pins as outputs
  //pinMode(13, OUTPUT);

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts

  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  for (int i=0; i<4;  i++) {
    pinMode(digit_pin[i], OUTPUT);
  }

  pinMode(speakerPin,  OUTPUT);

  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);
  pinMode(button4,INPUT_PULLUP);

}//end setup

// Timer1's interrupt service routing (ISR)
// The code in ISR will be executed every time timer1 interrupt occurs
// That is, the code will be called once every second
// TODO
//   you need to set a flag to trigger an evaluation of the conditions
//   in your state machine, then potentially transit to next state
//
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle1){
    digitalWrite(13,HIGH);
    toggle1 = 0;
  }
  else{
    digitalWrite(13,LOW);
    toggle1 = 1;
  }
}
  
void loop(){
  digitalWrite(redLED3Pin,HIGH); // turn the LED on (HIGH is the voltage level) 
  delay(500); // wait for a second
  digitalWrite(redLED3Pin, LOW); // turn the LED off by making the voltage LOW
  delay(500); // wait for a second   
  
  //read button state into variable "read1"
  int read1 = digitalRead(buttonPin);

  //check if button is pressed
  if (read1 != lastButtonState) {
    lastDebounceTime = millis();
  } // if switch state changes, reset debounce timer

  if ((millis() - lastDebounceTime) > debounceDelay) {
    //read at current state
    // if the button state has changed:
    if (read1 != buttonState) {
      buttonState = read1;
    }
    // only toggle the LED if the new button state is HIGH
    if (buttonState == HIGH) {
      iLEDState = !iLEDState;
    }
  }  

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = read1;

  // set the red LED to a constant on state for 15 seconds
  digitalWrite(redLED3Pin, iLEDState);
  digitalWrite(redLED6Pin, iLEDState);
  delay(150000);
  
  //Traffic light set 1
  //delay(150000);
  //digitalWrite(redLED3Pin, HIGH)
  digitalWrite(greenLED1Pin, HIGH);
  digitalWrite(redLED3Pin, LOW);
  delay(120000);
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(3000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
  //replace buzzer code with a function call

  digitalWrite(greenLED1Pin, LOW);
  digitalWrite(yellowLED2Pin, HIGH);
  delay(3000);
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(3000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec

  //Traffic light set 2
    //both red LEDs start on
    //then green LED in set 1 turns on, and red LED in set 2 stays off
    //when green LED in set 1 approaches yellow, turn on buzzer, remain in yellow for 3 seconds, then go to red.
    //add short delay to red LED in set 1, then turn on green LED in set 2 on
    //transition from green LED high in set 2 to yellow LED high in set 2
    //transition from yellow to red in set 2
    //repeat cycle


  //loop for program
  //if (toggle1) {
  //}
  //else {
  //}

}

//MAKE SURE TO CHANGE PINS FOR COUNTDOWN TIMER 
//MAKE SURE TO REMOVE BUZZER FOR THIS PART
//MAKE SURE TO BUILD THE HARDWARE FOR THIS PART

//everything written below is for the countdown timer, buzzer, and 7-segment display
void  playTone(int tone, int duration) {
  for (long k = 0; k < duration * 1000L; k  += tone * 2) {  
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
//REPLACE THE BUZZER WITH THIS FUNCTION, OR REMOVE THIS SPEAKER FUNCTION ENTIRELY

void  lightNumber(int numberToDisplay) {
  #define SEGMENT_ON  HIGH
  #define SEGMENT_OFF  LOW

  switch (numberToDisplay){

  case 0:
    digitalWrite(segA,  SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF,  SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case  1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA,  SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF,  SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case  3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE,  SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG,  SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD,  SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF,  SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case  5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE,  SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB,  SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG,  SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD,  SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF,  SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case  8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE,  SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB,  SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA,  SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC,  SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE,  SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG,  SEGMENT_OFF);
    break;  
  }

}

void SwitchDigit(int  digit) {
  for (int i=0; i<4; i++) {
    if (i == digit) {
      digitalWrite(digit_pin[i],  DIGIT_ON);
    } else {
      digitalWrite(digit_pin[i], DIGIT_OFF);
    }
  }
}

struct struct_digits IntToDigits(int n){
  struct struct_digits  dig;
  int zeros=0;
  int d;
  for (int i=0; i<4; i++) {
    d=n/pow(10,3-i);
    zeros += d;
    n = n - d*pow(10,3-i);
    if (zeros!=0 || i==3) {
      dig.digit[i]=d;
    } 
    else {
      dig.digit[i]=10;
    }
  }
  return dig;
}

void PrintNumber(int n, int time) {
  struct struct_digits  dig;

  dig = IntToDigits(n);

  for (int i=0; i<= time/20; i++) {
    if (digitalRead(button2)==LOW) {
      return;
    }
    for (int j=0;  j<4; j++) {
      SwitchDigit(j);
      lightNumber(dig.digit[j]);
      delay(5);
    }
  }
}

bool Countdown(int n, int del){
  for (int q=n;  q>0; q--){
    PrintNumber(q,del);
    if (digitalRead(button2)==LOW) {
      return false;
    }
  }
  PrintNumber(0,0);
  playTone(1519,1000);
  return true;
}

void reset() {
  int m, zeros, d, pressed3  = 0, pressed4 = 0;
  m=countdown_time;
  struct struct_digits dig;

  dig = IntToDigits(countdown_time);

  while (digitalRead(button1)==HIGH)  {
    for (int j=0; j<4; j++) {
      SwitchDigit(j);
      lightNumber(dig.digit[j]);
      delay(5);
    }
    if (digitalRead(button3)==LOW) { 
      if (pressed3  == 0 || pressed3 > 30) {
        if (countdown_time > 0) {
          countdown_time  -= 1 ;
        }
        dig = IntToDigits(countdown_time);
      } 
      pressed3 += 1;
    }
    else if (digitalRead(button4)==LOW) { 
      if (pressed4 == 0 || pressed4 > 30) {
        if (countdown_time <9999)  {
          countdown_time += 1 ;
        }
        dig = IntToDigits(countdown_time);
      } 
      pressed4 += 1;
    }
    if (digitalRead(button3)==HIGH)  {
      pressed3=0;
    }
    if (digitalRead(button4)==HIGH) {
      pressed4=0;
    }
  }
}