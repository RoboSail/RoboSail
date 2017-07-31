// Sweep through all the possible positions for two servos.
// Useful as a simple test program.

#include <Servo.h>

// Pin assignments
#define SERVO_PIN 7  // declare pin for servo

Servo Servo1;

// variable to store the servo position
int pos = 0;

void setup() {
  // sets servo on the specified pin to be controlled
  Servo1.attach(SERVO_PIN);
}

void loop() {
  // rotate in one direction
  // goes from 0 degrees to 180 degrees in steps of 1 degree
  // tell servo to go to position in variable 'pos'
  for (int pos = 0; pos < 180; pos += 1) {
    Servo1.write(pos);

    // waits 15ms for the servo to reach the position
    delay(15);
  }

  // rotate in the opposite direction
  for (int pos = 180; pos >= 1; pos -= 1) {
    Servo1.write(pos);
    delay(15);
  }
}
