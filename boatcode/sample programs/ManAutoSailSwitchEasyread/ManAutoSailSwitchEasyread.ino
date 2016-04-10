/* RudderFollowsWind rev 4/8/2016
© 2014-2016 RoboSail
Find detailed description in Decription tab
*/

#include <Servo.h>
#include "RoboSail.h"
boolean verbose = true;  //true calls function for values to be printed to monitor
boolean autoSail = false; // set to true for automatic sail
//Fill in min/max parameters for the RC Receiver and WindSensor in RoboSail.h tab

void setup() {
  Serial.begin(115200);
  Serial.println("\nRoboSail ManAutoSailSwitch - XXXXX");  //write program name here
  // Set RC receiver and WindSensor on digital input pins
  declarePins();
}

void loop() {
//*********** Read in data from the RC receiver and sensors *********
readReceiverSensors();

//**************** your code here ******************
// You have values from the RC Receiver and Sensors in these variable: 
// rudderPosition, sailPosition, and windAngle
// Calculate new values for rudderPosition and sailPosition in degrees 
// and set those variables to the new values.
// If you do not set the values, it will use the values from the RC Receiver
// For example, to make the rudder follow the wind angle you would have:
// rudderPosition = windAngle;


//  Automatic sail switch
if (sailPulseWidth > 1850){autoSail=true;}
else {autoSail=false;}

if (autoSail)
  {
  windAngle = abs(windAngle);           // sail position can only be positive
  if (windAngle < 45) sailPosition = 0;
  if ((windAngle >= 45) && (windAngle < 135)) sailPosition = windAngle - 45;
  if (windAngle >= 135) sailPosition = 90;
  // sailPosition = (180-abs(windAngle))/2;  // a different algorithm for automatic sail 
  }

// if autoSail = false sailPosition will be set by RC transmitter signal  
//  rudderPosition will always be set by RC transmitter signal


  
/********************* send commands to motors *************************/
  driveSailServo(sailPosition);
  driveRudderServo(rudderPosition);
  
  if (verbose) {printToMonitor();}
  
} //end of loop()


