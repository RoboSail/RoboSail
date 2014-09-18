/*
  Reads all sensors and attempts to command boat to sail Close haul and tack
  8/24/14 rev 9/9/14 to move pieces of code around
  Uses the Arduino Mega with the Seeduino Grove Mega SHield - no separate servo driver
  Connect servos to PWM servo ports 8 (rudder) and 9 (sailWinch)
  GPS is on Mega HardwareSerial port 1, Connect the GPS TX to Arduino RX1=19, GPS RX to TX1=18
  Wind Sensor i on digital pin 7
  Compass is on SCL, SDA
  Radio Control receiver is read on pins 11 (rudder), 12  (sail)
  prints commands to serial monitor if asked (verbose ==true)
  Many GPS calculations done in library Usefulcalcs.h
 Diane Brancazio and Eric Schneider
  www.robosail.org  --  diane@robosail.org


Sensors And Their Data Outputs

Wind Vane (direction wind is coming from) 
  US Digital MA3 Magnetic Encoder
  Data comes in 0-360°, set 0° = in irons, starboard wind is 90° and port wind is -90°
  Relative reading

Compass (direction boat is pointing)
  Data comes in 0 to 360°, expect North is 0° but orient sensor so East = 0° 
  Adafruit LSM303 Accelerometer and Compass Breakout
  Absolute reading

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
    
Rudder
  clockwise = negative
  counter-clockwise = positive
  
Sail
  depends on the wind, but limit is 90 degrees
*/

//----------------LIBRARIES-------------------
#include <Wire.h>				// for servos
#include <Servo.h>				// for servos
#include <Adafruit_Sensor.h>	// for comnpass
#include <Adafruit_LSM303_U.h>	// for comnpass
#include <Adafruit_GPS.h>		// for GPS
#include <SoftwareSerial.h>		// for GPS
#include <UsefulCalcs.h>		// for GPS

/*------------FLAGS --------------------------------------*/
bool verbose = true;	//Set to true to enable printlns to the Serial Monitor
bool manualSail = true; //set to false for autonomous sail
bool manualRudder = true;  //set to false for autonomous rudder

/*-- Definitions and Global variables for Servos, Receiver, Compass, GPS, WInd Sensor ---------------*/

Servo rudderServo;  //create servo objects
Servo sailWinchServo; 
#define rudder      11  //input pins from receiver
#define sailwinch   12
Adafruit_LSM303_Mag_Unified MagComp = Adafruit_LSM303_Mag_Unified(12345);  // unique ID
#define windpin       7   	//Wind sensor MA3 input pin
unsigned long duration;		// used to read wind sensor
int WindDirection;

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
  int rudderIn; // inputs from transmitter, in degrees
  int sailwinchIn;
  int rudderCMD = 0;
  int sailCMD = 45;

  bool RC_good = false;			//not currently used
  bool compass_good = false;
  bool GPS_good = false;		//not currently used
  bool wind_good = false;		//not currently used
uint32_t timer = millis();
  #define ironsRange    45
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
  Serial.println("BoatCode_0825 starting up, beginning intialization...");
  if (sizeof(waypointsX) != sizeof(waypointsY)) {
    while(1) {
      Serial.println("Your waypointsX and waypointsY aren't the same length!");
      Serial.println("This would cause problems, so I just won't run");
    }
  }

/*--------- SetUp Receiver, Servos, Compass, GPS, Wind Sensor ------------------*/

  pinMode(rudderIn, INPUT);	  // Set RC receiver on digital input pins
  pinMode(sailwinchIn, INPUT);
  RC_good = true;	//need to test for this?
  
  rudderServo.attach(8);
  sailWinchServo.attach(9);   // attaches the servo on pin 9 to the servo object 

  //Compass LMS303 setup code - check for magnetic compass sensor
  int tries = 0;
  // This tries 5 times to connect to the compass, wait half sec in between each try
  while(!MagComp.begin() && tries < 5) {
    Serial.println("Oops, no LSM303 detected ... Check your wiring!");
    				// need better way to indicate
    delay(500);
    tries ++;
  }

  if (tries >= 5)     {compass_good = false;}
  else    {compass_good = true;}

/*----------------------SET UP GPS SENSOR----------------------------*/
  // Adafruit MTK GPS setup code
  GPS.begin(9600); //9600 NMEA is the default baud rate
  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);	// Use only "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);	// set update rate to 1 Hz
  
  GPS_good = true;		// Check "fix" for this  ??
  useInterrupt(true);

  // This will run on startup and will save that position as the (0,0) origin
  //all variables are global, so we do not pass them to the function or get any in return
  getAbsBoatPosition();
  startingPositionX = absBoatPositionX;
  startingPositionY = absBoatPositionY;


  pinMode(windpin, INPUT);		// set Wind sensor pin as digital input
  wind_good = true;

} // End of setup



