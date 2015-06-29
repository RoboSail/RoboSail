/* SailServoTest rev 6/28/2015
   © 2014-2015 RoboSail
   Test the various sail positions available by driving the
   sail servo to positons entered by the user.
   The user types in the desired angle through the Serial Monitor.
   0 means the sail is fully in, and 90 means the sail is 
   fully out (at 90 degrees).
   Note that the Sail Servo actual positions for full in and out are approx 55 and 125, 
   so the user's desired angles must be mapped in the code to these values.
*/

#include <Servo.h>

const int ROBOSAIL_PIN_SAIL_SERVO = 9;

// create servo object to control a servo
Servo sailServo;

// variable to store the servo position
int pos = 0;

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(9600);

  // Tell Serial to wait for this long in order to receive a number
  Serial.setTimeout(1000 * 60 * 60);

  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);

}

void loop() {
  Serial.print("Enter desired sail angle (0 to 90): ");

  pos = Serial.parseInt();    // convert input to integer
  Serial.print(pos);          // print what was entered

  // convert from desired sail angle to servo angle
  pos = map(pos, 0, 90, 55, 125);
  
  // print the converted value
  Serial.print(" (servo angle: ");
  Serial.print(pos);
  Serial.println(")");

  // tell servo to go to position in variable 'pos'
  sailServo.write(pos);
}

