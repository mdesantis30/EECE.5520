// Lab #3: Sensors and Actuators -  Designing a robot car that can drive itself surrounding an object such as a trash can or
// a one-gallon milk bottle using a robot kit using a distance measurement function to determine the car’s distance from 
// the nearby object a and then control the movement of the car accordingly.
// Matthew DeSantis and Ryan Mouhib, EECE.5520

#include <arduinoFFT.h>

// Ultrasonic sensor pins
const int trigPin = 11;
const int echoPin = 12; // Width of high pulse indicates distance

// Motor control pins: L298N bridge
const int enAPin = 2; // Left motor PWM speed control
const int in1Pin = 3; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2
const int in3Pin = 6; // Right motor Direction 1
const int in4Pin = 7; // Right motor Direction 2
const int enBPin = 8; // Right motor PWM speed control

void setup()
{
  // Configuring input/output pins of ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite (trigPin, LOW);
  
  // Configuring motor output pins through the L298N
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);

  Serial.begin(9600); // starts the serial communication
}

// Function to set motor direction and speed
// Motor A (speedA and dirA) corresponds to the right motor
// Motor B (speedB and dirB) correpsonds to the left motor
// "true" means forward and "false" means backwards
// "duration" defines how long the motors will perform specified action
void setMotorControl(int speedA, int speedB, bool dirA, bool dirB, int duration) {
  // Set motor A (right motor) direction
  digitalWrite(in1Pin, dirA ? LOW : HIGH);
  digitalWrite(in2Pin, !dirA ? LOW : HIGH);
  // Set motor B (left motor) direction
  digitalWrite(in3Pin, dirB ?  LOW : HIGH);
  digitalWrite(in4Pin, !dirB ? LOW : HIGH);
  // Set motors speed
  analogWrite(enAPin, speedA);
  analogWrite(enBPin, speedB);
  delay(duration);
}

// Function to read distance from the ultrasonic sensor, returns distance in millimeters
unsigned int readDistance()
{
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  unsigned long period = pulseIn (echoPin, HIGH); // pulseIn returns time in microseconds (10ˆ−6)
  // 2d = p * 10ˆ−6 s * 343 m/s = p * 0.00343 m = p * 0.343 mm/us
  return period * 343 / 2000; // Speed of sound in dry air, 20C is 343 m/s
}
  
void loop()
{
  //********Notes********
  // Robot successfully circles object but sometimes turns on itself a few times before continuing circling object
  unsigned int distance = readDistance(); // read distance from the ultrasonic sensor
  int motorSpeedA = 100; // set speed of motor A
  int motorSpeedB = 100; // set speed of motor B
  
  if (distance < 80) // If the measured distance is less than the desired distance minus the tolerance, turn slightly left
  {
    setMotorControl(motorSpeedA, motorSpeedB, true, false, 300); // Turn slightly left to check distance and shape of object being circled
  }
  else if (distance > 110) // If the measured distance is greater than the desired distance plus the tolerance, turn slightly right
  {
    setMotorControl(motorSpeedA, motorSpeedB, false, true, 300); // Turn slightly right to continue circling object
  }
}
