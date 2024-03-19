// Lab #2: Game Control with Joy Stick, Gyro and Accelerometer
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a game controller for a “Snake” game using a a joystick and a gyro sensor (MPU-6050) to control the movement of the snake, 
// and a buzzer which beeps when an apple is eaten, when the accelerometer (MPU-6050) is "shaked" the points double for the next apple. 
// The game ends when the snake hits onto the boundary.

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output, returns an integer from 0-1023
const int Y_pin = A1; // analog pin connected to Y output, returns an integer from 0-1023
const int buzzer = 13; // pin of the active buzzer

// variables for analog reading
int X_read;
int Y_read;

// Neutral readings to determine joystick calibration
int LR_high;
int LR_low;
int UD_high;
int UD_low;

void setup() {
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT); //Initialize the buzzer pin as an output

  // leaving a +/- 5 degree angle dead zone to determine the joystick center and take into account its lack of precision
  // Important: assumes joystick is left untouched when code is initializing
  LR_high = analogRead(X_pin) + 5;
  LR_low = analogRead(X_pin) - 5;
  UD_high = analogRead(Y_pin) + 5;
  UD_low = analogRead(Y_pin) - 5;
}

void loop() {
  X_read = analogRead(X_pin);
  Y_read = analogRead(Y_pin);
  
  if (Y_read >= UD_high)
  {
    Serial.print("Moves down\n");
  }
  else if (Y_read <= UD_low)
  {
    Serial.print("Moves up\n");
  }
  else if (X_read >= LR_high)
  {
    Serial.print("Moves right\n");
  }
  else if (X_read <= LR_low)
  {
    Serial.print("Moves left\n");
  }
  delay(100);

  // Check if Python sent a signal indicating an apple is eaten
  if (Serial.available() > 0) {
    char signal = Serial.read();
    if (signal == 'e') {
      digitalWrite(buzzer, HIGH);// Turn on buzzer
      delay(200);
      digitalWrite(buzzer, LOW);// Turn off buzzer
    }
  }
}

// Sources:
// Source files for Lab 2, Professor Yan Luo, University of Massachusetts Lowell
// Elegoo Super Starter Kit forUNO, Lesson 12 Analog Joystick
