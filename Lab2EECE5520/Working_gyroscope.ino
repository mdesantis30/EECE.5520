#include <Wire.h>

const int MPU_addr=0x68;  // I2C address of the MPU-9250
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;

const int buzzer = 13; // pin of the active buzzer

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-9250)
  Wire.endTransmission(true);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT); //Initialize the buzzer pin as an output
}

void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 4 registers

  // variables accessing readings for gyroscope function of the MPU-9250
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  // GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // variables accessing readings for accelerometer function of the MPU-9250
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  // AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  
  // Accelerometer
  // Serial.print("AcX = "); Serial.print(AcX); Serial.print("\n");
  // Serial.print("AcY = "); Serial.println(AcY); Serial.print("\n");
  // Serial.print("AcZ = "); Serial.print(AcZ); Serial.print("\n");

  if (GyY >= 5000)
  {
    Serial.print("Moves down\n");
    // Serial.print(GyY);
    // Serial.print("\n\n");
  }
  else if (GyY <= -5000)
  {
    Serial.print("Moves up\n");
    // Serial.print(GyY);
    // Serial.print("\n\n");
  }
  else if (GyX <= -5000)
  {
    Serial.print("Moves right\n");
    // Serial.print(GyX);
    // Serial.print("\n\n");
  }
  else if (GyX >= 5000)
  {
    Serial.print("Moves left\n");
    // Serial.print(GyX);
    // Serial.print("\n\n");
  }
  /*else 
  {
    Serial.print("Flat\n");
  }*/

  delay(100); // update every 0.1 seconds

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