/****************************** LOOP ***************************/
void loop()
{
  if (timer > millis())  timer = millis(); //reset millis tmer if it wraps aroubd

  if (verbose) Serial.print("\n\n");	// clear space for next run through loop
 /*----------------------GET THE SENSOR VALUES------------------------*/
  
  getAbsBoatPosition(); // read GPS values from GPS breakout
  readTransmitter();    // read RC transmitter via receiver and 
  						// will put reading in rudderIn and sailwinchIn in degrees

  absBoatHeading = getAbsBoatHeading();    //read compass

  relWind = getRelativeWind();      //read wind sensor
 
  /*----------------------CALCULATE THE THINGS YOU NEED----------------*/
  absWind = getAbsWind(relWind, absBoatHeading);
  absAngleToWaypoint = getAbsAngleToWaypoint(waypointsX[currentWaypoint],
        waypointsY[currentWaypoint], absBoatPositionX, absBoatPositionY);
//  relAngleToWaypoint = getRelativeAngleToWaypoint(absAngleToWaypoint, absBoatHeading);

/*-------------------------------------------------------------------*/
/*----------------------PROGRAM LOGIC -----------------*/
/*--------------DECIDE HOW TO SET SAIL/RUDDER BASED ON DATA----------*/

  if (manualRudder == true) 
      {rudderCMD = rudderIn;}
  else
    // close Haul Code for rudder
    // assume maximum position of rudder is +-80.  
    // to turn upwind rudder comand has opposite sign of relWind
    // Greater than +-135 push rudder full towards sail 
    // Within +-45 leave rudder alone (may want to add bear off code)
    // in reach zones create a linear function with slope -80/90
  {
    if (relWind <= -135) 
      rudderCMD = 80;
    if ((relWind > -135) && (relWind < -45)) 
      rudderCMD = (-.89 * relWind) - 40;
    if ((relWind >= -45) && (relWind <= 45))
      rudderCMD = 0;
    if ((relWind > 45) && (relWind < 135)) 
      rudderCMD = (-.89 * relWind) + 40;
    if (relWind >= 135) 
      rudderCMD = -80;
  }
  if (verbose)
    {Serial.print("rudderCMD = "); Serial.println(rudderCMD);}
  
  if (manualSail == true) 
      {sailCMD = sailwinchIn;}
  else
  {
  // close Haul Code for sail
    // assume maximum position of sail is +-90.  
    // to turn upwind rudder comand has opposite sign of relWind
    // Greater than +-135 push rudder full towards sail 
    // Within +-45 leave rudder alone (may want to add bear off code)
    // in reach zones create a linear function with slope -80/90// trim sail based on rudder position   
  // set to "full in' if within 45 of upwind and "full out" if within 45 of downwind
  // When the sail is in between make a linear relationship
  // between the sail setting and the wind
    absvalRelWind = abs(relWind);
    if (absvalRelWind < 45)
      sailCMD = 0;    
    if ((absvalRelWind >= 45) && (absvalRelWind < 135))
      sailCMD = absvalRelWind - 45;
    if (absvalRelWind >= 135)
      sailCMD = 90;
  }
   if (verbose)
     {Serial.print("SailCMD = "); Serial.println(sailCMD);}

//COMMAND THE MOTORS

// will send input from transmitter or calculated postion 
//depending on whether it is manual or autonomous
commandMotor(rudderCMD, rudder);  
commandMotor(sailCMD, sailwinch);

} // End of loop()

/*----------------- FUNCTION TO COMMAND A MOTOR------------------*/

void commandMotor(int pos, int servoname)
 {
  if (servoname == rudder) rudderServo.write(pos+90); //servos go 0 to 180
  if (servoname == sailwinch) sailWinchServo.write(pos+90);
}

/*----------------- FUNCTION TO READ THE TRANSMITTER ------------------*/
void readTransmitter()
//read pulse width of each channel from tranmitter thru receiver
//syntax is pulseIn(pin, value, timeout), reads HIGH value, and timeout is in ms
{
  int rCH, sCH;
  rCH = pulseIn(rudder, HIGH, 25000);
if ((rCH < 1000) || (rCH > 2000)) return; //noisy/out of range, don't change any values
  rCH = map(rCH, 1000,2000,-80,80);     // put reading in degrees, only go to 80 max
 
  sCH = pulseIn(sailwinch, HIGH, 25000);
if ((sCH < 1000) || (sCH > 2000)) return;
    sCH = map(sCH, 1000,2000,0,90);
if (verbose)
  {
  Serial.print("Rudder from transmitter: ");   Serial.println(rCH);
  Serial.print("SailWinch from Transmitter:");  Serial.println(sCH);
  }
 rudderIn = rCH;
 sailwinchIn = sCH;
 return;
}


/*----------------- FUNCTIONS TO READ ALL SENSORS--------------------*/

float getRelativeWind()	  //read MA3 PWM output
{
  duration = pulseIn(windpin, HIGH);
  // use a calibration routine to find max pulse width which is typ 1023
  // after calibrating, convert to heading in degrees
  WindDirection= map(duration, 0, 1010, -180, 180);

  // constrain value between 0 and 360 to get rid of noisy outliers
  // use minus sign so values increase in CCW direction
  WindDirection = -min(max(WindDirection, -180), 180);
  
  if (verbose) {
    Serial.print("Relative wind direction:--------------------- ");
    Serial.println(WindDirection); }
  return WindDirection;
}

float getAbsBoatHeading()	//this compass code all needs to be tilt compensated
{
  if (compass_good) {
    sensors_event_t event;
    MagComp.getEvent(&event);
    
    // Calculate the angle of the vector y, x and convert from radians to degrees
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
   //CHECK should result be a pointer?
    calc.latLonToUTM(boatLat, boatLon, pos);  //call to usefulcalcs library
  Serial.print("Pos x: ");  Serial.println(pos[0]);
  Serial.print("Pos y: ");  Serial.println(pos[1]);

    // Make the boat position relative to where it was started
    absBoatPositionX = absBoatPositionX - startingPositionX;
    absBoatPositionY = absBoatPositionY - startingPositionY;
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
