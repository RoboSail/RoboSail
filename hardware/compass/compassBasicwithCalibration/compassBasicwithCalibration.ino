/* Hard iron calibration must be performed before you can get accurate compass readings.
 * Use compassCalibration (in the Orientation library) to determine the correct hard iron calibration.
 * Once you have determined the hard iron calibration factors, edit the sketch below.
 *
 * Note:
 * This sketch is written with the Y axis flipped from how it is shown on the LSM303.
 * If you hold the LSM303 such that the chips are up and the connector is closest to you
 * (the text should be right-side-up):
 * - The positive X-axis points away from you.
 * - The positive Y-axis points right.
 * - The positive Z-axis points down.
 *
 * You may need to adjust the signs in the code below so that:
 * - When an axis points straight down, the accelerometer reading on that axis should be positive
 * - When an axis points towards magnetic north, the magnetometer reading on that axis should be positive
 */
 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

// These values will need to be adjusted based on your particular compass.
// Use compassCalibration (in the Orientation library) to determine the correct hard iron calibration.
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
  Serial.begin(115200);
  Serial.println("Magnetometer + Accelerometer Test with Hard Iron Calibration"); Serial.println("");
  
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!mag.begin() || !accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
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

  delay(1000);
}

