// EECE.5520 Microprocessors II
// Matthew DeSantis and Ryan Mouhib
// Lab 1 - Traffic Lights
// This program is designed to simulate the timing and execution of a traffic light system.
// For the graduate section, this program will execute faster light blinking, intersection traffic lights, and a 7 segment display for the timer.

//timer setup for timer0, timer1, and timer2.
//this code will enable one arduino timer interrupt
//timer1 will interrupt at 1Hz

//storage variables
boolean toggle1 = 0;

void setup(){
  
    //digital pin and constants for button debounce
    const int buttonPin = 10;
    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
    unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
    int buttonState;            // the current reading from the input pin
    int lastButtonState = LOW;  // the previous reading from the input pin

    //variable setup for first set of traffic lights
    const int greenLED1Pin = 41;
    const int yellowLED2Pin = 39;
    const int redLED3Pin = 37;

    //variable setup for second set of traggic lights
    const int greenLED4Pin = 43;
    const int yellowLED5Pin = 45;
    const int redLED6Pin = 47;
    //nearest LED to the buzzer is pin 47, farthest from the buzzer is pin 41

    pinMode(buttonPin, INPUT);
    //button mode should only work for the red lights
    pinMode(redLED3Pin, OUTPUT); //first red light
    pinMode(redLED6Pin, OUTPUT); //second red light

    pinMode(yellowLED2Pin, OUTPUT); //first yellow light
    pinMode(yellowLED5Pin, OUTPUT); //second yellow light

    pinMode(greenLED1Pin, OUTPUT); //first green light
    pinMode(greenLED4Pin, OUTPUT); //second green light
    

    //attachInterrupt(0, pin_ISR, CHANGE);  //  "Watches" in the background for a  button press
    /* Attach an interrupt to the ISR vector to monitor Push Button.
      Number 0 (for digital pin 2) or number 1 (for digital pin 3) are used.
      Interrupts  are useful for making things happen automatically in
      microcontroller programs,and  can help solve timing problems.
      Good tasks for using an interrupt may include  reading
      a rotary encoder, or monitoring user input */
    // Set Initial  state of all red LED to HIGH
    digitalWrite (redLED3Pin, HIGH);
    digitalWrite  (redLED6Pin, HIGH);

    //set pins as outputs
    pinMode(13, OUTPUT);

    cli();//stop interrupts

    //set timer1 interrupt at 1Hz
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0
    // set compare match register for 1hz increments
    OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    sei();//allow interrupts

}//end setup

// Timer1's interrupt service routing (ISR)
// The code in ISR will be executed every time timer1 interrupt occurs
// That is, the code will be called once every second
// TODO
//   you need to set a flag to trigger an evaluation of the conditions
//   in your state machine, then potentially transit to next state
//
ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
  //generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
    if (toggle1){
      digitalWrite(13,HIGH);
      toggle1 = 0;
    }
    else{
      digitalWrite(13,LOW);
      toggle1 = 1;
    }
  }
  
void loop(){
  //do other things here



  //loop for program
  if (toggle1) {
  }
  else {
  }
}


