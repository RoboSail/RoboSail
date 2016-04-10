


// Pin assignments
//input pins from receiver
const int RUDDER_RC_PIN = 2;
const int SAIL_RC_PIN = 3;

// Output pins to the servos
const int RUDDER_SERVO_PIN = 8;
const int SAIL_SERVO_PIN = 9;

// variables to hold input and output values
int rudderPulseWidth;
int rudderServoOut;
int sailPulseWidth;
int sailServoOut;

//create servo objects
Servo rudderServo;
Servo sailServo;

