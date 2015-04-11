// Helpful program to see what the RC tranmitter is sending to the servos,
// and to test and figure out the limits of the attached servos.

// This program will print the recieved values, and the corresponding angle,
// to the serial console.

#include <Servo.h>

// Pin assignments
//input pins from receiver
const int ROBOSAIL_PIN_RUDDER_RC = 2;
const int ROBOSAIL_PIN_SAIL_RC = 3;

const int ROBOSAIL_INTERRUPT_SAIL = 1;
const int ROBOSAIL_INTERRUPT_RUDDER = 0;

volatile unsigned long rcPulseRudderStart = 0;
volatile int rcPulseRudderWidth;
volatile unsigned long rcPulseSailStart = 0;
volatile int rcPulseSailWidth;

/** Handy method that we can reuse for multiple ISRs.
 */
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

void setup() {
  Serial.begin(115200);

  attachInterrupt(ROBOSAIL_INTERRUPT_RUDDER, rcInputRudderISR, CHANGE);
  attachInterrupt(ROBOSAIL_INTERRUPT_SAIL, rcInputSailISR, CHANGE);
}

void loop() {
  // the rcPulse****Width variables are set automatically in the background.
  // Calculate the servo position in degrees.
  int rudderServoOut = map(rcPulseRudderWidth, 1000, 2000, -75, 75);
  int sailServoOut = map(rcPulseSailWidth, 1090, 1900, 0, 90);

  // Print out the values for debug.
  Serial.print("rudder, pulse: ");
  Serial.print(rcPulseRudderWidth);
  Serial.print("\tangle: ");
  Serial.print(rudderServoOut);

  Serial.print("\t\tsail, pulse: ");
  Serial.print(rcPulseSailWidth);
  Serial.print("\tangle: ");
  Serial.print(sailServoOut);

  Serial.print("\n"); // Print a new line
  
  delay(250);
}

