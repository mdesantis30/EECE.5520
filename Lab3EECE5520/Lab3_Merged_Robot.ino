// Lab #3: Sensors and Actuators -  Designing a robot car that can drive itself surrounding an object such as a trash can or
// a one-gallon milk bottle using a robot kit using a distance measurement function to determine the car’s distance from 
// the nearby object a and then control the movement of the car accordingly.
// Matthew DeSantis and Ryan Mouhib, EECE.5520

#include <arduinoFFT.h>

arduinoFFT FFT = arduinoFFT(); // Create FFT object

#define SAMPLES 128           // Number of samples
#define SAMPLING_FREQUENCY 4700  // Sampling frequency in Hz
#define DETECTION_WINDOW 500 // Detection window in milliseconds

// Sound sensor module pins
int sensorAnalogPin = A0;    // Select the Arduino input pin to accept the Sound Sensor's analog output 
int analogValue = 0;         // Define variable to store the analog value coming from the Sound Sensor

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
void setMotorControl(int speedA, int speedB, bool dirA, bool dirB, int duration)
{
  if (speedA == 0)
  {
  // Set motor A (right motor) direction
  digitalWrite(in1Pin, dirA ? LOW : LOW);
  digitalWrite(in2Pin, !dirA ? LOW : LOW);
  // Set motor B (left motor) direction
  digitalWrite(in3Pin, dirB ?  LOW : HIGH);
  digitalWrite(in4Pin, !dirB ? LOW : HIGH);
  }
  else if (speedB == 0)
  {
  // Set motor A (right motor) direction
  digitalWrite(in1Pin, dirA ? LOW : HIGH);
  digitalWrite(in2Pin, !dirA ? LOW : HIGH);
  // Set motor B (left motor) direction
  digitalWrite(in3Pin, dirB ?  LOW : LOW);
  digitalWrite(in4Pin, !dirB ? LOW : LOW);
  }
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
  
enum State
{
  CIRCLING,
  STOPPED
};

State currentState = CIRCLING;

void loop()
{
  double vReal[SAMPLES]; // Array to store real values
  double vImag[SAMPLES]; // Array to store imaginary values
  double peakSum = 0;    // Variable to store sum of peak frequencies
  int peakCount = 0;     // Variable to store count of detected peaks

  unsigned long detectionStartTime = millis(); // Timestamp when detection starts

  int motorSpeedA = 100; // set speed of motor A
  int motorSpeedB = 100; // set speed of motor B

  while (millis() - detectionStartTime < DETECTION_WINDOW)
  {
    unsigned int distance = readDistance(); // read distance from the ultrasonic sensor

    // Read analog values from sound sensor
    for (int i = 0; i < SAMPLES; i++)
    {
      analogValue = analogRead(sensorAnalogPin);
      vReal[i] = analogValue; // Store analog value in real array
      vImag[i] = 0;           // Set imaginary value to 0
      delayMicroseconds(100);  // Adjust delay as needed
    }

    // Perform FFT
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    // Find peak frequency
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    // Check if peak is within frequency ranges of A4 or C4
    if ((peak > 257 && peak < 267) || (peak > 431 && peak < 449))
    {
      peakSum += peak;
      peakCount++;
    }

    // Adjust motor movement based on distance
    if (currentState == CIRCLING) {
      if (distance < 120) // If the measured distance is less than the desired distance: 120 mm, turn slightly left
      {
        setMotorControl(motorSpeedA, 0, true, false, 100); // Turn slightly left to check distance and shape of object being circled
      }
      else if (distance > 150) // If the measured distance is greater than the desired distance: 150 mm, turn slightly right
      {
        setMotorControl(0, motorSpeedB, false, true, 100); // Turn slightly right to continue circling object
      }
    }
  }

  // Calculate average peak frequency
  double averagePeak = peakSum / peakCount;

  // Determine if the tone is A4 or C4 based on average peak frequency
  if (averagePeak > 431 && averagePeak < 449)
  {
    Serial.print("A4 note detected at average peak frequency: ");
    Serial.print(averagePeak, 1);
    Serial.println(" Hz.");
    currentState = CIRCLING; // Set state to circling
  }
  else if (averagePeak > 257 && averagePeak < 267)
  {
    // C4 note detected, stop motors
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, LOW);

    Serial.print("C4 note detected at average peak frequency: ");
    Serial.print(averagePeak, 1);
    Serial.println(" Hz.");
    currentState = STOPPED; // Set state to stopped
  }
}
