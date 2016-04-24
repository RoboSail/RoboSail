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

const int windAngleIncrement = 30;
int rudderPulseWidth;
int rudderServoOut;
int sailPulseWidth;
int sailServoOut;
bool automatic=false;
//variables for WindSensor
int windAngle = 0;
int desWindAngle=180;
int windPulseWidth = 0;
int i = 0;
int rudderAngle = 0;
boolean waitForInput = true;

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
//input of abs value
//--------------------------------------------------------------------
//Decide Auto
//Serial.print(sailPulseWidth);
if(sailPulseWidth<1300){
//RC Position
  automatic=false;
  //Serial.println(" False");
}
//Else automatic
else{
  // If entering automatic mode, change desired wind angle to actual wind angle.
  if (automatic == false) {
    desWindAngle = windAngle;
  }
  automatic=true;
  //Serial.println("True");
}

if(automatic==true){
  Serial.print("Rudder Pulse Width ");
  Serial.println(rudderPulseWidth);
  // Detect joystick right input
  if (waitForInput) {
    if (rudderPulseWidth>1700) {
      desWindAngle += windAngleIncrement;
      desWindAngle = (desWindAngle>180) ? desWindAngle-360 :desWindAngle;
      waitForInput = false;
      //Serial.println("right");
    }
    
    // detect joystick left input
    if (rudderPulseWidth<1250)
    {//input delay for both input 
      //delay(500);
      desWindAngle -= windAngleIncrement;
      desWindAngle = (desWindAngle<-180) ? desWindAngle+360 :desWindAngle;
      //Serial.println("left");
      waitForInput = false;
    }
  } else {
    if (rudderPulseWidth >= 1300 && rudderPulseWidth <= 1550) {
      waitForInput = true;
    }
  }

  rudderAngle = (desWindAngle-windAngle);
  if (rudderAngle > 180) {
    rudderAngle -= 360;
  }

  if (rudderAngle < -180) {
    rudderAngle += 360;
  }
  
  rudderServoOut = rudderAngle/4;  
}


// Sail Control = always automatic
  windAngle = abs(windAngle);
  if(windAngle<45){
    sailServoOut=20;
  } else{
   sailServoOut=(windAngle*2/3-10);
  }

 
//---------------------------------------------------------------------
    if (verbose) {printToMonitor();}
  
//*************create output commands ***********************
  // calculate appropriate values for the servos, then command the servos to move 
  // the Rudder servo motor ranges from 0 to 180 with 90 deg in the center
  // the Sailwinch servo is at ~55 deg when full-in, which we think of as 0 deg,
  //  and ~125 deg when full out, which we think of as 90 deg
  rudderServoOut = map(rudderServoOut, -90, 90, 0, 180);
  rudderServo.write(rudderServoOut);
  sailServoOut = map(sailServoOut, 0, 90, 45, 125);
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

