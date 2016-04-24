/* CompassTest rev 7/22/2015 IN PROCESS
© 2014-2016 RoboSail
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <orientation.h>

// These values will need to be adjusted based on your particular compass.
// Use compassCalibration to determine the correct hard iron calibration.
// Data from 7/26/15 Hard iron calibration for X: -1.59 for Y: -7.18 for Z: 34.44
//Data from 7/29 BLI Hard iron calibration for X: -8.41 for Y: -8.00 for Z: 33.32
float hardiron_x = -8.41;
float hardiron_y = -8.00;
float hardiron_z = 33.32;

// Source: http://www.ngdc.noaa.gov/geomag-web/#igrfwmm
float declination = -14.6067;

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Orientation compass;
boolean sensorGood = false;
float roll;
float pitch;
float yaw;
float heading;
float robosailHeading;

void setup() {
  Serial.begin(115200);
  Serial.println("RoboSail Compass Test"); Serial.println("");
  
  /* Wait for the sensor to initialize  */
    if(!mag.begin() || !accel.begin()) sensorGood = false;
    else sensorGood = true;
  //  if (accel.begin()) { sensorGood = true; } //hanging up here
  while (sensorGood == false) 
  {
   Serial.println("no LSM303 detected ... Check the wiring");
    delay(2000); // wait and try again
  }
  initialDisplay();

  // Set the calibration parameters
  compass.setParameters(&accel, &mag, declination, hardiron_x, hardiron_y, hardiron_z);
}

void loop() {
  // Get a compass reading
  compass.calculate(roll, pitch, yaw, heading);
  robosailHeading = (360 - heading) + 90;   //change crom standard compass reading to RoboSail frame of ref
  if (robosailHeading >= 360) {robosailHeading -= 360;}

  displayToScreen();
  delay(300);
}

