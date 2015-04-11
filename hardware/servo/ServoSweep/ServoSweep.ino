// Sweep through all the possible positions for two servos.
// Useful as a simple test program.

#include <Servo.h>

// Pin assignments
const int ROBOSAIL_PIN_RUDDER_SERVO = 8;
const int ROBOSAIL_PIN_SAIL_SERVO = 9;

Servo rudderServo;
Servo sailServo;

// variable to store the servo position
int pos = 0;

void setup() {
  // sets servo on the specified pin to be controlled
  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);
  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);
}

void loop() {
  // rotate in one direction
  // goes from 0 degrees to 180 degrees in steps of 1 degree
  // tell servo to go to position in variable 'pos'
  for (int pos = 0; pos < 180; pos += 1) {
    rudderServo.write(pos);
    sailServo.write(pos);

    // waits 15ms for the servo to reach the position
    delay(15);
  }

  // rotate in the opposite direction
  for (int pos = 180; pos >= 1; pos -= 1) {
    rudderServo.write(pos);
    sailServo.write(pos);
    delay(15);
  }
}
