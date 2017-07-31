// ****** Find the values for the transmitter (using program RCReader) 
// ****** and WindSensor (using WindSensorTest)
// ****** and change the values below as needed
#define RUDDER_HIGH 1900  //nominal 2000
#define RUDDER_LOW 1100   //nominal 1000
#define SAIL_HIGH 2000   //nominal 2000
#define SAIL_LOW 1000    //nominal 1000
#define WIND_HIGH 1023   //nominal 1023

// Pin assignments
#define WIND_PIN 7
//input pins from receiver
#define RUDDER_RC_PIN 2
#define SAIL_RC_PIN 3
// Output pins to the servos
#define RUDDER_SERVO_PIN 8
#define SAIL_SERVO_PIN 9
// variables to hold input and output values
int rudderPulseWidth;
int rudderServoOut;
int sailPulseWidth;
int sailServoOut;
//variables for WindSensor
int windAngle = 0;
int windPulseWidth = 0;
int rudderPosition = 0;
int sailPosition = 45;

//create servo objects
Servo rudderServo;
Servo sailServo;
