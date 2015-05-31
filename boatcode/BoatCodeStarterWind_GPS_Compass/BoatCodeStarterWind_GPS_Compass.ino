/* BoatCodeStarterWind+GPS+Compass rev 5/31/2015
© 2014-2015 RoboSail
This program contains starter code for programming a boat 
using information from the Wind Sensor, GPS module and Compass/Accelerometer.
For detailed information about getting input from the WindSensor, GPS, Compass,
input from the RC receiver, and sending output to the Servos,
see these programs: WindSensor, GPSTest, CompassTest and RCPassThroughSimple

Program values are displayed to the Serial Monitor when the 
variable "verbose" is set to true.  otherwise set verbose to false.

Wind data is displayed in degrees with irons wind = 0, 
and GPS data is calculated and displayed as relative (x,y)
position from start on powerup.
*/
boolean verbose = true;  //true calls function for values to be printed to monitor

#include <Servo.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <UsefulCalcs.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

Adafruit_GPS GPS(&Serial1);
// initialize utility that will convert lat/lon to (x,y) positions in meters
UsefulCalcs calc(false);

// Pin assignments
const int ROBOSAIL_PIN_WIND = 7; 
//input pins from receiver
const int ROBOSAIL_PIN_RUDDER_RC = 2;
const int ROBOSAIL_PIN_SAIL_RC = 3;
// Output pins to the servos
const int ROBOSAIL_PIN_RUDDER_SERVO = 8;
const int ROBOSAIL_PIN_SAIL_SERVO = 9;
// variables to hold input and output values
int rudderPulseWidth;
int rudderServoOut;
int sailPulseWidth;
int sailServoOut;
//variables for WindSensor
int windAngle = 0;
int windPulseWidth = 0;

//variables for GPS
// this will be false until GPS fix is found and starting position saved
bool start_pos_found = false;
int GPSfix;
int GPSqual;
int GPSsat;
// once GPS fix is found, these variables will be updated
float startPositionX = 0;
float startPositionY = 0;
float relPositionX = 0;
float relPositionY = 0;
float pos[2];
float angleFromStart = 0;

// These values will need to be adjusted based on your particular compass.
// Use compassCalibration (in the Orientation library) to determine the correct hard iron calibration.
float hardiron_x = -11.91;
float hardiron_y = 5.05;
float hardiron_z = -1.02;

float roll, pitch, yaw, heading;
// Source: http://www.ngdc.noaa.gov/geomag-web/#igrfwmm
float declination = -14.6067;
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);


//create servo objects
Servo rudderServo;
Servo sailServo;

void setup() {
  Serial.begin(115200);
  Serial.println("\nRoboSail BoatCode Wind+GPS+Compass-5/31");  //write program name/rev here
  // Set RC receiver and WindSensor on digital input pins
  pinMode(ROBOSAIL_PIN_RUDDER_RC, INPUT);
  pinMode(ROBOSAIL_PIN_SAIL_RC, INPUT);
  pinMode(ROBOSAIL_PIN_WIND, INPUT);

  // attach the servos to the proper pins
  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);
  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);
  
  //Set Up GPS and wait for fix on position
  GPS.begin(9600);  //default baud rate for Adafruit MTK GPS's
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);  //setting for minimum recommended data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  //update rate is 1 Hz
  enableInterrupt(); // activate TIMER0 interrupt, goes off every 1 msec
  while (start_pos_found == false)  //loop code will not start until GPS is ready
  { readGPS();}
  
  // Set up Compass and check that it is connected
  mag.enableAutoRange(true);
    if(!mag.begin() || !accel.begin()) //Initialize the sensor
    {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("No LSM303 Compass detected ... Check your wiring!");
    while(1); }
}

void loop() {
//*********** Read in data from the RC receiver and sensors *********
  // Read the command pulse from the RC receiver
  rudderPulseWidth = pulseIn(ROBOSAIL_PIN_RUDDER_RC, HIGH, 25000);
  sailPulseWidth = pulseIn(ROBOSAIL_PIN_SAIL_RC, HIGH, 25000);
  // Calculate the servo position in degrees.  Calibrate the 1st 2 values in map as necessary
  rudderServoOut = map(rudderPulseWidth, 1000, 2000, -75, 75);
  sailServoOut = map(sailPulseWidth, 1090, 1900, 0, 90);
  
  // Read values from the WindSensor
  windPulseWidth = pulseIn(ROBOSAIL_PIN_WIND, HIGH, 25000);
  // Convert the wind angle to degrees from PWM.  Range -180 to +180
  windAngle = map(windPulseWidth, 0, 1023, 180, -180);
  windAngle = constrain(windAngle, -180, 180);

  // Read position from the GPS
  readGPS();  //puts values in pos array
  
  //Read heading and tilt from the Compass
  readCompass();

//**************** your code here ******************
// calculate values for rudderServoOut and sailServoOut in degrees.




  if (verbose) {printToMonitor();}
  
//*************create output commands ***********************
  // calculat appropriate values for the servos, then command the servos to move 
  // the Rudder servo motor ranges from 0 to 180 with 90 deg in the center
  // the Sailwinch servo is at ~55 deg when full-in, which we think of as 0 deg,
  //  and ~125 deg when full out, which we think of as 90 deg
  rudderServoOut = map(rudderServoOut, -90, 90, 0, 180);
  rudderServo.write(rudderServoOut);
  sailServoOut = map(sailServoOut, 0, 90, 55, 125);
  sailServo.write(sailServoOut);
  
} //end of loop()


