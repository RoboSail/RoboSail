/*
GPS AddonCode
  7/30/14
  put code into appropriate places in your code
  Uses the Arduino Mega
  Diane Brancazio and Eric Schneider
  www.robosail.org  --  www.dianemarie@alum.mit.edu
*/


// GPS libraries
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <UsefulCalcs.h>

// Enter the waypoints here
const float waypointsX[] = {0, 5, 10};
const float waypointsY[] = {0, 5, -10};
// The boat will start at the first waypoint, indexes from 0
int currentWaypoint = 0;	// index of current waypoint


/*----------------------SET UP GPS-----------------------------------*/
// For the Adafruit GPS module with software serial:
// Connections: GPS Power pin to 5V, GPS Ground pin to ground
// GPS TX (transmit) and GPS RX (receive) to pins 3 and 2 on Uno, pins 53 and 53 on Mega
#define gpsTX         13   //GPS communication pins
#define gpsRX         10
// Define a software serial port called gpsSerial, 
//need this even if you are reading  GPS via interrupt 
SoftwareSerial gpsSerial(gpsTX, gpsRX);
Adafruit_GPS GPS(&gpsSerial);
#define GPSECHO false
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

/*----------------------USEFUL GLOBAL VARIABLES----------------------*/
  // What is meant by relative and absolute
  //  Relative is relative to the boat. Wind at the nose is 0 degrees,
  //      and it goes from 0-180 on the starboard side and 0 to -180
  //      on the port side. A relative boat direction of 90 means the boat 
  //      needs to turn 90 degrees to port to point at the Absolute AngletoWayPoint
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
  
  bool GPS_good = false;		//not currently used
  bool wind_good = false;		//not currently used

  
// Initialize the calculator object for GPS library
UsefulCalcs calc(verbose);
// Create the variables for latLontoUTM (these are placeholders)
float pos[2];
float lat = 42.36;  //comment these out when ready to use real numbers
float lon = -71.06;

  if (sizeof(waypointsX) != sizeof(waypointsY)) 
  {
    while(1) 
    {
      Serial.println("Your waypointsX and waypointsY aren't the same length!");
      Serial.println("This would cause problems, so I just won't run");
    }
  }


/*----------------------SET UP GPS SENSOR----------------------------*/
  // Adafruit MTK GPS setup code
  GPS.begin(9600); //9600 NMEA is the default baud rate
  // USe only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // set update rate to 1hz so the code can read the data and print it
  // TODO: try higher if necessary
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);/*  */
  useInterrupt(true);
  // This will run on startup and will save that position as the (0,0) origin
  
  //all variables are global, so we do not pass them to the function or get any in return
  getAbsBoatPosition();
  startingPositionX = absBoatPositionX;
  startingPositionY = absBoatPositionY;

  GPS_good = true;


} // End of setup

// this code is between setup and loop
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
      } 
}

uint32_t timer = millis();




// In Arduino the loop() section is run constantly once power is given
void loop()
{
  if (timer > millis())  timer = millis(); //reset millis tmer if it wraps aroubd

  // // approximately every 2 seconds or so, print out the current stats
  // if (millis() - timer > 2000) 
  //   timer = millis(); // reset the timer

  if (verbose) Serial.print("\n\n");
 /*----------------------GET THE SENSOR VALUES------------------------*/
  
  getAbsBoatPosition(); // read GPS values from GPS breakout
 
  /*----------------------CALCULATE THE THINGS YOU NEED----------------*/
  // absWind = getAbsWind(relWind, absBoatHeading);
  absAngleToWaypoint = getAbsAngleToWaypoint(waypointsX[currentWaypoint],
        waypointsY[currentWaypoint], absBoatPositionX, absBoatPositionY);
 relAngleToWaypoint = getRelativeAngleToWaypoint(absAngleToWaypoint, absBoatHeading);

/*-------------------------------------------------------------------*/
/*----------------------PROGRAM LOGIC -----------------*/
  
} // End of loop()

/*----------------- FUNCTIONS ------------------*/


void getAbsBoatPosition()
{
  if (GPS.newNMEAreceived()) {
   
   if (verbose) {
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
        Serial.print("Location: ");
// ASK: What's difference between GPS.latitude and GPS.lat? lat is just N, lon is W
        Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
        Serial.print(", ");
        Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
      }
   }


   float boatLat = (GPS.latitude - floor(GPS.latitude/100.0)*100)/60.0 + floor(GPS.latitude/100);
   float boatLon = -((GPS.longitude - floor(GPS.longitude/100.0)*100)/60.0 + floor(GPS.longitude/100));
    // Change lat/lon to UTM
   // CHECK should result be a pointer?
    calc.latLonToUTM(boatLat, boatLon, pos);  //result will be in pos[]
  if (verbose) {
  Serial.print("Pos x: ");  Serial.println(pos[0]);
  Serial.print("Pos y: ");  Serial.println(pos[1]); }

    // Make these` boat position relative to where it was started
    absBoatPositionX = pos[0] - startingPositionX;
    absBoatPositionY = pos[1] - startingPositionY;
  }
  else Serial.println("GPS data not received");
}


/*----------------- FUNCTIONS TO CALCULATE WHAT YOU NEED-------------*/

float getAbsWind(float rWind, float aBoatHeading) {
  float aWind = aBoatHeading - rWind;
  aWind = normalizeN180ToP180(aWind);

  if (verbose) {
    Serial.print("Absolute wind direction: "); Serial.println(aWind);
  }
  return aWind;
}

float getAbsAngleToWaypoint(float wpX, float wpY, float boatX, float boatY) {
  return angleBetweenTwoPoints(wpX, wpY, boatX, boatY);
}

float getRelativeAngleToWaypoint(float absAngle, float boatHeading) {
  float angle = normalizeN180ToP180(absAngle - boatHeading);  //reversed from Eric's code

  if (verbose) {
    Serial.print("The boat needs to turn "); Serial.print(angle);
    Serial.println(" degrees to point straight at the next waypoint");
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
