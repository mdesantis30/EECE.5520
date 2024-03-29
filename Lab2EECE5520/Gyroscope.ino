// Lab #2: Game Control with Joy Stick, Gyro and Accelerometer
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a game controller for a “Snake” game using a a joystick and a gyro sensor (MPU-6050) to control the movement of the snake, 
// and a buzzer which beeps when an apple is eaten, when the accelerometer (MPU-6050) is "shaked" the points double for the next apple. 
// The game ends when the snake hits onto the boundary.
// This arduino file handles the gyroscope movements and accelerometer "shaking" detection.

#include<Wire.h>

const int MPU_addr=0x68;  // I2C address of the MPU-9250
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;

const int buzzer = 13; // pin of the active buzzer

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-9250)
  Wire.endTransmission(true);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT); //Initialize the buzzer pin as an output
}

void loop()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  
  // variables accessing readings for gyroscope function of the MPU-9250
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // variables accessing readings for accelerometer function of the MPU-9250
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  
  // The criteria set in the if statement below determines a baseline for the accelerometer data to determine when the sensor has been "shaken"
  if ((AcX >= 1000 && AcY >= 25000 && AcZ >= 25000) || (AcX >= 1000 && AcY <= -25000 && AcZ >= 25000))
  {
    Serial.println("Shake detected"); // Send a message over serial port to notify code in python to move turn apple gold and double points per apple
  }
  
  // This if-else-if interprets the data received from the gyroscope sensor to movements as defined in serial.print
  if (GyY >= 6000)
  {
    Serial.print("Moves down\n"); // Send a message over serial port to notify code in python to move snake down
  }
  else if (GyY <= -6000)
  {
    Serial.print("Moves up\n"); // Send a message over serial port to notify code in python to move snake up
  }
  else if (GyX <= -6000)
  {
    Serial.print("Moves right\n"); // Send a message over serial port to notify code in python to move snake right
  }
  else if (GyX >= 6000)
  {
    Serial.print("Moves left\n"); // Send a message over serial port to notify code in python to move snake left
  } 
  
  delay(100);
  
  // Check if Python sent a flag indicating an apple is eaten
  if (Serial.available() > 0)
  {
    char signal = Serial.read();
    if (signal == 'e') // if flag received
    {
      digitalWrite(buzzer, HIGH);// Turn on buzzer
      delay(200);
      digitalWrite(buzzer, LOW);// Turn off buzzer
    }
  }
}

// Sources:
// Source files for Lab 2, Professor Yan Luo, University of Massachusetts Lowell
// Elegoo Super Starter Kit forUNO, Lesson 16 GY-521 Module
