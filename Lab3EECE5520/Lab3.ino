// Lab #3: Sensors and Actuators -  Designing a robot car that can drive itself surrounding an object such as a trash can or
// a one-gallon milk bottle using a robot kit using a distance measurement function to determine the car’s distance from 
// the nearby object a and then control the movement of the car accordingly.
// Matthew DeSantis and Ryan Mouhib, EECE.5520

#include <arduinoFFT.h>

// Ultrasonic sensor pins
const int trigPin = 13;
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
void setMotorControl(int speedA, int speedB, bool dirA, bool dirB) {
  // Set motor A direction
  digitalWrite(in1Pin, dirA ? HIGH : LOW);
  digitalWrite(in2Pin, !dirA ? HIGH : LOW);
  // Set motor B direction
  digitalWrite(in3Pin, dirB ?  LOW : HIGH);
  digitalWrite(in4Pin, !dirB ? LOW : HIGH);
  // Set motor speed
  analogWrite(enAPin, speedA);
  analogWrite(enBPin, speedB);
}

// Function to read distance from the ultrasonic sensor , return distance in millimeters
unsigned int readDistance()
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  unsigned long period = pulseIn (echoPin, HIGH); // pulseIn returns time in microseconds (10ˆ−6)
  // 2d = p * 10ˆ−6 s * 343 m/s = p * 0.00343 m = p * 0.343 mm/us
  return period * 343 / 2000; // Speed of sound in dry air , 20C is 343 m/s
}
  
void loop()
{
  unsigned int distance = readDistance(); // read distance from the ultrasonic sensor
  unsigned int desiredDistance = 100; // define desired distance from the object (trash can) to 200 mm
  int motorSpeedA = 150;
  int motorSpeedB = 150;

  if (distance < 100) // If the measured distance is less than the desired distance minus the tolerance, turn slightly left
  {
    // Turn slightly left to check distance and shape of object being circled
    setMotorControl(motorSpeedA, motorSpeedB, false, true);
    /*
    Serial.print("Car turning left.\n");
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print("\n");
    */
  }
  else if (distance > 150) // If the measured distance is greater than the desired distance plus the tolerance, turn slightly right
  {
    // Turn slightly right
    setMotorControl(motorSpeedA, motorSpeedB, true, false);
    /*
    Serial.print("Car turning right.\n");
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print("\n");
    */
  }
  else // If the measured distance is within the acceptable range, move forward
  {
    // Move forward
    setMotorControl(motorSpeedA, motorSpeedB, true, true);
    /*
    Serial.print("Car moving forward.\n");
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print("\n");
    */
  }
}