// Function to Print out all values for debug.
void printToMonitor()
{
  Serial.print("Wind Angle: ");
  Serial.print(windAngle);
 
  Serial.print("  Rudder, from RC: ");
  Serial.print(rudderPulseWidth);
  Serial.print("  angle out: ");
  Serial.print(rudderServoOut);

  Serial.print("\t Sail, from RC: ");
  Serial.print(sailPulseWidth);
  Serial.print("  angle out: ");
  Serial.print(sailServoOut);

  Serial.print("\n"); // Print a new line
  Serial.print("Fix: "); Serial.print(GPSfix);
  Serial.print(" quality: "); Serial.print(GPSqual);
  Serial.print(" satellites: "); Serial.println(GPSsat);
  Serial.print("x = "); Serial.print(relPositionX);
  Serial.print("   y = "); Serial.print(relPositionY);
  Serial.print("  angle from start = "); Serial.println(angleFromStart);
      
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print(", Pitch: "); Serial.print(pitch);
  Serial.print(", Yaw: "); Serial.print(yaw);
  Serial.print(", Heading: "); Serial.println(heading);
  Serial.println();
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  GPS.read(); // reads char (if available) into internal buffer in GPS object
}

// function to enable TIMER0 interrupt for GPS
void enableInterrupt() {
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

void readGPS()  //gets GPS data, pareses it, and returns (x,y) position in meters in array called pos[]
{
if (GPS.newNMEAreceived())
  {
    char* LastNMEA; // declare pointer to GPS data
    LastNMEA = GPS.lastNMEA(); // read the string and set the newNMEAreceived() flag to false
    if (!GPS.parse(LastNMEA)) 
    {
      return; // failed to parse a sentence (was likely incomplete) so just wait for another
    }
    Serial.println("\nNew data from GPS");
    GPSfix = GPS.fix;  //put parsed data in variables for printing
    GPSqual = GPS.fixquality;
    GPSsat = GPS.satellites;
    if (GPS.fix)
    {
      if (start_pos_found)
      {
        // take in lat/lon degree values and return (x,y) in meters in pos array
        calc.latLonToUTM(GPS.latitudeDegrees, GPS.longitudeDegrees, pos);
        
        // calculate the boat position relative to where it was started
        relPositionX = pos[0] - startPositionX;
        relPositionY = pos[1] - startPositionY;
        angleFromStart = atan2(relPositionY, relPositionX) * 180 / 3.14;
        while (angleFromStart < 0){ angleFromStart += 360; }
        while (angleFromStart > 360){ angleFromStart -= 360; }

      }
      else // starting position not yet found but there is a fix
      { 
        // take in lat/lon degree values and return (x,y) in meters in pos array
        calc.latLonToUTM(GPS.latitudeDegrees, GPS.longitudeDegrees, pos);
        startPositionX = pos[0];
        startPositionY = pos[1];
        
        Serial.println("Starting position found!");
        Serial.print("x = "); Serial.print(startPositionX);
        Serial.print("   y = "); Serial.println(startPositionY);
        Serial.println();
        
        start_pos_found = true;
      }
    }
  }
}

void readCompass()  //reads Compass to get heading and tilt
{
  float ax, ay, az, mx, my, mz;
  float my_adj, mx_adj;
  float Pi = 3.1415926;

   /* Get a new sensor event */
  sensors_event_t accel_event;
  accel.getEvent(&accel_event);
  sensors_event_t mag_event;
  mag.getEvent(&mag_event);

  /* Invert X so that when when X, Y, or Z is pointed down, it has a positive reading. */
  ax = -accel_event.acceleration.x;
  ay = accel_event.acceleration.y;
  az = accel_event.acceleration.z;
  
  /* Adjust for hard iron effects */
  mx = mag_event.magnetic.x - hardiron_x;
  my = mag_event.magnetic.y - hardiron_y;
  mz = mag_event.magnetic.z - hardiron_z;

  /* Invert Y and Z axis so that when X, Y, or Z is pointed towards Magnetic North they get a positive reading. */
  my = -my;
  mz = -mz;
  
  roll = atan2(ay,az);
  pitch = atan(-ax/sqrt(pow(ay,2)+pow(az,2)));
  
  my_adj = mz*sin(roll) - my*cos(roll);
  mx_adj = (mx*cos(pitch) + my*sin(pitch)*sin(roll) + mz*sin(pitch)*cos(roll));
  
  yaw = atan2(my_adj,mx_adj);
  
  roll = roll * 180/Pi;
  pitch = pitch * 180/Pi;
  yaw = yaw * 180/Pi;
  
  heading = yaw + declination;
  
  if (heading >= 360) {
    heading -= 360;
  } else if (heading < 0) {
    heading += 360;
  }
  
}
