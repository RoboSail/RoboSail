// Helpful program to see what the RC tranmitter is sending to the servos,
// and to test and figure out the limits of the attached servos.

// This program will print the recieved values, and the corresponding angle,
// to the serial console.

// WARNING: This program is an example of what NOT to do. It shows that using
// pulseIn() to read PWM results in poor readings, and should be replaced with
// an ISR.

#include <Servo.h>

// Pin assignments
//input pins from receiver
const int ROBOSAIL_PIN_RUDDER_RC = 3;
const int ROBOSAIL_PIN_SAIL_RC = 2;
// Output pins to the servos
const int ROBOSAIL_PIN_RUDDER_SERVO = 8;
const int ROBOSAIL_PIN_SAIL_SERVO = 9;

//create servo objects
Servo rudderServo;
Servo sailServo;

void setup() {
  Serial.begin(115200);

  // Set RC receiver on digital input pins
  pinMode(ROBOSAIL_PIN_RUDDER_RC, INPUT);
  pinMode(ROBOSAIL_PIN_SAIL_RC, INPUT);

  // attaches the servo on pin 9 to the servo object
  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);
  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);
}

void loop() {
  // Read commanded (manual) values from the RC reciever
  // pulseIn returns the width of the command pulse in microseconds.
  int rudderPulseWidth = pulseIn(ROBOSAIL_PIN_RUDDER_RC, HIGH, 25000);
  // Calculate the servo position in degrees.
  int rudderServoOut = map(rudderPulseWidth, 1000, 2000, 0, 180);

  int sailPulseWidth = pulseIn(ROBOSAIL_PIN_SAIL_RC, HIGH, 25000);
  int sailServoOut = map(sailPulseWidth, 1000, 2000, 0, 180);

  // Command the servos to move the the position given by RC
  rudderServo.write(rudderServoOut);
  sailServo.write(sailServoOut);

  // Print out the values for debug.
  Serial.print("rudder, pulse: ");
  Serial.print(rudderPulseWidth);
  Serial.print("\tangle: ");
  Serial.print(rudderServoOut);

  Serial.print("\t\tsail, pulse: ");
  Serial.print(sailPulseWidth);
  Serial.print("\tangle: ");
  Serial.print(sailServoOut);

  Serial.print("\n"); // Print a new line
}

