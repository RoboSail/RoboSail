//determine these values using the hardware test programs and fill in the appropriate values
int RUDDER_HIGH = 1900;   //nominal 2000
int RUDDER_LOW = 1100;    //nominal 1000
int SAIL_HIGH = 2000;    //nominal 2000
int SAIL_LOW = 1000;     //nominal 1000
int WIND_HIGH = 1023;    //nominal 1023

// Pin assignments
int WIND_PIN = 7; 
//input pins from receiver
int RUDDER_RC_PIN = 2;
int SAIL_RC_PIN = 3;
// Output pins to the servos
int RUDDER_SERVO_PIN = 8;
int SAIL_SERVO_PIN = 9;
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
