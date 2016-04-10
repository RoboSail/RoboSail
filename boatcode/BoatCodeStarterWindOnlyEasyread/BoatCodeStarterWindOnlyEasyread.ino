/* BoatCodeStarterWindOnly rev 4/8/2016
© 2014-2016 RoboSail
Find detailed description in Decription tab
*/

#include <Servo.h>
#include "RoboSail.h"
boolean verbose = true;  //true calls function for values to be printed to monitor

//Fill in min/max parameters for the RC Receiver and WindSensor in RoboSail.h tab

void setup() {
  Serial.begin(115200);
  Serial.println("\nRoboSail BoatCode - XXXXX");  //write program name here
  // Set RC receiver and WindSensor on digital input pins
  declarePins();
}

void loop() {
//*********** Read in data from the RC receiver and sensors *********
readReceiverSensors();

// You now have values from the RC Receiver and Sensors in these variable: 
// rudderPosition, sailPosition, and windAngle
// Calculate new values for rudderPosition and sailPosition in degrees 
// and set those variables to the new values.
// If you do not set the values, it will use the values from the RC Receiver
// For example, to make the rudder follow the wind angle you would have:
// rudderPosition = windAngle;
//**************** your code here ******************


  
/********************* send commands to motors *************************/
  driveSailServo(sailPosition);
  driveRudderServo(rudderPosition);
  
  if (verbose) {printToMonitor();}
  
} //end of loop()


