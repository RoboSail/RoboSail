/* BoatCodeStarterWind+GPS rev 5/16/2015
© 2014-2015 RoboSail
This program contains starter code for programming a boat 
using information from the Wind Sensor and GPS module.
For detailed information about getting input from the WindSensor,
input from the RC receiver, and sending output to the Servos,
see these programs: WindSensor, GPSTest, and RCPassThroughSimple

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

//create servo objects
Servo rudderServo;
Servo sailServo;

void setup() {
  Serial.begin(115200);
  Serial.println("\nRoboSail BoatCode Wind+GPS-5/16");  //write program name/rev here
  // Set RC receiver and WindSensor on digital input pins
  pinMode(ROBOSAIL_PIN_RUDDER_RC, INPUT);
  pinMode(ROBOSAIL_PIN_SAIL_RC, INPUT);
  pinMode(ROBOSAIL_PIN_WIND, INPUT);

  // attach the servos to the proper pins
  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);
  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);

  GPS.begin(9600);  //default baud rate for Adafruit MTK GPS's
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);  //setting for minimum recommended data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  //update rate is 1 Hz
  enableInterrupt(); // activate TIMER0 interrupt, goes off every 1 msec
  while (start_pos_found == false)  //loop code will not start until GPS is ready
  { readGPS();}
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
