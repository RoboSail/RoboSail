/* SailServoTest rev 7/22/2015
   © 2014-2015 RoboSail
   Test the various sail positions available by driving the
   sail servo to positons entered by the user.
   The user types in the desired angle through the Serial Monitor.
   0 means the sail is fully in, and 90 means the sail is 
   fully out (at 90 degrees).
   Note that the Sail Servo actual positions for full in and out 
   for 1 boat are approx 55 and 125, and they may vary for other boats.
   Use this code to check these values and adjust as necessary.
   Then map the user's desired angles to these values.    
   
   The Sail servo is controlled on digital pin 9
*/

#include <Servo.h>

const int ROBOSAIL_PIN_SAIL_SERVO = 9;

Servo sailServo;    // define servo 

// variable to store the servo position
int pos = 0;

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(115200);

  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);

}

void loop() {
  Serial.print("Enter desired sail angle (0 to 90): ");
  while (Serial.available() == 0) 
  {}
  pos = Serial.parseInt();    // convert input to integer
  Serial.println(pos);          // print what was entered

  // convert from desired sail angle to servo angle
  pos = map(pos, 0, 90, 55, 125);
  
  // print the converted value
  Serial.print("Comand sent to Servo: ");
  Serial.println(pos);
  Serial.println();

  // drive servo to position in variable 'pos'
  sailServo.write(pos);
}

