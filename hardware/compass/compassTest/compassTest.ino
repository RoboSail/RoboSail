/* Hard iron calibration must be performed before you can get accurate compass readings.
 * Use compassCalibration to determine the correct hard iron calibration.
 * Once you have determined the hard iron calibration factors, edit the sketch below.
 *
 * You will also need to adjust the declination based on your geographical area.
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

void setup() {
  Serial.begin(115200);
  Serial.println("Compass Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!mag.begin() || !accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  // Set the calibration parameters
  compass.setParameters(&accel, &mag, declination, hardiron_x, hardiron_y, hardiron_z);
}

void loop() {
  // Get a compass reading
  compass.calculate(roll, pitch, yaw, heading);
  
  // Display the results
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print(", Pitch: "); Serial.print(pitch);
  Serial.print(", Yaw: "); Serial.print(yaw);
  Serial.print(", Heading: "); Serial.println(heading);

  delay(1000);
}

