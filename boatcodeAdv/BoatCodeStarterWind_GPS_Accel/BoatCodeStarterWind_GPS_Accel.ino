/* BoatCodeStarterWind+GPS+Accel rev 7/31/2016
© 2014-2016 RoboSail
Full description under Description tab */

boolean verbose = true;  //true calls function for values to be printed to monitor

#include <Servo.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <UsefulCalcs.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include "RoboSail.h"
//Fill in min/max parameters for the RC Receiver and WindSensor in RoboSail.h tab

Adafruit_GPS GPS(&Serial1);
// initialize utility that will convert lat/lon to (x,y) positions in meters
UsefulCalcs calc(false);



void setup() {
  Serial.begin(115200);
  Serial.println("\nRoboSail BoatCode Wind+GPS+Accel 7/31/16");  //write program name/rev here
  accel.begin();
  
  // Set RC receiver and all Sensors on digital input pins
  declarePins();
  
  checkGPS();

  checkCompass(); // will check Accelerometer
}

void loop() {
//*********** Read in data from the RC receiver and sensors *********
  readReceiver();
  readWind();
  readAccel();
  readGPS();  //puts values in "start" and "relative" variable

// You now have values from the RC Receiver and Sensors in these variables: 
// rudderPosition, sailPosition, and windAngle, 
// pitchAccel, rollAccel, yawAccel, robosailRollAccel, robosailRoll,
// startPositionX, startPositionY, relPositionX, relPositionY, angleFromStart
// as defined in the RoboSail.h tab
// Calculate new values for rudderPosition and sailPosition in degrees 
// and set those variables to the new values.
// If you do not set the values, it will use the values from the RC Receiver

//**************** your code here - add lines as needed ******************


  
/********************* send commands to motors *************************/
  driveSailServo(sailPosition);
  driveRudderServo(rudderPosition);
  
  if (verbose) {printToMonitor();} 
  
} //end of loop()



