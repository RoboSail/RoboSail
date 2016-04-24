/* BoatCodeStarterWindOnly rev 5/2/2015
© 2014-2015 RoboSail
This program contains starter code for programming a boat
using information from the Wind Sensor.
For detailed information about getting input from the WindSensor,
input from the RC receiver, and sending output to the Servos,
see these programs: WindSensor and RCPassThroughSimple

Program values are displayed to the Serial Monitor when the
variable "verbose" is set to true.  otherwise set verbose to false.
*/

#include <Servo.h>
boolean verbose = true;  //true calls function for values to be printed to monitor

// Pin assignments
const int ROBOSAIL_PIN_WIND = 7;
//input pins from receiver
const int ROBOSAIL_PIN_RUDDER_RC = 2;
const int ROBOSAIL_PIN_SAIL_RC = 3;
// Output pins to the servos
const int ROBOSAIL_PIN_RUDDER_SERVO = 8;
const int ROBOSAIL_PIN_SAIL_SERVO = 9;
// variables to hold input and output values
int rudderPulseWidth;
int rudderServoOut;
int oldRudderServoOut;
int waitingForZero = 0;
int sailPulseWidth;
int sailServoOut;
//variables for WindSensor
int desiredAngle = 0;
int windAngle = 0;
int windPulseWidth = 0;
int courseChange = 45;

//create servo objects
Servo rudderServo;
Servo sailServo;

void setup() {
  Serial.begin(115200);
  Serial.println("\nRCPassthroughSimple code - RoboSail");
  // Set RC receiver and WindSensor on digital input pins
  pinMode(ROBOSAIL_PIN_RUDDER_RC, INPUT);
  pinMode(ROBOSAIL_PIN_SAIL_RC, INPUT);
  pinMode(ROBOSAIL_PIN_WIND, INPUT);

  // attach the servos to the proper pins
  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);
  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);
}

void loop() {
  //*********** Read in data from the RC receiver and sensors *********
  // Read the command pulse from the RC receiver
  rudderPulseWidth = pulseIn(ROBOSAIL_PIN_RUDDER_RC, HIGH, 25000);
  sailPulseWidth = pulseIn(ROBOSAIL_PIN_SAIL_RC, HIGH, 25000);
  // Calculate the servo position in degrees.
  rudderServoOut = map(rudderPulseWidth, 1000, 2000, -75, 75);
  sailServoOut = map(sailPulseWidth, 1090, 1900, 0, 90);

  // Read values from the WindSensor
  windPulseWidth = pulseIn(ROBOSAIL_PIN_WIND, HIGH, 25000);
  // Convert the wind angle to degrees from PWM.  Range -180 to +180
  windAngle = map(windPulseWidth, 0, 1024, 180, -180);
  windAngle = constrain(windAngle, -180, 180);
  bool Auto = false;
  //**************** your code here ******************
  // calculate values for rudderServoOut and sailServoOut in degrees.
  if (sailPulseWidth > 1500)
  {
    Auto = true;
  } else
  {
    Auto = false;
  }
Serial.println(Auto);
  if (Auto == true) // Autonomous control
  {
    // Check for "zero position of rudder control"
    if (waitingForZero) {
      if (rudderPulseWidth > 1250 && rudderPulseWidth < 1750) {
        waitingForZero = 0;
      }
    }
    else  {  // Not waiting for zero ... means we can accept a command
      // Detect Left Turn and calculate new desired angle
      if (rudderPulseWidth > 1750) {
         desiredAngle = desiredAngle + courseChange;
         Serial.println ("Left Turn Command");
         if (desiredAngle > 180) {
          desiredAngle = desiredAngle - 360;
         } 
         waitingForZero = 1;
      }
      
      // Detect Right Turn and calculate new desired angle
      if (rudderPulseWidth < 1250) {
         desiredAngle = desiredAngle - courseChange;
         Serial.println ("Right Turn Command");
         if (desiredAngle < -180) {
          desiredAngle = desiredAngle + 360;
         } 
         waitingForZero = 1;
      } 
    }  
      // Change Rudder As needed
      rudderServoOut = desiredAngle - windAngle;
      if (rudderServoOut > 180)
      {rudderServoOut -= 360;}
      if (rudderServoOut < -180)
      {rudderServoOut += 360;}
      rudderServoOut /= 2.5;    //Makes less sensitive
      
      rudderServoOut = constrain(rudderServoOut,-60,60);
      rudderServoOut = map(rudderServoOut, -90, 90, 0, 180);
      rudderServo.write(rudderServoOut);
 
  /* Debug Notes:
     + Need to handle case when desired wind angle is near 180.  This does not work
     properly.
     + Need to reverse sign on left vs right turn.  i.e., left push should mean 
     turn boat towards the left.
     * Need to make rudder less sensitive - slower to react. 
     
     */

          
          
  }
  else //Manual control
  {
    rudderServoOut = map(rudderServoOut, -90, 90, 0, 180);
    rudderServo.write(rudderServoOut);
    desiredAngle = windAngle;
    desiredAngle /= courseChange;
    desiredAngle *= courseChange;
  }
  
  // Always automatically trim the sail
    windAngle = abs(windAngle);

    sailServoOut = windAngle;
    //make irons calculation

    if (sailServoOut <= 45)
    {
      sailServoOut = 10;
    }
    else {
      sailServoOut = map(sailServoOut, 45, 180, 1, 135);
      sailServoOut = 13 * sailServoOut / 27;
    }

  if (verbose) {
    printToMonitor();
  }

  //*************create output commands ***********************
  // calculat appropriate values for the servos, then command the servos to move
  // the Rudder servo motor ranges from 0 to 180 with 90 deg in the center
  // the Sailwinch servo is at ~55 deg when full-in, which we think of as 0 deg,
  //  and ~125 deg when full out, which we think of as 90 deg

  sailServoOut = map(sailServoOut, 0, 90, 55, 125);
  sailServo.write(sailServoOut);

  //delay(2000);

} //end of loop()


// Function to Print out all values for debug.
void printToMonitor()
{
  Serial.print("Wind Angle: ");
  Serial.print(windAngle);
  Serial.print("Desired Angle: ");
  Serial.print(desiredAngle);

  Serial.print("  Rudder, from RC: ");
  Serial.print(rudderPulseWidth);
  Serial.print("  angle out: ");
  Serial.print(rudderServoOut);

  Serial.print("\t Sail, from RC: ");
  Serial.print(sailPulseWidth);
  Serial.print("  angle out: ");
  Serial.print(sailServoOut);

  Serial.print("\n"); // Print a new line
}

