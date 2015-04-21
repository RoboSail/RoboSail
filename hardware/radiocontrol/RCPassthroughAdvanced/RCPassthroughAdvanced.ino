/* RCPassthroughAdvanced rev 4/21/2015
© 2014-2015 RoboSail
This program puts the Arduino micro-computer in the RC (Radio Control)system 
It takes in the control signals coming in from the Receiver and
pass them back to the Rudder and Sail servo motors on the boat.
It also displays several values to the Serial Monitor:
  - The actual "pulse" coming in from the receiver for each channel
    typical range of 1000 - 2000)
  - the angle at which the rudder or sail should be positioned 
    given that command (in the RoboSail frame of reference) 

This program helps the user determine 
  - if they are reading good signals from the receiver (range of 1000 - 2000)
  - if the servos are functioning correctly and moving to the expected positions
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
// Output pins to the servos
const int ROBOSAIL_PIN_RUDDER_SERVO = 8;
const int ROBOSAIL_PIN_SAIL_SERVO = 9;

const int ROBOSAIL_INTERRUPT_SAIL = 1;
const int ROBOSAIL_INTERRUPT_RUDDER = 0;

// variables to hold input and output values
volatile unsigned long rcPulseRudderStart = 0;
volatile int rcPulseRudderWidth;
volatile unsigned long rcPulseSailStart = 0;
volatile int rcPulseSailWidth;
int rudderServoOut;
int sailServoOut;

//create servo objects
Servo rudderServo;
Servo sailServo;

void setup() {
  Serial.begin(115200);
  Serial.println("\nRCPassthroughAdvanced code - RoboSail");
  
  // Sets the Receiver to interrupt pins
  attachInterrupt(ROBOSAIL_INTERRUPT_RUDDER, rcInputRudderISR, CHANGE);
  attachInterrupt(ROBOSAIL_INTERRUPT_SAIL, rcInputSailISR, CHANGE);

  // attaches the servos to the proper pins
  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);
  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);
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
  
  //adjust the servo values from the angles on the Robosail boat to 
  // values for the servos, then command the servos to move 
  rudderServoOut = map(rudderServoOut, -90, 90, 0, 180);
  rudderServo.write(rudderServoOut);
  sailServoOut = map(sailServoOut, 0, 90, 55, 125);
  sailServo.write(sailServoOut); 
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

