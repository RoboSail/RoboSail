/*
  GPStest
  Diane Brancazio and Eric Schneider
  www.robosail.org  --  diane@robosail.org  revised 10/12/14
  
  Reads GPS as latitude and logitude and converts this to X, Y in meters
  Uses the Arduino Mega with the Seeduino Grove Mega SHield - no separate servo driver
  GPS is on Mega HardwareSerial port 1, Connect the GPS TX to Arduino RX1=19, GPS RX to TX1=18

  Many GPS calculations done in library Usefulcalcs.h
  
Sensors And Their Data Outputs


GPS (location in latitude and longitude) 
  starts as angle in degrees, we convert to radians, then to x, y (meters)
  Adafruit Ultimate GPS Breakout
  Absolute reading
  
Absolute Wind Direction (real direction wind is coming from)
  Set East = 0° range = -180° to +180°
  Data needed: Relative Wind Direction
  
Absolute Angle-to-Waypoint (direction from boat to destination) 
  range is -180° to +180° or 0 to 360°
  Starting point is (x1, y1), destination waypoint is (x2, y2)
  Data needed: GPS location of boat and destination, or just difference
  
Relative Angle-to-Waypoint (angle boat must turn through to be heading at destination) 
  range is -180° to +180° or 0 to 360°
  Data needed: 
    o GPS location of boat and destination (or just difference)
    o Compass heading
    
*/

//----------------LIBRARIES-------------------

#include <Adafruit_GPS.h>		// for GPS
#include <SoftwareSerial.h>		// for GPS
#include <UsefulCalcs.h>		// for GPS

/*------------FLAGS --------------------------------------*/
bool verbose = true;	//Set to true to enable printlns to the Serial Monitor
bool GPSgood = false;

/*-- Definitions and Global variables for Servos, Receiver, Compass, GPS, WInd Sensor ---------------*/

// Use hardware serial on Mega with GPS TX to Arduino RX1=19 and GPS RX toTX1=18
Adafruit_GPS GPS(&Serial1);  //call this Serial for Uno, Serial1 for Mega
#define GPSECHO false
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy ??

/*----------------------USEFUL GLOBAL VARIABLES----------------------*/
  // What is meant by relative and absolute
  //  Relative is relative to the boat. Wind at the nose is 0 degrees,
  //      and it goes from 0-180 on the starboard side and 0 to -180
  //      on the port side. A relative boat direction of 90 means turning
  //      the boat 90 degrees to starboard
  //  Absolute is relative to the world. East is 0, and it increases
  //      In the counter clockwise direction. If the boat is at (0, 0) and
  //      the next waypoint is (-20, 20), the next waypoint is at an absolute
  //      heading of 135 degrees, regardless of which direction the boat is
  //      pointing at the moment

  // Things we can get from the sensors
  int relWind = 0;              // From wind sensor (degrees)
  float absBoatHeading = 0;       // From compass (degrees)
  float absBoatPositionX = 0;     // From the GPS (meters)
  float absBoatPositionY = 0;     // From the GPS (meters)
  float startingPositionX = 0;    // From the GPS (meters)
  float startingPositionY = 0;    // From the GPS (meters)

  // Things we can calculate
  int absvalRelWind;  // absolute value of wind sensor
  float absWind = 0;              // From wind sensor + compass (degrees)
  float absAngleToWaypoint = 0;   // From waypoints + GPS (degrees)
  float relAngleToWaypoint = 0;   // From absATW + compass (degrees)

  // Useful variables
  int GPS_counter = 0;

  uint32_t timer = millis();
  float pi = 3.14159;
// Initialize the calculator object
UsefulCalcs calc(verbose);
// Create the variables for latLontoUTM (these are placeholders)
float pos[2];
float lat = 42.36;
float lon = -71.06;

// Enter the waypoints here
const float waypointsX[] = {0, 5, 10};
const float waypointsY[] = {0, 5, -10};
// The boat will start at the first waypoint, indexes from 0
int currentWaypoint = 0;	// index of current waypoint

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
}

/************************** SETUP ***********************************/
void setup()  
{
  Serial.begin(9600);	// Set baud rate for the serial monitor
  Serial.println("GPStest starting up...");
 

/*----------------------SET UP GPS SENSOR----------------------------*/
  // Adafruit MTK GPS setup code
  GPS.begin(9600); //9600 NMEA is the default baud rate
  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);	// Use only "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);	// set update rate to 1 Hz
  
//  GPS_good = true;		// Check "fix" for this  ??
  useInterrupt(true);

  // This will run on startup and will save that position as the (0,0) origin
  //all variables are global, so we do not pass them to the function or get any in return        
          
    while (GPSgood == false)
