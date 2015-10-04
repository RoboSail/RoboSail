/* RudderServoTest rev 7/17/2015
   © 2014-2015 RoboSail
   Test the various rudder positions available by driving the
   rudder servo to positons entered by the user.
   The user types in the desired angle through the Serial Monitor.
   -50 means the rudder is fully to port, and +50 means the rudder
   is fully to starboard.
   Note that the Rudder Servo actual center postion is 90, so
   the user's desired angles must be mapped in the code to these values.
   
   The Rudder servo is controlled on digital pin 8
   */

#include <Servo.h>

const int ROBOSAIL_PIN_RUDDER_SERVO = 8;

Servo rudderServo;  // define servo 

// variable to store the servo position
int pos = 0;

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(115200);
  
  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);
}

void loop() {
  Serial.print("Enter desired rudder angle (-50 to 50): ");
  while (Serial.available() == 0)
  {}
  pos = Serial.parseInt();   // convert input to integer
  Serial.println(pos);      // print what was entered

  // convert from rudder angle to servo angle
  pos = map(pos, -90, 90, 0, 180);

  // print out what was calculated
  Serial.print("Command sent to Servo: ");
  Serial.println(pos);
  Serial.println();

  // drive servo to position in variable 'pos'
  rudderServo.write(pos);
}

