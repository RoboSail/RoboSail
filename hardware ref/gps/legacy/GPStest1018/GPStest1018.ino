/*
  GPStest
  Diane Brancazio, Eric Schneider, Andrew Anselmo
  www.robosail.org  --  diane@robosail.org  revised 10/18/14
  
  Reads GPS as latitude and logitude and converts this to X, Y in meters
  Uses the Arduino Mega with the Seeduino Grove Mega SHield - no separate servo driver
  GPS is on Mega HardwareSerial port 1, Connect the GPS TX to Arduino RX1=19, GPS RX to TX1=18

  Many GPS calculations done in library Usefulcalcs.h

  GPS Sensors Output: location in latitude and longitude
  starts as angle in degrees, we convert to radians, then to x, y (meters)
  Adafruit Ultimate GPS Breakout
  Absolute reading
    
*/

//----------------LIBRARIES-------------------

#include <Adafruit_GPS.h>		// for GPS
#include <SoftwareSerial.h>		// for GPS
#include <UsefulCalcs.h>		// for GPS

/*------------FLAGS --------------------------------------*/
bool verbose = false;	//Set to true to enable printlns to the Serial Monitor
bool GPSgood = false;
bool initPrint = false;  // set this flag to true after the starting position has been displayed

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
  float absBoatHeading = 0;       // From compass (degrees)
  float absBoatPositionX = 0;     // From the GPS (meters)
  float absBoatPositionY = 0;     // From the GPS (meters)
  float startingPositionX = 0;    // From the GPS (meters)
  float startingPositionY = 0;    // From the GPS (meters)


  // Things we can calculate
  float boatLat = 0;              // GPS position in all decimal (converted from degrees and minutes)
  float boatLon = 0;
  float absAngleToWaypoint = 0;   // From waypoints + GPS (degrees)
  float relAngleToWaypoint = 0;   // From absATW + compass (degrees)
  float relBoatPositionX = 0;     // X position from starting position (meters)
  float relBoatPositionY = 0;     // Y position from starting position (meters)
  float relDist = 0;
  
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
float currentAngle = 0;

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
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);// Use only "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);	// set update rate to 1 Hz
  useInterrupt(true);

} // End of setup



/****************************** LOOP ***************************/
void loop()
{
  //First get good GPS data and print it out to the serial monitor
 while (GPSgood == false)  // This loop will run on startup and will save position as the (0,0) origin
  {
  Serial.println("looking for GPS"); Serial.print("GPS.fix = "); Serial.println(GPS.fix);
  getAbsBoatPosition();   
  }
  
  if (initPrint == false)  //have just gotten good GPS data, will save starting position and print it out
  {
    startingPositionX = absBoatPositionX; 
    startingPositionY = absBoatPositionY;  
    printStartPosition();
    initPrint = true;    
  }

  if (verbose) Serial.print("\n\n");	// clear space for next run through loop
 
 /*-----------------------MAIN LOGIC---------------------------------*/
  // read GPS
  getAbsBoatPosition(); // read GPS values from GPS breakout
  
  //calculate and display distance and angle you have moved from starting position 
  if (GPSgood)
    {
    currentAngle = angleBetweenTwoPoints(relBoatPositionX, relBoatPositionY, 0, 0);
    currentAngle = normalize0To360(currentAngle);
    relDist = sqrt(sq(relBoatPositionX) + sq(relBoatPositionY));
    Serial.print("GPS is "); Serial.print(relDist); Serial.print("meters at ");
    Serial.print(currentAngle);  Serial.println(" degrees from start (where East = 0 deg)");
    }

 if (timer > millis()) timer = millis(); //reset millis timer if it wraps around, need this for reading GPS

} // End of loop()



/*----------------- FUNCTIONS TO READ ALL SENSORS--------------------*/

void getAbsBoatPosition()  //Read GPS sensor and save 
{
  char *ReadGPSLastNMEA;  // string to hold GPS data
  
  if (GPS.newNMEAreceived()) 
  {
   ReadGPSLastNMEA=GPS.lastNMEA();  // Read the string and set the newNMEAreceived() flag to false
   GPS.parse(ReadGPSLastNMEA);
   if (GPS.fix) GPSgood = true;    // set flag that says GPS has a fix
   
   if (verbose) 
   {
      Serial.print("\n\n");
      Serial.println(ReadGPSLastNMEA);
      Serial.print("\nTime: ");
      Serial.print(GPS.hour, DEC);    Serial.print(':');
      Serial.print(GPS.minute, DEC);  Serial.print(':');
      Serial.print(GPS.seconds, DEC); Serial.print('.');
      Serial.print("Fix: ");          Serial.print((int)GPS.fix);
      Serial.print(" quality: ");     Serial.println((int)GPS.fixquality); 

      if (GPSgood) {
        Serial.print("Location: ");
        Serial.print(GPS.latitude, 4); Serial.print(GPS.lat); //GPS.lat is N or S
        Serial.print(", ");
        Serial.print(GPS.longitude, 4); Serial.println(GPS.lon); //GPS.lon is E or W
      }
     } //end of if(verbose) code
    
    //this code converts the data from the GPS to a more useful format. 
    //Data comes in as degrees and minutes DDMM.MMMM and we want DD.DDDDDD degrees in decimal
    boatLat = (GPS.latitude - floor(GPS.latitude/100.0)*100)/60.0 + floor(GPS.latitude/100);
    boatLon = -((GPS.longitude - floor(GPS.longitude/100.0)*100)/60.0 + floor(GPS.longitude/100));
    // Change lat/lon to UTM with a call to usefulcalcs library
    //  Takes in degree values and send back a pointer to an array with X, Y positions in meters
    calc.latLonToUTM(boatLat, boatLon, pos);  
    absBoatPositionX = pos[0];
    absBoatPositionY = pos[1];
    
//   if (verbose) //**** Calling it al the timein this test code
    {
    Serial.print("-------------Pos x: ");  Serial.print(absBoatPositionX);
    Serial.print("--------------------------Pos y: ");  Serial.println(absBoatPositionY);
    }

    // Make the boat position relative to where it was started
    relBoatPositionX = absBoatPositionX - startingPositionX;
    relBoatPositionY = absBoatPositionY - startingPositionY;
  }
}



/*----------------- USEFUL FUNCTIONS (YOUR LIBRARY)------------------*/

float normalize0To360(float angle)
{
  while (angle < 0){ angle += 360; }
  while (angle > 360){ angle -= 360; }
  return angle;
}

float normalizeN180ToP180(float angle)
{
  while (angle < -180){ angle += 360;  }
  while (angle > 180){ angle -= 360;  }
  return angle;
}

float angleBetweenTwoPoints(float p2X, float p2Y, float p1X, float p1Y){
  // Do trig and convert to degrees
  float angle = atan2((p2Y - p1Y), (p2X - p1X)) * 180 / pi;

  return angle;
}

void printStartPosition()
  {
    Serial.print("\n####Initial boat position in degrees is: Lat "); 
    Serial.print(boatLat, 5); Serial.print(GPS.lat); Serial.print(", Lon ");
    Serial.print(boatLon, 5); Serial.println(GPS.lon); 
    Serial.print("\n****Initial boat position in X,Y plane is: X "); 
    Serial.print(pos[0]);    Serial.print(", Y ");    Serial.print(pos[1]);
  }
  
void useInterrupt(boolean v) 
  {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
      } 
  }
