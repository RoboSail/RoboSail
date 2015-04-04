// Sweep through all the possible positions for two servos.
// Useful as a simple test program.

#include <Servo.h>

// Pin assignments
const int ROBOSAIL_PIN_RUDDER_SERVO = 8;
const int ROBOSAIL_PIN_SAIL_SERVO = 9;

// The maximum angle that a servo can rototate to.
const int MAX = 180;

// create servo object to control a servo
// a maximum of eight servo objects can be created
Servo myservo1;
Servo myservo2;

// variable to store the servo position
int pos = 0;

void setup() {
  // sets servo on the specified pin to be controlled
  myservo1.attach(ROBOSAIL_PIN_SAIL_SERVO);
  myservo2.attach(ROBOSAIL_PIN_RUDDER_SERVO);
}

void loop() {
  // rotate in one direction
  // goes from 0 degrees to 180 degrees in steps of 1 degree
  // tell servo to go to position in variable 'pos'
  for (int pos = 0; pos < MAX; pos += 1) {
    myservo1.write(pos);
    myservo2.write(pos);

    // waits 15ms for the servo to reach the position
    delay(15);
  }

  // rotate in the opposite direction
  for (int pos = MAX; pos >= 1; pos -= 1) {
    myservo1.write(pos);
    myservo2.write(pos);
    delay(15);
  }
}
