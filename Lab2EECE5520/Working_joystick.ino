// Lab #2: Game Control with Joy Stick, Gyro and Accelerometer
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a game controller for a “Snake” game using a a joystick and a gyro sensor (MPU-6050) to control the movement of the snake, 
// and a buzzer which beeps when an apple is eaten, when the accelerometer (MPU-6050) is "shaked" the points double for the next apple. 
// The game ends when the snake hits onto the boundary.

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output, returns an integer from 0-1023
const int Y_pin = A1; // analog pin connected to Y output, returns an integer from 0-1023

// variables for analog reading
int LR;
int UD;

// Neutral reading for joystick calibration

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
}

void loop() {
  LR = analogRead(X_pin);
  UD = analogRead(Y_pin);
  if (UD >= 516)
  {
    Serial.print("Moves down\n"); // Translate: move snake down
  }
  else if (UD <= 500)
  {
    Serial.print("Moves up\n"); // To be translated in python: move snake up
  }
  else if (LR >= 516)
  {
    Serial.print("Moves right\n"); // To be translated in python: move snake right
  }
  else if (LR <= 500)
  {
    Serial.print("Moves left\n"); // To be translated in python: move snake left
  }
  else
  {
    Serial.print("Joystick is centered\n"); // To be translated in python: stop moving snake
  }
  delay(250);
}


// Sources:
// Source files for Lab 2, Professor Yan Luo, University of Massachusetts Lowell
// Elegoo Super Starter Kit forUNO, Lesson 12 Analog Joystick
