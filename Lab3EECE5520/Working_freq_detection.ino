#include <arduinoFFT.h>

arduinoFFT FFT = arduinoFFT(); // Create FFT object

// Sound sensor module pins
int sensorAnalogPin = A0;    // Select the Arduino input pin to accept the Sound Sensor's analog output 
int analogValue = 0;         // Define variable to store the analog value coming from the Sound Sensor

#define SAMPLES 128           // Number of samples
#define SAMPLING_FREQUENCY 4700  // Sampling frequency in Hz

void setup()
{
  Serial.begin(9600); // Starts the serial communication
}

void loop()
{
  double vReal[SAMPLES]; // Array to store real values
  double vImag[SAMPLES]; // Array to store imaginary values

  // Read analog values from sound sensor
  for (int i = 0; i < SAMPLES; i++)
  {
    analogValue = analogRead(sensorAnalogPin);
    vReal[i] = analogValue; // Store analog value in real array
    vImag[i] = 0;           // Set imaginary value to 0
    delayMicroseconds(100);  // Adjust delay as needed based on the sampling frequency
  }

  // Perform FFT
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  // Find peak frequency
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

  // Print peak frequency
  Serial.print("Peak frequency: ");
  Serial.print(peak, 1);
  Serial.println(" Hz");

  delay(100); // Adjust delay as needed
}
