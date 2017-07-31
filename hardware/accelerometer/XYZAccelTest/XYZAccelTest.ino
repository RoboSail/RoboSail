/* XYZAccelTest rev 7/30/2017
© 2014-2017 RoboSail
This program reads the LSM303 Magnetometer/Accelerometer module through 
the Arduino Mega. It reads the raw values for X, Y, Z from both the 
Acclerometer and prints them to the screen.

Use the code to confirm that the module is wired correctly, to view the range of 
values coming in from the sensors, and to observe the values as you rotate/move 
the module.
*/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to this sensor */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

sensors_event_t event; //define event


void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Accelerometer Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop() 
{
  accel.getEvent(&event);   /* Get a new sensor event */ 
  displayValues();
  delay(500);
}

