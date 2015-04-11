// Test the various positions available on the sail servo
//   by manually inputting positions

#include <Servo.h>

const int ROBOSAIL_PIN_SAIL_SERVO = 9;

// create servo object to control a servo
Servo sailServo;

// variable to store the servo position
int pos = 0;

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(115200);

  // Tell Serial to wait for this long in order to receive a number
  Serial.setTimeout(1000 * 60 * 60);

  sailServo.attach(ROBOSAIL_PIN_SAIL_SERVO);

  // Give the user some time to set up the serial monitor
  delay(5000);
}

void loop() {
  Serial.print("Enter desired sail servo angle (0 to 180): ");

  // convert input to integer
  pos = Serial.parseInt();

  // print what was entered
  Serial.println(pos);

  // tell servo to go to position in variable 'pos'
  sailServo.write(pos);
}

