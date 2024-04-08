#include <Wire.h>
// Install new library
#include "arduinoFFT.h"

// Motor pins
#define motor1Pin1 5
#define motor1Pin2 9
#define motor2Pin1 4
#define motor2Pin2 8

// Motor PWM speed pins
#define motor1PWM 7
#define motor2PWM 6

// Ultrasonic sensor pins
#define trigPin 12
#define echoPin 11

// Analog pin for the sound sensor
#define soundSensorPin 3 

// Number of samples and sampling frequency for FFT
#define SAMPLES 128
#define SAMPLING_FREQUENCY 10000

// Microphone pin sound sensor
#define micPin A0

// Variables for sampling period and microseconds
unsigned int sampling_period_us;
unsigned long microseconds;

// Arrays to store real and imaginary parts of FFT
double vReal[SAMPLES];
double vImag[SAMPLES];

// Variable to store peak frequency
double peakFrequency;

// Predefined frequencies for C4 and A4 notes
const double C4_FREQUENCY = 262.0;
const double A4_FREQUENCY = 440.0;

// Error margin for frequency comparison
const double FREQUENCY_ERROR_MARGIN = 0.02;

// Create an arduinoFFT object
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

// Function prototypes
void controlCarMovement();
int measureDistance();
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft();
void turnRight();
void avoidObstacle();
void detectPeakFrequency();
void comparePeakFrequency();
bool isInRange(double frequency, double targetFrequency);

void setup() {
  // Set motor pins as output
  pinMode(motor1Pin1, OUTPUT); // HIGH -> spinning the motor forwards
  pinMode(motor1Pin2, OUTPUT); // LOW -> spinning the motor backwards
  pinMode(motor2Pin1, OUTPUT); 
  pinMode(motor2Pin2, OUTPUT); 
  
  // Set PWM pins for motor speed control
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
  
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Calculate sampling period in microseconds
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

}

void loop() {
  // Control car movement
  controlCarMovement();
  
  // Measure distance using ultrasonic sensor
  int distance = measureDistance();
  
  // If obstacle is detected, avoid it; otherwise, move forward
  if (distance < 20) {
    avoidObstacle();
  } else {
    // Move forward at a specified speed
    moveForward(255); // Full speed
  }
  
  // Detect peak frequency from sound sensor
  detectPeakFrequency();
  
  // Compare peak frequency with predefined frequencies
  comparePeakFrequency();
}

// Function to control car movement
void controlCarMovement() {
  moveForward(255); // Full speed
  delay(2000);
  moveBackward(128); // Half speed
  delay(2000);
  turnLeft();
  delay(1000);
  turnRight();
  delay(1000);
}

// Function to measure distance using ultrasonic sensor
int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  // Calculate distance in centimeters
  int distance = duration * 0.034 / 2;
  return distance;
}

// Functions to control car movement with speed parameter
void moveForward(int speed) {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(motor1PWM, speed); // Set motor 1 speed
  analogWrite(motor2PWM, speed); // Set motor 2 speed
}

void moveBackward(int speed) {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(motor1PWM, speed); // Set motor 1 speed
  analogWrite(motor2PWM, speed); // Set motor 2 speed
}

void turnLeft() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void turnRight() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

// Function to avoid obstacle
void avoidObstacle() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  turnRight();
  delay(1000);
  moveForward(200); // Reduced speed to navigate
  delay(2000);
}

// Function to detect peak frequency from sound sensor
void detectPeakFrequency() {
  // Collect samples from the sound sensor
  for (int i = 0; i < SAMPLES; i++) {
    microseconds = micros();
    vReal[i] = analogRead(micPin);
    vImag[i] = 0;
    while (micros() < (microseconds + sampling_period_us)) {
    }
  }

  // Apply windowing and compute FFT
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFT.compute(FFTDirection::Forward);

  // Get the major peak frequency
  peakFrequency = FFT.majorPeak();

  // Print the peak frequency in Hz
  Serial.print("Peak Frequency: ");
  Serial.print(peakFrequency);
  Serial.println(" Hz");
}

// Function to compare peak frequency with predefined frequencies
void comparePeakFrequency() {
  // Check if the peak frequency corresponds to C4 or A4 note
  if (isInRange(peakFrequency, C4_FREQUENCY)) {
    Serial.println("Detected note: C4");
  } else if (isInRange(peakFrequency, A4_FREQUENCY)) {
    Serial.println("Detected note: A4");
  } else {
    Serial.println("Unknown note");
  }
  // Delay for readability
  delay(1000);
}

// Function to check if a frequency is within a certain range
bool isInRange(double frequency, double targetFrequency) {
  // Calculate lower and upper bounds based on error margin
  double lowerBound = targetFrequency - (targetFrequency * FREQUENCY_ERROR_MARGIN);
  double upperBound = targetFrequency + (targetFrequency * FREQUENCY_ERROR_MARGIN);
  // Check if the frequency is within the range
  return (frequency >= lowerBound && frequency <= upperBound);
}