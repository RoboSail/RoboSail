/* BoatCodewithSwitch
7/24/2015
© 2014-2015 RoboSail
This program has 
automatic sail control at all times
manual/automatic options for rudder using the Sail lever.
In automatic mode is uses the Rudder lever to add or subtract to
the desired windAngle

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

int rudderOut;
int sailOut;
boolean automatic = false;
int desiredAngle = 90;
const int SAILCHANGE = 45;

//create servo objects
Servo rudderServo;
Servo sailServo;

void setup() {
  Serial.begin(115200);
  Serial.println("\nRoboSail BoatCode - XXXXX");  //write program name here
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
  rudderPulseWidth = pulseIn(ROBOSAIL_PIN_RUDDER_RC, HIGH);
  sailPulseWidth = pulseIn(ROBOSAIL_PIN_SAIL_RC, HIGH);
  // Calculate the servo position in degrees.
  rudderServoOut = map(rudderPulseWidth, 1000, 2000, -75, 75);
  sailServoOut = map(sailPulseWidth, 1090, 1900, 0, 90);
  
  // Read values from the WindSensor
  windPulseWidth = pulseIn(ROBOSAIL_PIN_WIND, HIGH);
  // Convert the wind angle to degrees from PWM.  Range -180 to +180
  windAngle = map(windPulseWidth, 0, 1024, 180, -180);
  windAngle = constrain(windAngle, -180, 180);

//**************** your code here ******************
// calculate values for rudderServoOut and sailServoOut in degrees.
  if (sailPulseWidth > 1800)
    {automatic = true; }
   else 
    {automatic = false;}
  if (automatic)
  {
      rudderOut = desiredAngle - windAngle;
      if (rudderOut > 50)
        { rudderOut = 50;}
      if (rudderOut < -50)
        { rudderOut = -50;}
      rudderServoOut = rudderOut;
  }
  //----------------- Set Sail Trim ---------------------------//
  windAngle = abs(windAngle);
  if (windAngle < 45)
    {sailOut = 0; }
  if ((windAngle > 45) && (windAngle <= 135))
    {sailOut = windAngle - 45; }
  if (windAngle > 135)
    {sailOut = 90; }
  sailServoOut = sailOut;

    if (rudderPulseWidth < 1200)
    {
      while (rudderPulseWidth < 1400)
      {
        rudderPulseWidth = pulseIn(ROBOSAIL_PIN_RUDDER_RC, HIGH);
      }
      desiredAngle = desiredAngle + SAILCHANGE;
      if (desiredAngle > 180)
        {desiredAngle = desiredAngle - 360;}
    }
    if (rudderPulseWidth > 1800)
    {
      while (rudderPulseWidth > 1600)
      {
        rudderPulseWidth = pulseIn(ROBOSAIL_PIN_RUDDER_RC, HIGH);
      }
      desiredAngle = desiredAngle - SAILCHANGE;
      if (desiredAngle < 180)
        {desiredAngle = desiredAngle + 360;}
    }
  
  

  if (verbose) {printToMonitor();}
  
//*************create output commands ***********************
  // calculat appropriate values for the servos, then command the servos to move 
  // the Rudder servo motor ranges from 0 to 180 with 90 deg in the center
  // the Sailwinch servo is at ~55 deg when full-in, which we think of as 0 deg,
  //  and ~125 deg when full out, which we think of as 90 deg
  rudderServoOut = map(rudderServoOut, -90, 90, 180, 0);
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
}

