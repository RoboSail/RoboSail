/*
  BoatCodeStarter
  Rev 10/12/14 D. Brancazio
  Reads receiver, wind sensor, compass, and GPS and creates commands to drive servo motors.
  Set to manual rudder and sail to function as a pass through for 
  commands from transmitter.
  Set to autonompus rudder and/or sail to try code for a basic sailing maneuver.
  (set manual rudder and sail flags to false)

  Uses the Arduino Mega with the Seeduino Grove Mega SHield - no separate servo driver
  Connect servos to PWM servo ports 8 (rudder) and 9 (sailWinch)
  Wind Sensor is on digital pin 7
  Radio Control receiver is read on pins 11 (rudder), 12  (sail)
  
  Prints commands to serial monitor if asked (verbose ==true)
 Diane Brancazio and Eric Schneider
  www.robosail.org  --  diane@robosail.org

Sensors And Their Data Outputs

Wind Vane (direction wind is coming from) 
  US Digital MA3 Magnetic Encoder
  Data comes in 0-360°, set 0° = in irons, starboard wind is 90° and port wind is -90°
  Relative reading

Rudder
  clockwise = negative
  counter-clockwise = positive
  
Sail
  depends on the wind, but limit is 90 degrees
*/


//----------------LIBRARIES-------------------
#include <Wire.h>				   // for servos
#include <Servo.h>				 // for servos
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <orientation.h>
#include <Math.h>    // For the fabs function in fixedWidthPrint
/* Assign IDs to both the sensors on the LSM303 sensor at the same time */
Adafruit_LSM303_Accel_Unified accl = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified magn = Adafruit_LSM303_Mag_Unified(12345);

#include <Adafruit_GPS.h>		// for GPS
#include <SoftwareSerial.h>		// for GPS
#include <UsefulCalcs.h>		// for GPS

/*------------FLAGS --------------------------------------*/
bool verbose = true;	     //Set to true to enable printlns to the Serial Monitor
bool manualSail = true;    //set to false for autonomous sail
bool manualRudder = true;  //set to false for autonomous rudder
bool GPSgood = false;

/*-- Definitions and Global variables for Servos, Receiver, Compass, GPS, WInd Sensor ---------------*/

Servo rudderServo;      //create servo objects
Servo sailWinchServo; 
#define rudder      11  //input pins from receiver
#define sailwinch   12
#define windpin     7   //Wind sensor MA3 input pin

// Use hardware serial on Mega with GPS TX to Arduino RX1=19 and GPS RX toTX1=18
Adafruit_GPS GPS(&Serial1);  //call this Serial for Uno, Serial1 for Mega
#define GPSECHO false
void useInterrupt(boolean);

UsefulCalcs calc(verbose);  // Initialize the calculator object
// Create  variables for the GPS and set to appropriate values (these are placeholders)
float pos[2];
float lat = 42.36;
float lon = -71.06;


/*----------------------USEFUL GLOBAL VARIABLES----------------------*/
  int rudderIn; // inputs from transmitter, in degrees
  int sailwinchIn;
  int rudderCMD = 0;
  int sailCMD = 45;
  int relWind = 0;    // The Relative Wind direction from wind sensor (degrees)
  int absvalRelWind;  // absolute value of wind direction

  float pi = 3.14159;
  float roll;
  float pitch;
  float yaw;
  float heading;
  float BoatHeading;

  float absBoatHeading = 0;       // From compass (degrees)
  float absBoatPositionX = 0;     // From the GPS (meters)
  float absBoatPositionY = 0;     // From the GPS (meters)
  float startingPositionX = 0;    // From the GPS (meters)
  float startingPositionY = 0;    // From the GPS (meters)
  // Things we can calculate
  float absWind = 0;              // From wind sensor + compass (degrees)
  float absAngleToWaypoint = 0;   // From waypoints + GPS (degrees)
  float relAngleToWaypoint = 0;   // From absATW + compass (degrees)
  // Useful variables
  int GPS_counter = 0;

  uint32_t timer = millis();

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
  Serial.println("BoatCode starting up.  Windvane, Compass, and GPS will be read");
  
/*--------- SetUp Receiver, Servos, Compass, GPS, Wind Sensor ------------------*/

  pinMode(11, INPUT);	  // Set RC receiver on digital input pins
  pinMode(12, INPUT);     // 11 is rudder, 12 is sail
  
  rudderServo.attach(8);
  sailWinchServo.attach(9);   // attaches the servo on pin 9 to the servo object 

  pinMode(windpin, INPUT);		// set Wind sensor pin as digital input
  /* Initialise the sensor */
  if(!accl.begin() || !magn.begin())  // also initalizes mag and accel sensors
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  magn.enableAutoRange(true);  // enable auto-dain on compass magnetometer
  //---------------data for Compass -----------------------
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
  
 // Boat DB  | hardiron_x: -0.500 | hardiron_y: -3.682 | hardiron_z:-13.929
 //Use program hardiron.ino to get the correct values and type in here for your boat
  float hardiron_x = -0.5;
  float hardiron_y = -3.682;
  float hardiron_z = -13.929;
  Orientation::setParameters(&accl, &magn, declination, hardiron_x, hardiron_y, hardiron_z);
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

  if (verbose)
  {
    Serial.print("\n\n\n\n\n\n"); // clear space for next run through loop
  }
 /*----------------------GET THE SENSOR VALUES------------------------*/
  relWind = getRelativeWind();      //read wind sensor

  readTransmitter();  // read RC transmitter via receiver and 
  		      // put reading in rudderIn and sailwinchIn in degrees
  
  Orientation::calculate(roll, pitch, yaw, heading);  // get tilt-compensated compass reading 
    BoatHeading = (360 - heading)+ 90; //translate to FrameofReference with East = 0 and CCW is increasing
  if (BoatHeading > 360) BoatHeading -= 360;
  if (verbose) 
  {Serial.print("\n\nBoat heading is **************************** "); Serial.println(BoatHeading);}
  
  getAbsBoatPosition(); // read GPS values from GPS breakout
  /*----------------------CALCULATE THE THINGS YOU NEED----------------*/
  absAngleToWaypoint = getAbsAngleToWaypoint(waypointsX[currentWaypoint],
        waypointsY[currentWaypoint], absBoatPositionX, absBoatPositionY);
        
