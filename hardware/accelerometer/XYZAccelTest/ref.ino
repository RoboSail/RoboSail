///* CompassBasic rev 6/28/2015
//© 2014-2015 RoboSail
//This program reads the LSM303 Magnetometer/Accelerometer module through 
//the Arduino Mega. It reads the raw values for X, Y, Z from both the 
//Magnetometer and Acclerometer and prints them to the screen.
//
//Use the code to see that the wiring is set up correctly, the range of 
//values coming in from the sensors,and to observe the values as you rotate/move 
//the module.
//
//It also test the libraries that convert the GPS latitude/longitude data (in degrees) 
//into x,y coordinates (in meters).
//The user can determine the information that is printed to the Serial Monitor
//by setting variables in the code to true or false.
//*/
//
//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_LSM303_U.h>
//
///* Assign a unique ID to this sensor at the same time */
//Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
//Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
//
//void displaySensorDetails()
//{
//  sensor_t sensor;
//  mag.getSensor(&sensor);
//  Serial.println("------------------------------------");
//  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
//  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
//  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
//  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
//  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
//  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
//  Serial.println("------------------------------------");
//  Serial.println("");
//  delay(500);
//}
//
//void setup(void) 
//{
//  Serial.begin(115200);
//  Serial.println("Magnetometer + Accelerometer Test"); Serial.println("");
//  
//  /* Initialise the sensor */
//  if(!mag.begin() || !accel.begin())
//  {
//    /* There was a problem detecting the LSM303 ... check your connections */
//    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
//    while(1);
//  }
//    /* Enable auto-gain */
//  mag.enableAutoRange(true);
//  /* Display some basic information on this sensor */
//  displaySensorDetails();
//}
//
//void loop(void) 
//{
//  /* Get a new sensor event */
//  sensors_event_t accel_event;
//  accel.getEvent(&accel_event);
//
//  float ax = accel_event.acceleration.x;
//  float ay = accel_event.acceleration.y;
//  float az = accel_event.acceleration.z;
//
//  /* Display the results (acceleration is measured in m/s^2) */
//  Serial.print("Accel X: "); Serial.print(ax);
//  Serial.print(", Y: "); Serial.print(ay);
//  Serial.print(", Z: "); Serial.print(az);
//  Serial.print("m/s^2  ");
//
//  /* Get a new sensor event */
//  sensors_event_t mag_event;
//  mag.getEvent(&mag_event);
//  
//  float mx = mag_event.magnetic.x;
//  float my = mag_event.magnetic.y;
//  float mz = mag_event.magnetic.z;
//
//  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
//  Serial.print("Mag X: "); Serial.print(mx);
//  Serial.print(", Y: ");   Serial.print(my);
//  Serial.print(", Z: ");   Serial.print(mz);
//  Serial.println("uT  ");
//
//  delay(1000);
//}

