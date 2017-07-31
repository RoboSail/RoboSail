/* BoatCodeStarterWind_Accel rev 7/31/2017
© 2014-2017 RoboSail
Find detailed description in Decription tab
*/

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "RoboSail.h"
//Fill in min/max parameters for the RC Receiver and WindSensor in RoboSail.h tab
boolean displayValues = true;  //true calls function for values to be printed to monitor


void setup() {
  Serial.begin(115200);
  accel.begin();
  Serial.println("\nRoboSail BoatCode using Wind and Accel - XXXXX");  //write program name here
  // Set RC receiver and WindSensor on digital input pins
  declarePins();
 
}

void loop() {
//*********** Read in data from the RC receiver and sensors *********
readReceiver();
readWind();
readAccel();

// You now have values from the RC Receiver and Sensors in these variable: 
// rudderPosition, sailPosition, and windAngle, pitch, roll, yaw as
// defined in the RoboSail.h tab
// Calculate new values for rudderPosition and sailPosition in degrees 
// and set those variables to the new values.
// If you do not set the values, it will use the values from the RC Receiver
//**************** your code here - add lines as needed ******************


  
/********************* send commands to motors *************************/
  driveSailServo(sailPosition);
  driveRudderServo(rudderPosition);
  
  if (displayValues) {printToMonitor();}
  
} //end of loop()


