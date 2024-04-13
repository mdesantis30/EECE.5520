// Lab #3: Sensors and Actuators -  Designing a robot car that can drive itself surrounding an object such as a trash can or
// a one-gallon milk bottle using a robot kit using a distance measurement function to determine the car’s distance from 
// the nearby object a and then control the movement of the car accordingly.
// Matthew DeSantis and Ryan Mouhib, EECE.5520

#include <arduinoFFT.h>

// Ultrasonic sensor pins
const int trigPin = 12;
const int echoPin = 11; // Width of high pulse indicates distance

// Motor control pins: L298N bridge
const int enAPin = 7; // Left motor PWM speed control
const int in1Pin = 5; // Left motor Direction 1
const int in2Pin = 9; // Left motor Direction 2
const int in3Pin = 4; // Right motor Direction 1
const int in4Pin = 8; // Right motor Direction 2
const int enBPin = 6; // Right motor PWM speed control

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

void setup()
{
  // Configuring input/output pins of ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite (trigPin, LOW);
  
  // Configuring motor output pins through the L298N/L293D
  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enBPin, OUTPUT);

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

  Serial.begin(9600); // starts the serial communication
}

void loop()
{
  unsigned int distance = readDistance(); // read distance from the ultrasonic sensor
  int motorSpeedA = 140; // set speed of motor A
  int motorSpeedB = 140; // set speed of motor B
  
  if (distance < 120) // If the measured distance is less than the desired distance: 120 mm, turn slightly left
  {
    setMotorControl(motorSpeedA, motorSpeedB, true, false, 180); // Turn slightly left to check distance and shape of object being circled
    // duration of motors movements in milliseconds defines as 180
  }
  else if (distance > 150) // If the measured distance is greater than the desired distance: 150 mm, turn slightly right
  {
    setMotorControl(motorSpeedA, motorSpeedB, false, true, 180); // Turn slightly right to continue circling object
    // duration of motors movements in milliseconds defines as 180
  }

  // Detect peak frequency from sound sensor
  detectPeakFrequency();
  
  // Compare peak frequency with predefined frequencies
  comparePeakFrequency();

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
  
// Function to detect peak frequency from sound sensor
void detectPeakFrequency() {
  // Collect samples from the sound sensor
  for (int i = 0; i < SAMPLES; i++) {
    microseconds = micros();
    vReal[i] = analogRead(micPin);  // read analog sound signal from the microphone as the real component
    vImag[i] = 0;  // no imaginary component
    while (micros() < (microseconds + sampling_period_us)) {}  // continue to detect the peak frequency and monitor the time of the sample 
  }

  // Apply windowing and compute FFT 
  // see FFT library example
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
  /// Check if the peak frequency corresponds to C4 or A4 note
  if (isInRange(peakFrequency, C4_FREQUENCY)) {
    Serial.println("Detected note: C4");
    // Stop the robot if the detected note is C4 (all motors low)
    stopRobot();
  } 

  else if (isInRange(peakFrequency, A4_FREQUENCY)) {
    Serial.println("Detected note: A4");

    unsigned int distance = readDistance(); // read distance from the ultrasonic sensor
    int motorSpeedA = 140; // set speed of motor A
    int motorSpeedB = 140; // set speed of motor B
    
    // Move the robot if the detected note is A4
    if (distance < 120) // If the measured distance is less than the desired distance: 120 mm, turn slightly left
    {
      setMotorControl(motorSpeedA, motorSpeedB, true, false, 180); // Turn slightly left to check distance and shape of object being circled
      // duration of motors movements in milliseconds defines as 180
    }
    else if (distance > 150) // If the measured distance is greater than the desired distance: 150 mm, turn slightly right
    {
      setMotorControl(motorSpeedA, motorSpeedB, false, true, 180); // Turn slightly right to continue circling object
      // duration of motors movements in milliseconds defines as 180
    }
  }
  
  else {
    Serial.println("Unknown note");  // can only read A4 or C4
  }

}

// Function to check if a frequency is within a certain range
bool isInRange(double frequency, double targetFrequency) {
  // Calculate lower and upper bounds based on error margin
  double lowerBound = targetFrequency - (targetFrequency * FREQUENCY_ERROR_MARGIN);
  double upperBound = targetFrequency + (targetFrequency * FREQUENCY_ERROR_MARGIN);
  // Check if the frequency is within the range
  return (frequency >= lowerBound && frequency <= upperBound);
}

// Function to stop the robot
void stopRobot() {
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
}
