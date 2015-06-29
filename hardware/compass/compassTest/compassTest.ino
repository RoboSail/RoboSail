/* CompassTest rev 6/29/2015
© 2014-2015 RoboSail
Use this program after getting hardiron calibration values from compassCalibration.
Edit the code here and enter the hardiron calibration factors.
You will also need to adjust the declination based on your geographical area.

This program calculates and displays roll, pitch, yaw, standard Compass heading 
and RoboSail Heading.  Standard Compass heading defines North = 0, E = 90, S = 180, W = 270.
For Robosail we define E = 0, N = 90, W = 180, s = 270.  The translation is done in the code.

It also tests the orientation.cpp library where the calculations are done.  
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <orientation.h>

// These values will need to be adjusted based on your particular compass.
// Use compassCalibration to determine the correct hard iron calibration.
float hardiron_x = -11.91;
float hardiron_y = 5.05;
float hardiron_z = -1.02;

// Source: http://www.ngdc.noaa.gov/geomag-web/#igrfwmm
float declination = -14.6067;

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Orientation compass;

float roll;
float pitch;
float yaw;
float heading;
float robosailHeading;

void setup() {
  Serial.begin(9600);
  Serial.println("RoboSail Compass Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin() || !accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("no LSM303 detected ... Check the wiring");
    while(1);
  }
  Serial.print("Your hardiron values are X: "); Serial.print(hardiron_x);
  Serial.print("  Y: "); Serial.print(hardiron_y); 
  Serial.print("  Z: "); Serial.println(hardiron_z); Serial.println();

  // Set the calibration parameters
  compass.setParameters(&accel, &mag, declination, hardiron_x, hardiron_y, hardiron_z);
}

void loop() {
  // Get a compass reading
  compass.calculate(roll, pitch, yaw, heading);
  robosailHeading = (360 - heading) + 90;
  if (robosailHeading >= 360) {robosailHeading -= 360;}

  // Display the results
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print(", Pitch: "); Serial.print(pitch);
  Serial.print(", Yaw: "); Serial.print(yaw);
  Serial.print(",   Compass Heading: "); Serial.print(heading); Serial.print(" °");
  Serial.print(",   RoboSail Heading: "); Serial.print(robosailHeading); Serial.println(" °");
  delay(500);
}

