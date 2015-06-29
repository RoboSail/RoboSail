/* RudderServoTest rev 6/28/2015
   © 2014-2015 RoboSail
   Test the various rudder positions available by driving the
   sail servo to positons entered by the user.
   The user types in the desired angle through the Serial Monitor.
   -50 means the rudder is fully to port, and +500 means the rudder
   is fully to starboard.
   Note that the Rudder Servo actual center postion is 90, so
   the user's desired angles must be mapped in the code to these values.
*/

#include <Servo.h>

const int ROBOSAIL_PIN_RUDDER_SERVO = 8;

// create servo object to control a servo
Servo rudderServo;

// variable to store the servo position
int pos = 0;

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(9600);

  // Tell Serial to wait for this long in order to receive a number
  Serial.setTimeout(1000 * 60 * 60);

  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);

}

void loop() {
  Serial.print("Enter desired rudder angle (-50 to 50): ");

  pos = Serial.parseInt();   // convert input to integer
  Serial.println(pos);      // print what was entered

  // convert from rudder angle to servo angle
  pos = map(pos, -90, 90, 0, 180);

  // print out what was calculated
  Serial.print("Calculated Servo Value: ");
  Serial.println(pos);
  Serial.println();

  // tell servo to go to position in variable 'pos'
  rudderServo.write(pos);
}