//   {
//  {Serial.println("looking for GPS"); Serial.print("GPS.fix = "); Serial.println(GPS.fix);}
//      getAbsBoatPosition(); 
//   }
  
  getAbsBoatPosition(); 
  startingPositionX = absBoatPositionX;
  startingPositionY = absBoatPositionY;
        Serial.println("Initial boat position is Lat: "); 
        Serial.print(GPS.latitude, 5); Serial.print(GPS.lat);
        Serial.print(", ");
        Serial.print(GPS.longitude, 5); Serial.println(GPS.lon);
} // End of setup



/****************************** LOOP ***************************/
void loop()
{
  if (timer > millis())  timer = millis(); //reset millis tmer if it wraps around

  if (verbose) Serial.print("\n\n");	// clear space for next run through loop
 /*----------------------GET THE SENSOR VALUES------------------------*/
  
  getAbsBoatPosition(); // read GPS values from GPS breakout
  /*----------------------CALCULATE THE THINGS YOU NEED----------------*/
  absAngleToWaypoint = getAbsAngleToWaypoint(waypointsX[currentWaypoint],
        waypointsY[currentWaypoint], absBoatPositionX, absBoatPositionY);
        
//  relAngleToWaypoint = getRelativeAngleToWaypoint(absAngleToWaypoint, absBoatHeading);

/*-------------------------------------------------------------------*/
/*----------------------PROGRAM LOGIC -----------------*/
/*--------------DECIDE HOW TO SET SAIL/RUDDER BASED ON DATA----------*/

} // End of loop()



/*----------------- FUNCTIONS TO READ ALL SENSORS--------------------*/

void getAbsBoatPosition()
{
  if (GPS.newNMEAreceived()) 
  {
   
   if (verbose) 
   {
	Serial.print("\n\n");
    	// Print the string and set the newNMEAreceived() flag to false
      Serial.println(GPS.lastNMEA());
    	GPS.parse(GPS.lastNMEA());

      Serial.print("\n read and parsing");
      Serial.print("\nTime: ");
      Serial.print(GPS.hour, DEC);    Serial.print(':');
      Serial.print(GPS.minute, DEC);  Serial.print(':');
      Serial.print(GPS.seconds, DEC); Serial.print('.');

      Serial.print("Fix: ");          Serial.print((int)GPS.fix);
      Serial.print(" quality: ");     Serial.println((int)GPS.fixquality); 

      if (GPS.fix) {
        GPSgood = true;
        Serial.print("Location: ");
        Serial.print(GPS.latitude, 4); Serial.print(GPS.lat); //GPS.lat is N or S
        Serial.print(", ");
        Serial.print(GPS.longitude, 4); Serial.println(GPS.lon); //GPS.lon is E or W
      }
     } //end of if(verbose) code


   float boatLat = (GPS.latitude - floor(GPS.latitude/100.0)*100)/60.0 + floor(GPS.latitude/100);
   float boatLon = -((GPS.longitude - floor(GPS.longitude/100.0)*100)/60.0 + floor(GPS.longitude/100));
    // Change lat/lon to UTM
   //CHECK should result be a pointer?
    calc.latLonToUTM(boatLat, boatLon, pos);  //call to usefulcalcs library
//   if (verbose)
    {
    Serial.print("-------------Pos x: ");  Serial.print(pos[0]);
    Serial.print("--------------------------Pos y: ");  Serial.println(pos[1]);
    }
    // Make the boat position relative to where it was started
    absBoatPositionX = absBoatPositionX - startingPositionX;
    absBoatPositionY = absBoatPositionY - startingPositionY;
  }
//  else Serial.println("GPS data not received");
}





/*----------------- FUNCTIONS TO CALCULATE WHAT YOU NEED-------------*/


float getAbsAngleToWaypoint(float wpX, float wpY, float boatX, float boatY) {
  return angleBetweenTwoPoints(wpX, wpY, boatX, boatY);
}

float getRelativeAngleToWaypoint(float absAngle, float boatHeading) {
  float angle = normalizeN180ToP180(boatHeading - absAngle);

  if (verbose) {
    Serial.print("The boat needs to turn "); Serial.print(angle);
    Serial.println(" degrees to point straight at the next waypoint");
  }
  return angle;
}


/*----------------- USEFUL FUNCTIONS (YOUR LIBRARY)------------------*/

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

float angleBetweenTwoPoints(float p2X, float p2Y, float p1X, float p1Y){
  // Do trig and convert to degrees
  float angle = atan2((p2Y - p1Y), (p2X - p1X)) * 180 / pi;

  if (verbose){
    Serial.print("Angle from point1 to point2: "); Serial.println(angle);
  }

  return angle;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
      } 
}
