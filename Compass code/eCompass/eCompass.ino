#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <orientation.h>

// For the fabs function in fixedWidthPrint
#include <Math.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accl = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified magn = Adafruit_LSM303_Mag_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); 
  Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); 
  Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); 
  Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); 
  Serial.print(sensor.max_value); 
  Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); 
  Serial.print(sensor.min_value); 
  Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); 
  Serial.print(sensor.resolution); 
  Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Tilt Compensated Compass  Test"); 
  Serial.println("");

  /* Initialise the sensor */
  if(!accl.begin() || !magn.begin())  // also initalizes mag and accel sensors
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* Enable auto-gain */
  magn.enableAutoRange(true);
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  
  // Boston geomagnetic field
  // Declination -14 degrees
  // Inclunation -6.4 degrees
  // Horizontal Intensity 20,000 nT
  // North Component 19,000 nT north
  // East Component 5,000 nT west
  // Vertical Component 48,000 nT down
  // Total Field 52,000 nT
  float declination = -14;

  // Boat  A | hardiron_x: 13.864 | hardiron_y: -0.818 | hardiron_z:11.786
  
 // Boat RogueWave Oct12_2014  | hardiron_x: -1.545 | hardiron_y: 19.818 | hardiron_z:-25.000
  float hardiron_x = -1.545;
  float hardiron_y = 19.818;
  float hardiron_z = -25.000;
  
  Orientation::setParameters(&accl, &magn, declination, hardiron_x, hardiron_y, hardiron_z);
}

void fixedWidthPrint(float value){
  // Bug: one too many spaces when value === 10
  int characters = 0;
   
  // Count the number of digits before the decimal point
  for(int i = 10000; i > 0; i /= 10){
     if((int)fabs(value) >= i){
       characters ++;
     }
  } 
  
  if(characters == 0){
    characters = 1; // Minimum of 1 character if we print '0'
  }
  
  if(fabs(value) != value){ // Is it negative?
    characters++;
  }
  
  for(int i = 6; i > characters; i--){
    Serial.print(' ');
  }
  Serial.print(value, 2);
}

void loop(void) 
{ 
  float roll;
  float pitch;
  float yaw;
  float heading;

  Orientation::calculate(roll, pitch, yaw, heading);
 
  Serial.print(" | roll:"); fixedWidthPrint(roll);
  Serial.print(" | pitch:"); fixedWidthPrint(pitch);  
  Serial.print(" | yaw:"); fixedWidthPrint(yaw);
  Serial.print(" | heading:"); fixedWidthPrint(heading);
  
  Serial.print('\n');
}
