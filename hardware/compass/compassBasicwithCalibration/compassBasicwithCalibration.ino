/* CompassBasicwith Calibration rev 6/29/2015
© 2014-2015 RoboSail
Use this program to verify the LSM303 module after getting hardiron 
calibration values from compassCalibration.
Edit the code here and enter the hardiron calibration factors.
You will also need to adjust the declination based on your geographical area.

This program reads the LSM303 Magnetometer/Accelerometer module through 
the Arduino Mega. It reads the raw values for X, Y, Z from both the 
Magnetometer and Acclerometer and prints them to the screen.  
Magnetometer values re adjusted for hardiron calibration.

Use the code to see that the wiring is set up correctly, the range of 
values coming in from the sensors,and to observe the values as you rotate/move 
the module.


*/
 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

// These values will need to be adjusted based on your particular compass.
// Use compassCalibration.ino to determine the correct hard iron calibration.
float hardiron_x = -11.91;
float hardiron_y = 5.05;
float hardiron_z = -1.02;

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

void displaySensorDetails()
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Magnetometer + Accelerometer Test with Hard Iron Calibration"); Serial.println("");
  
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  
  /* Initialize the sensor */
  if(!mag.begin() || !accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("no LSM303 detected ... Check the wiring");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  Serial.print("Your hardiron values are X: "); Serial.print(hardiron_x);
  Serial.print("  Y: "); Serial.print(hardiron_y); 
  Serial.print("  Z: "); Serial.println(hardiron_z); Serial.println();
}

void loop(void) 
{
  /* Get a new sensor event */
  sensors_event_t accel_event;
  accel.getEvent(&accel_event);

  /* Invert X so that when when X, Y, or Z is pointed down, it has a positive reading. */
  float ax = -accel_event.acceleration.x;
  float ay = accel_event.acceleration.y;
  float az = accel_event.acceleration.z;

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("Accel X: "); Serial.print(ax);
  Serial.print(", Y: "); Serial.print(ay);
  Serial.print(", Z: "); Serial.print(az);
  Serial.print("m/s^2  ");

  /* Get a new sensor event */
  sensors_event_t mag_event;
  mag.getEvent(&mag_event);
  
  /* Adjust for hard iron effects */
  float mx = mag_event.magnetic.x - hardiron_x;
  float my = mag_event.magnetic.y - hardiron_y;
  float mz = mag_event.magnetic.z - hardiron_z;

  /* Invert Y and Z axis so that when X, Y, or Z is pointed towards Magnetic North they get a positive reading. */
  my = -my;
  mz = -mz;
  
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("Mag X: "); Serial.print(mx);
  Serial.print(", Y: ");   Serial.print(my);
  Serial.print(", Z: ");   Serial.print(mz);
  Serial.println("uT  ");

  delay(500);
}

