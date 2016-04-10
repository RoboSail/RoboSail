/* SailServoTest rev 3/28/2016
   © 2014-2016 RoboSail
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

const int SAIL_SERVO_PIN = 9;

Servo sailServo;    // define servo 

// variable to store the servo position
int position = 0;

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(115200);

  sailServo.attach(SAIL_SERVO_PIN);

}

void loop() {
  Serial.print("Enter desired sail angle (0 to 90): ");
  while (Serial.available() == 0) 
  {}
  position = Serial.parseInt();    // convert input to integer
  Serial.println(position);          // print what was entered

  // convert from desired sail angle to servo angle
  position = map(position, 0, 90, 55, 125);
  
  // print the converted value
  Serial.print("Comand sent to Servo: ");
  Serial.println(position);
  Serial.println();

  // drive servo to position in variable 'position'
  sailServo.write(position);
}

