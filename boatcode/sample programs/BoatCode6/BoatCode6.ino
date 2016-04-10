/* BoatCodeStarterWindOnly rev 5/2/2015
© 2014-2015 RoboSail
This program contains starter code for programming a boat
using information from the Wind Sensor.
For detailed information about getting input from the WindSensor,
input from the RC receiver, and sending output to the Servos,
see these programs: WindSensor and RCPassThroughSimple

Program values are displayed to the Serial Monitor when the
variable "verbose" is set to true.  otherwise set verbose to false.
*/

#include <Servo.h>
boolean verbose = true;  //true calls function for values to be printed to monitor

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

boolean plzTack = false;
boolean matchAngle = false;
int desiredAngle = 45;
int prevRudderPulseWidth = 1500;
int prevRudderPosition = 0;

//create servo objects
Servo rudderServo;
Servo sailServo;

void setup() {
  Serial.begin(115200);
  Serial.println("\nRCPassthroughSimple code - RoboSail");
  // Set RC receiver and WindSensor on digital input pins
  pinMode(ROBOSAIL_PIN_RUDDER_RC, INPUT);
  pinMode(ROBOSAIL_PIN_SAIL_RC, INPUT);
  pinMode(ROBOSAIL_PIN_WIND, INPUT);

  // attach the servos to the proper pins
  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);
  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);
}

void loop() {
  //*********** Read in data from the RC receiver and sensors *********
  // Read the command pulse from the RC receiver
  rudderPulseWidth = pulseIn(ROBOSAIL_PIN_RUDDER_RC, HIGH, 25000);
  sailPulseWidth = pulseIn(ROBOSAIL_PIN_SAIL_RC, HIGH, 25000);
  // Calculate the servo position in degrees.
  rudderServoOut = map(rudderPulseWidth, 1000, 2000, -75, 75);
  sailServoOut = map(sailPulseWidth, 1090, 1900, 0, 90);

  // Read values from the WindSensor
  windPulseWidth = pulseIn(ROBOSAIL_PIN_WIND, HIGH, 25000);
  // Convert the wind angle to degrees from PWM.  Range -180 to +180
  windAngle = map(windPulseWidth, 0, 1024, 180, -180);
  windAngle = constrain(windAngle, -180, 180);

  //**************** your code here ******************
  // calculate values for rudderServoOut and sailServoOut in degrees.

  //**********SAIL*******

  int windAngleSail = abs(windAngle);
  //closeHauled
  if (windAngleSail >= 0 && windAngleSail <= 35) {
    sailServoOut = -5;
  }

  //beamReach
  else if (windAngleSail > 35 && windAngleSail <= 90) {
    sailServoOut = map(windAngleSail, 35, 90, -5, 35);
  }

  //run.
  else if (windAngleSail > 90) {
    sailServoOut = map(windAngleSail, 90, 180, 35, 75);
  }

  //**************Match Angle************
  if (sailPulseWidth > 1840 || matchAngle == true) {
    rudderServoOut = desiredAngle - windAngle;
    if (rudderServoOut > 180) {
     rudderServoOut -= 360; 
    }
    if (rudderServoOut<-180) {
     rudderServoOut += 360; 
    }
    if (rudderServoOut > 50) {
      rudderServoOut = 50;
    }
    
    if (rudderServoOut < -50) {
     rudderServoOut = -50; 
    }

  }


  //********Change desiredAngle*********
  
  
  if (rudderPulseWidth < 1265 && prevRudderPosition > -1) 
  {
    desiredAngle -= 45;
    prevRudderPosition = -1;
  }
  if (rudderPulseWidth > 1800 && prevRudderPosition < 1) 
  {
    desiredAngle += 45;
    prevRudderPosition = 1;
  }
  //prevRudderPulseWidth = rudderPulseWidth;
  if (rudderPulseWidth < 1600 && rudderPulseWidth > 1400) {
   prevRudderPosition = 0; 
  }
  if (desiredAngle > 180) {
    desiredAngle -= 360;
  }
  if (desiredAngle <-180) {
    desiredAngle += 360;
  }
  
    
  if (verbose) {
    printToMonitor();
  }

  //*************create output commands ***********************
  // calculat appropriate values for the servos, then command the servos to move
  // the Rudder servo motor ranges from 0 to 180 with 90 deg in the center
  // the Sailwinch servo is at ~55 deg when full-in, which we think of as 0 deg,
  //  and ~125 deg when full out, which we think of as 90 deg



  /*
  close hauled = -5
  run = 75
  beam = 35


  */
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
  Serial.print("  Desired Angle: ");
  Serial.print(desiredAngle);

  Serial.print("  Rudder, from RC: ");
  Serial.print(rudderPulseWidth);
  Serial.print("  angle out: ");
  Serial.print(rudderServoOut);

  Serial.print("\t Sail, from RC: ");
  Serial.print(sailPulseWidth);
  Serial.print("  angle out: ");
  Serial.print(sailServoOut);

  Serial.print("\n"); // Print a new line
}

