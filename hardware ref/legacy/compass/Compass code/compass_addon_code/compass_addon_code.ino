/*
  Add this code to yours to get compass readings
  7/28/14
*/

// Compass libraries
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/*----------------------SET UP COMPASS-------------------------------*/
//Assign unique ID to compass sensor
Adafruit_LSM303_Mag_Unified MagComp = Adafruit_LSM303_Mag_Unified(12345);  

/*
/*----------------------USEFUL GLOBAL VARIABLES----------------------*/

  float absBoatHeading = 0;       // From compass (degrees)


  // Things we can calculate
  int absvalRelWind;  // absolute value of wind sensor
  float absWind = 0;              // From wind sensor + compass (degrees)


  bool compass_good = false;

void setup()  
{

/*----------------------SET UP COMPASS-------------------------------*/
  //Compass LMS303 setup code - check for magnetic compass sensor
  int tries = 0;
  // This tries 5 times to connect to the compass, waiting half
  // a second in between each try
  while(!MagComp.begin() && tries < 5) {
    Serial.println("Oops, no LSM303 detected ... Check your wiring!");
    delay(500);
    tries ++;
  }

  if (tries >= 5)
    {compass_good = false;}
  else
    {compass_good = true;}
} // End of setup


void loop()
{
 
 /*----------------------GET THE SENSOR VALUES------------------------*/

  absBoatHeading = getAbsBoatHeading();    //read compass

  /*----------------------CALCULATE THE THINGS YOU NEED----------------*/
  absWind = getAbsWind(relWind, absBoatHeading);

/*-------------------------------------------------------------------*/
/*----------------------PROGRAM LOGIC -----------------*/
/*--------------DECIDE HOW TO SET SAIL/RUDDER BASED ON DATA----------*/

float getAbsBoatHeading()
{
  if (compass_good) {
    sensors_event_t event;
    MagComp.getEvent(&event);
    
    // Calculate the angle of the vector y, x and convert from radians to degrees
//    map(event.magnetic.y, -36, 43, -100, 100);  //find actual values of x,y and use
//    map(event.magnetic.x, -55, 38, -100, 100);  //calibration code if not balanced
  float heading = atan2(event.magnetic.y, event.magnetic.x)*180.0/pi;
    // Then normalize to 0-360
    heading = normalize0To360(heading);

    if (verbose) {
      //Serial.print("y reading: ");        Serial.print(event.magnetic.y);
      //Serial.print("  ---  x reading: "); Serial.println(event.magnetic.x);
      Serial.print("Compass Heading: ");  Serial.println(heading);
    }
    return heading;
  }
}

/*----------------- FUNCTIONS TO CALCULATE WHAT YOU NEED-------------*/

float getAbsWind(float rWind, float aBoatHeading) {
  float aWind = aBoatHeading - rWind;
  aWind = normalizeN180ToP180(aWind);  //fix

  if (verbose) {
    Serial.print("Absolute wind direction: "); Serial.println(aWind);
  }
  return aWind;
}

float getRelativeAngleToWaypoint(float absAngle, float boatHeading) {
float angle = normalizeN180ToP180(boatHeading - absAngle);

  if (verbose) {
    Serial.print("The boat needs to turn "); Serial.print(angle);
    Serial.println(" degrees to point straight at the next waypoint");
  }
  return angle;
}


float normalize0To360(float angle)
{
  while (angle < 0){
    angle += 360;
  }
  while (angle > 360){
    angle -= 360;
  }
  return angle;
}

float normalizeN180ToP180(float angle)
{
  while (angle < -180){
    angle += 360;
  }
  while (angle > 180){
    angle -= 360;
  }
  return angle;
}

