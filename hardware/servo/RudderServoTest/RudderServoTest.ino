// Test the various positions available on the rudder servo
//   by manually inputting positions

#include <Servo.h>

const int ROBOSAIL_PIN_RUDDER_SERVO = 8;

// create servo object to control a servo
Servo rudderServo;

// variable to store the servo position
int pos = 0;

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(115200);

  // Tell Serial to wait for this long in order to receive a number
  Serial.setTimeout(1000 * 60 * 60);

  rudderServo.attach(ROBOSAIL_PIN_RUDDER_SERVO);

  // Give the user some time to set up the serial monitor
  delay(5000);
}

void loop() {
  Serial.print("Enter desired rudder angle (-90 to 90): ");

  // convert input to integer
  pos = Serial.parseInt();

  // print what was entered
  Serial.println(pos);

  // convert from rudder angle to servo angle
  pos = map(pos, -90, 90, 0, 180);

  // print out what was calculated
  Serial.print("Calculated Servo Value: ");
  Serial.println(pos);
  Serial.println();

  // tell servo to go to position in variable 'pos'
  rudderServo.write(pos);
}

