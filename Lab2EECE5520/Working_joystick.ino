// Lab #2: Game Control with Joy Stick, Gyro and Accelerometer
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a game controller for a “Snake” game using a a joystick and a gyro sensor (MPU-6050) to control the movement of the snake, 
// and a buzzer which beeps when an apple is eaten, when the accelerometer (MPU-6050) is "shaked" the points double for the next apple. 
// The game ends when the snake hits onto the boundary.

#include<Wire.h>

const int MPU_addr=0x68;  // I2C address of the MPU-9250
int16_t AcX,AcY,AcZ;

// Arduino pin numbers
const int X_pin = A0; // analog pin connected to X output, returns an integer from 0-1023
const int Y_pin = A1; // analog pin connected to Y output, returns an integer from 0-1023
const int buzzer = 13; // pin of the active buzzer

// variables for analog reading
int X_read; // x-axis
int Y_read; // y-axis

// Neutral readings to determine joystick calibration
int LR_high;
int LR_low;
int UD_high;
int UD_low;

void setup() {
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT); //Initialize the buzzer pin as an output

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-9250)
  Wire.endTransmission(true);

  // leaving a +/- 5 degree angle dead zone to determine the joystick center and take into account its lack of precision
  // Important: assumes joystick is left untouched when code is initializing
  LR_high = analogRead(X_pin) + 5;
  LR_low = analogRead(X_pin) - 5;
  UD_high = analogRead(Y_pin) + 5;
  UD_low = analogRead(Y_pin) - 5;
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 4 registers

  //variable accessing readings for accelerometer function of the MPU-9250
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  X_read = analogRead(X_pin);
  Y_read = analogRead(Y_pin);
  
  if (AcZ <= 11000)
  {
    Serial.println("Shake detected"); // Send a message over serial port to notify code in python that sensor has been shaken
  }

  if (Y_read >= UD_high)
  {
    Serial.print("Moves down\n"); // Send a message over serial port to notify code in python that joystick moves down
  }
  else if (Y_read <= UD_low)
  {
    Serial.print("Moves up\n"); // Send a message over serial port to notify code in python that joystick moves up
  }
  else if (X_read >= LR_high)
  {
    Serial.print("Moves right\n"); // Send a message over serial port to notify code in python that joystick moves rifht
  }
  else if (X_read <= LR_low)
  {
    Serial.print("Moves left\n"); // Send a message over serial port to notify code in python that joystick moves left
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