//  relAngleToWaypoint = getRelativeAngleToWaypoint(absAngleToWaypoint, absBoatHeading);
 
  calc.latLonToUTM(lat, lon, pos);   // Call to Usefulcalcs library
//  Serial.print("Pos x: ");  Serial.println(pos[0]);
//  Serial.print("Pos y: ");  Serial.println(pos[1]);
//  delay(500);
  if (timer > millis())  timer = millis(); //reset millis tmer if it wraps around


/*--------------------------PROGRAM LOGIC ---------------------------*/
/*--------------DECIDE HOW TO SET SAIL/RUDDER BASED ON DATA----------*/

  if (manualRudder == true) 
  {
    rudderCMD = rudderIn;
  }
  else
  {
    // Sailing Code for rudder
    // assume maximum position of rudder is +-80.  
  
  }
  
  if (manualSail == true) 
  {
    sailCMD = sailwinchIn;
  }
  else
  {
    // Sailing Code for sail
    // assume maximum position of sail is +-90.  
  
  }

  if (verbose)
  {
    Serial.print("     RudderCMD:   ");   Serial.print(rudderCMD); 
    Serial.print("  |        SailCMD:      ");  Serial.println(sailCMD);
  }
   
  //COMMAND THE MOTORS

  // will send input from transmitter or calculated postion 
  //depending on whether it is manual or autonomous
   if (rudderCMD !=99) commandMotor(rudderCMD, rudder);  // will send input from transmitter 
   else Serial.println("Rudder command from Tx error");

   if (sailCMD !=99) commandMotor(sailCMD, sailwinch);
   else Serial.println("Sail command from Tx error");
} // End of loop()

/*----------------- FUNCTION TO COMMAND A MOTOR------------------*/

void commandMotor(int pos, int servoname)
{
  if (servoname == rudder)
  {
    rudderServo.write(pos+90); //servos go 0 to 180
  }
  if (servoname == sailwinch) 
  {
    sailWinchServo.write(pos+90);
  }
}

/*----------------- FUNCTION TO READ THE TRANSMITTER ------------------*/
void readTransmitter()
//read pulse width of each channel from tranmitter thru receiver
//syntax is pulseIn(pin, value, timeout), reads HIGH value, and timeout is in ms
{
  int rCH, sCH;
  rCH = pulseIn(rudder, HIGH, 25000);
  sCH = pulseIn(sailwinch, HIGH, 25000);

  if ((rCH < 1000) || (rCH > 2000)) rCH = 99; //noisy/out of range, send flag,don't change any values
  else rCH = map(rCH, 1000,2000,-80,80);     // put reading in degrees, only go to 80 max
 
  if ((sCH < 1000) || (sCH > 2000)) sCH = 99;  //noisy/out of range, send flag, don't change any values
  else sCH = map(sCH, 1000,2000,0,90);     // put reading in degrees, only go to 80 max
  
  if (verbose)
  {
    Serial.print("Rudder from TxRx: ");   
    Serial.print(rCH); Serial.print("  |  ");
    Serial.print("SailWinch from TxRx:");  
    Serial.println(sCH);
  }
  rudderIn = rCH;
  sailwinchIn = sCH;
  return;
}


/*----------------- FUNCTION TO READ WIND SENSORS--------------------*/

float getRelativeWind()	  //read MA3 PWM output
{
  unsigned long duration;   // used to read wind sensor as pulse length
  int WindDirection;        // degree value of wind from wind sensor
  duration = pulseIn(windpin, HIGH);
  // use a calibration routine to find max pulse width which is typ 1023
  // after calibrating, convert to heading in degrees
  WindDirection= map(duration, 0, 1010, -180, 180);

  // constrain value between 0 and 360 to get rid of noisy outliers
  // use minus sign so values increase in CCW direction
  WindDirection = -min(max(WindDirection, -180), 180);
  
  if (verbose) {
    Serial.print("Relative wind direction:------------------------ ");
    Serial.println(WindDirection);
    Serial.println();
  }
  return WindDirection;
}

/*----------------- FUNCTION TO READ GPS --------------------*/

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



/*----------------- FUNCTIONS TO CALCULATE ANGLES -------------*/

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
