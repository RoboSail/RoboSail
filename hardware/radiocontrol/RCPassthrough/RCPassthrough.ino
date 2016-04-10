
/* RCPassthrough rev 4/8/2016
© 2014-2016 RoboSail
Find detailed description in Decription tab
*/
#include <Servo.h>
#include "RoboSail.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\nRCPassThrough code - RoboSail");
    // Set RC receiver and WindSensor on digital input pins
    declarePins();
}

void loop() {
  // Read commanded (manual) values from the RC reciever
  // pulseIn returns the width of the command pulse in microseconds.
  rudderPulseWidth = pulseIn(RUDDER_RC_PIN, HIGH);
  // Calculate the servo position in degrees.
  rudderServoOut = map(rudderPulseWidth, 1000, 2000, -60, 60);

  sailPulseWidth = pulseIn(SAIL_RC_PIN, HIGH);
  sailServoOut = map(sailPulseWidth, 1090, 1900, 0, 90);

 
  // Print out the values for debug.
  Serial.print("Rudder pulse from receiver: ");
  Serial.print(rudderPulseWidth);
  Serial.print("\t Mapped Angle: ");
  Serial.print(rudderServoOut);

  Serial.print("\t Sail pulse from receiver: ");
  Serial.print(sailPulseWidth);
  Serial.print("\t Mapped Angle: ");
  Serial.println(sailServoOut);
 
  // adjust the servo values from desired angles on the Robosail boat 
  // to appropriate values for the servos, then command the servos to move 
  // the Rudder servo motor ranges from 0 to 180 with 90 deg in the center
  // the Sailwinch servo is at ~ 55 deg when full-in, which we think of as 0 deg,
  //  and ~125 deg when full out, which we thnk of as 90 deg
  rudderServoOut = map(rudderServoOut, -90, 90, 0, 180);
  rudderServo.write(rudderServoOut);
  sailServoOut = map(sailServoOut, 0, 90, 55, 125);
  sailServo.write(sailServoOut);

}

