/* RCReaderAdvanced rev 4/21/2015
© 2014-2015 RoboSail
This program puts the Arduino micro-computer to read the control signals 
coming in from the RC (Radio Control) system for both the Rudder and Sail channels.

It displays several values to the Serial Monitor:
  - The actual "pulse" coming in from the receiver for each channel
    (typical range of 1000 - 2000)
  - the angle at which the rudder or sail should be positioned 
    given that command (in the RoboSail frame of reference) 

This program helps the user determine 
  - if they are reading good signals from the receiver (range of 1000 - 2000)
  - if the Arduino computer is functioning correctly

  NOTE: This program uses interrupts to read in the PWM signals from the Receiver
  rather than using the pulseIn() function. It calculates the pulse length by 
  reading the Low-High and High-Low transitions on the input pins and 
  calculating the time lapsed between them.
 
*/

#include <Servo.h>

// Pin assignments
//input pins from receiver
const int ROBOSAIL_PIN_RUDDER_RC = 2;
const int ROBOSAIL_PIN_SAIL_RC = 3;

const int ROBOSAIL_INTERRUPT_SAIL = 1;
const int ROBOSAIL_INTERRUPT_RUDDER = 0;

// variables to hold input and output values
volatile unsigned long rcPulseRudderStart = 0;
volatile int rcPulseRudderWidth;
volatile unsigned long rcPulseSailStart = 0;
volatile int rcPulseSailWidth;
int rudderServoOut;
int sailServoOut;

void setup() {
  Serial.begin(9600);
  Serial.println("\nRC Reader code - RoboSail");

  attachInterrupt(ROBOSAIL_INTERRUPT_RUDDER, rcInputRudderISR, CHANGE);
  attachInterrupt(ROBOSAIL_INTERRUPT_SAIL, rcInputSailISR, CHANGE);
}

void loop() {
  // the rcPulse****Width variables are set automatically in the background.
  // Calculate the servo position in degrees.
  rudderServoOut = map(rcPulseRudderWidth, 1000, 2000, -75, 75);
  sailServoOut = map(rcPulseSailWidth, 1090, 1900, 0, 90);

  // Print out the values for debug.
  Serial.print("Rudder, pulse: ");
  Serial.print(rcPulseRudderWidth);
  Serial.print("\t Angle: ");
  Serial.print(rudderServoOut);

  Serial.print("\t\t Sail, pulse: ");
  Serial.print(rcPulseSailWidth);
  Serial.print("\t Angle: ");
  Serial.print(sailServoOut);

  Serial.print("\n"); // Print a new line
  
  delay(250);
}

//*************Functions to handle interrupts***********

// Handy method that we can reuse for multiple ISRs.

void calculatePulseIn(const int pin, volatile unsigned long & timeStart, volatile int & pulseTime) {
  //if the pin has gone HIGH, record the microseconds since the Arduino started up
  if (digitalRead(pin) == HIGH) {
    timeStart = micros();
  }
  //otherwise, the pin has gone LOW
  else {
    //only worry about this if the timer has actually started
    if (timeStart > 0) {
      //record the pulse time
      pulseTime = (volatile int)micros() - timeStart;
      
      //restart the timer
      timeStart = 0;
    }
  }
}

void rcInputRudderISR() {
  calculatePulseIn(ROBOSAIL_PIN_RUDDER_RC, rcPulseRudderStart, rcPulseRudderWidth);
}

void rcInputSailISR() {
  calculatePulseIn(ROBOSAIL_PIN_SAIL_RC, rcPulseSailStart, rcPulseSailWidth);
}


