// Lab #3: Sensors and Actuators -  Designing a robot car that can drive itself surrounding an object such as a trash can or
// a one-gallon milk bottle using a robot kit
// Matthew DeSantis and Ryan Mouhib, EECE.5520

#include <arduinoFFT.h>

// Ultrasonic sensor pins
const int trigPin = 13; // 10 microsecond high pulse causes chirp , wait 50 us
const int echoPin = 12; // Width of high pulse indicates distance

// Motor control pins: L298N bridge
const int enAPin = 6; // Left motor PWM speed control
const int in1Pin = 7; // Left motor Direction 1
const int in2Pin = 5; // Left motor Direction 2
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 2; // Right motor Direction 2
const int enBPin = 3; // Right motor PWM speed control

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
}

// Function to set motor direction and speed
// Motor A (speedA and dirA) corresponds to the right motor
// Motor B (speedB and dirB) correpsonds to the left motor
// "true" means forward and "false" means backwards
void setMotorSpeed(int speedA, int speedB, bool dirA, bool dirB) {
  // Set motor A direction
  digitalWrite(in1Pin, dirA ? HIGH : LOW);
  digitalWrite(in2Pin, !dirA ? HIGH : LOW);
  // Set motor B direction
  digitalWrite(in3Pin, dirB ? HIGH : LOW);
  digitalWrite(in4Pin, !dirB ? HIGH : LOW);
  // Set motor speed
  analogWrite(enAPin, speedA);
  analogWrite(enBPin, speedB);
}

// Function to read distance from the ultrasonic sensor , return distance in millimeters
unsigned int readDistance ()
{
  digitalWrite ( trigPin , HIGH );
  delayMicroseconds (10);
  digitalWrite ( trigPin , LOW );
  unsigned long period = pulseIn ( echoPin, HIGH ); // pulseIn returns time in microseconds (10ˆ−6)
  // 2d = p * 10ˆ−6 s * 343 m/s = p * 0.00343 m = p * 0.343 mm/us
  return period * 343 / 2000; // Speed of sound in dry air , 20C is 343 m/s
}
  
void loop()
{
  // Read distance from the ultrasonic sensor
  unsigned int distance = readDistance();
  
  // Define the desired distance from the object (trash can)
  unsigned int desiredDistance = 200; // 200mm
  
  // Define the tolerance for distance adjustment
  unsigned int tolerance = 10; // 10mm
  
  // If the measured distance is less than the desired distance minus the tolerance, turn slightly left
  if (distance < desiredDistance - tolerance)
  {
    // Turn slightly left
    setMotorSpeed(200, 200, true, false); // Adjust motor speed as needed
    delay(100); // Adjust delay as needed
  }
  // If the measured distance is greater than the desired distance plus the tolerance, turn slightly right
  else if (distance > desiredDistance + tolerance)
  {
    // Turn slightly right
    setMotorSpeed(200, 200, false, true); // Adjust motor speed as needed
    delay(100); // Adjust delay as needed
  }
  // If the measured distance is within the acceptable range, move forward
  else
  {
    // Move forward
    setMotorSpeed(200, 200, true, true); // Adjust motor speed as needed
    delay(100); // Adjust delay as needed
  }

  /*
  // Example: move forward at full speed
  setMotorSpeed(255, 255, true, false);
  delay(1000); // Wait for 1 second
  
  // Example: turn left at half speed
  setMotorSpeed(127, 127, false, true);
  delay(1000); // Wait for 1 second

  // Example: stop for 1 second
  setMotorSpeed(0, 0, true, true);
  delay(1000); // Wait for 1 second*/
}