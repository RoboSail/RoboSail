/* RCPassthrough rev 3/28/2016
© 2014-2016 RoboSail
This program puts the Arduino micro-computer in the RC (Radio Control)system 
It takes in the control signals coming in from the Receiver and
pass them back to the Rudder and Sail servo motors on the boat.
It also displays several values to the Serial Monitor:
  - The actual "pulse" coming in from the receiver for each channel
    (typical range of 1000 - 2000)
  - the angle at which the rudder or sail should be positioned 
    given that command (in the RoboSail frame of reference) 

This program helps the user determine 
  - if they are reading good signals from the receiver (range of 1000 - 2000)
  - if the servos are functioning correctly and moving to the expected positions
  - if the Arduino computer is functioning correctly

Rudder and Sail data from the RC receiver are read in on 
digital pins 2 and 3 respectively.
Rudder and sail servos are controlled on digital 
pins 8 and 9 respectively.
*/

#include <Servo.h>

// Pin assignments
//input pins from receiver
const int RUDDER_RC_PIN = 2;
const int SAIL_RC_PIN = 3;

// Output pins to the servos
const int RUDDER_SERVO_PIN = 8;
const int SAIL_SERVO_PIN = 9;

// variables to hold input and output values
int rudderPulseWidth;
int rudderServoOut;
int sailPulseWidth;
int sailServoOut;

//create servo objects
Servo rudderServo;
Servo sailServo;

void setup() {
  Serial.begin(115200);
  Serial.println("\nRCPassThrough code - RoboSail");
  // Set RC receiver on digital input pins
  pinMode(RUDDER_RC_PIN, INPUT);
  pinMode(SAIL_RC_PIN, INPUT);

  // attach the servos to the proper pins
  rudderServo.attach(RUDDER_SERVO_PIN);
  sailServo.attach(SAIL_SERVO_PIN);
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

