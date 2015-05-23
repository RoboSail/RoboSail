/* Compass Calibration Instructions
 * Hard iron calibration must be performed before you can get accurate compass readings.
 *
 * The process is simple:
 *   1. Mount the magnetometer in the location that you intend to use it.
 *   2. Slowly rotate the body through all possible orientations.
 *   3. Record the minimum and maximum for each axis of the magnetometer.
 *   4. Average the minumum and maximum for each axis. This will give you your hardiron x,y,z offsets.
 *
 * This sketch below will help you with this process.
 * 
 * The output is in the form:
 * 
 * X: min value < average value < max value: corrected reading (raw reading)
 *
 * For example:
 * X: -69.82 < -11.91 < 46.00 : 18.73 (6.82) Y: -51.09 < 5.50 < 62.09 : -48.23 (-42.73) Z: -60.31 < -1.02 < 58.27 : 23.16 (22.14) 
 *
 * Minimum X is -69.82 uT
 * Maximum X is 46.00 uT
 * Hard iron calibration for X is -11.91 uT
 * Current raw X reading is 6.82 uT
 * Current corrected X reading is 18.73 uT.
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

float magXrange[2];
float magYrange[2];
float magZrange[2];

float magXcal = 0;
float magYcal = 0;
float magZcal = 0;

unsigned long lastPrint = 0;

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
  Serial.println("Magnetometer Hard Iron Calibration"); Serial.println("");
  
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  
  /* Initialise the sensor */
  if(!mag.begin())
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
  sensors_event_t mag_event; 
  mag.getEvent(&mag_event);
  
  magCalibration(mag_event);
  
  float x = mag_event.magnetic.x - magXcal;
  float y = mag_event.magnetic.y - magYcal;
  float z = mag_event.magnetic.z - magZcal;
  
  if ((millis() - lastPrint) > 2000)
  {
     /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    Serial.print("X: ");
    Serial.print(magXrange[0]); Serial.print(" < "); Serial.print(magXcal); Serial.print(" < "); Serial.print(magXrange[1]);
    Serial.print(" : "); Serial.print(x); Serial.print(" ("); Serial.print(mag_event.magnetic.x); Serial.print(") ");
  
    Serial.print("Y: ");
    Serial.print(magYrange[0]); Serial.print(" < "); Serial.print(magYcal); Serial.print(" < "); Serial.print(magYrange[1]);
    Serial.print(" : "); Serial.print(y); Serial.print(" ("); Serial.print(mag_event.magnetic.y); Serial.print(") ");
    
    Serial.print("Z: ");
    Serial.print(magZrange[0]); Serial.print(" < "); Serial.print(magZcal); Serial.print(" < "); Serial.print(magZrange[1]);
    Serial.print(" : "); Serial.print(z); Serial.print(" ("); Serial.print(mag_event.magnetic.z); Serial.println(") ");
    
    lastPrint = millis();
  }
}

void magCalibration(sensors_event_t mag_event)
{
  magXrange[0] = min(magXrange[0], mag_event.magnetic.x);
  magXrange[1] = max(magXrange[1], mag_event.magnetic.x);

  magYrange[0] = min(magYrange[0], mag_event.magnetic.y);
  magYrange[1] = max(magYrange[1], mag_event.magnetic.y);
  
  magZrange[0] = min(magZrange[0], mag_event.magnetic.z);
  magZrange[1] = max(magZrange[1], mag_event.magnetic.z);
  
  magXcal = (magXrange[0] + magXrange[1])/2;
  magYcal = (magYrange[0] + magYrange[1])/2;
  magZcal = (magZrange[0] + magZrange[1])/2;
}
