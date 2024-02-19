// Lab #1: Traffic Light Controller
// Matthew DeSantis and Ryan Mouhib, EECE.5520
// This lab is to design a controller for 2 traffic lights with 4 lights each: Red, Yellow, Green, Green-Left-Arrow.
// The R-GLA-G-Y pattern continues until the system is powered off, an active buzzer beeps for 3 seconds before a light is changed.

int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
int clockPin = 9;       // (9) SH_CP [SCK] on 74HC595
int dataPin = 12;       // (12) DS [S1] on 74HC595
int buzzer = 13;        // the pin of the active buzzer

void setup()
{
  // Set the three shift register pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //initialize the buzzer pin as an output
  pinMode(buzzer, OUTPUT);
}

// Function to be called in the traffic lights loop each time an LED needs to blink and buzzer beep for the last 3 seconds
void blinkLED(int pin) // the pin parameter determines which LEDs will blink
{
  int startTime;
  int endTime;
  int loopcount;
  startTime = millis();
  endTime = startTime;
  digitalWrite(buzzer, HIGH);
  while ((endTime - startTime) <= 3000) // loops for 3 seconds
  {
    digitalWrite(latchPin, LOW);  // flashing specified LEDs repeatedly by first turning it on for 0.5 seconds then off for 0.5 seconds
    shiftOut(dataPin, clockPin, MSBFIRST, pin); // pins are specified when calling blinkLED function in traffic lights loop
    digitalWrite(latchPin, HIGH); // LEDs on
    delay(500); // 0.5 second delay while LEDs are on
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
    digitalWrite(latchPin, HIGH); // LEDs off
    delay(500); // 0.5 second delay while LEDs are off
    loopcount = loopcount+1;
    endTime = millis();
  }
  digitalWrite(buzzer, LOW);
}

// Traffic lights loop
void loop()
{
  // GLA stays on for 5 seconds
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00011000); // the GLA LEDs are synchronized for both traffic lights
  digitalWrite(latchPin, HIGH); // turning GLA LEDs on
  delay(2000);  // GLA stays on full for 2 seconds before flashing for 3 seconds 
  blinkLED(0b00011000); // calling function to blink GLA LEDs and buzzer beep for 3 seconds
  
  // Green stays on for 12 seconds
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00100100); // the green LEDs are synchronized for both traffic lights
  digitalWrite(latchPin, HIGH); // turning green LEDs on
  delay(9000); // green stays on full for 9 seconds before flashing for 3 seconds 
  blinkLED(0b00100100); // calling function to blink green LEDs and buzzer beep for 3 seconds

  // Yellow stays on for 3 seconds
  digitalWrite(buzzer, HIGH);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b01000010); // the yellow LEDs are synchronized for both traffic lights
  digitalWrite(latchPin, HIGH); // turning yellow LEDs on
  delay(3000);  // Yellow stays on for 3 seconds
  digitalWrite(buzzer, LOW);

  // Red stays on for 20 seconds
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b10000001); // the red LEDs are synchronized for both traffic lights
  digitalWrite(latchPin, HIGH); // turning red LEDs on
  delay(17000); // green stays on full for 17 seconds before flashing for 3 seconds 
  blinkLED(0b10000001); // calling function to blink red LEDs and buzzer beep for 3 seconds
}

// Sources:
// Elegoo Super Starter Kit forUNO, Lesson 6 Active buzzer
// Elegoo Super Starter Kit forUNO, Lesson 16 Eight LED with 74HC595
