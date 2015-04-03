// Helpful program to see what the wind sensor is reading.

// This program will print the recieved values, and the corresponding angle,
// to the serial console.

// Pin assignments
const int ROBOSAIL_PIN_WIND = 999; // TODO: Change this to match the correct I/O pin!

void setup() {
  Serial.begin(9600);

  // Set RC receiver on digital input pins
  pinMode(ROBOSAIL_PIN_WIND, INPUT);
}

void loop() {
  // Read commanded (manual) values from the RC reciever
  // pulseIn returns the width of the command pulse in microseconds.
  int windPulseWidth = pulseIn(ROBOSAIL_PIN_WIND, HIGH, 25000);
  // Calculate the wind angle in degrees.
  int windAngle = map(rudderPulseWidth, 1, 1025, 0, 360);
  
  // Print out the values for debug.
  Serial.print("pulse: ");
  Serial.print(windPulseWidth);
  Serial.print("\tangle: ");
  Serial.print(windAngle);

  Serial.print("\n"); // Print a new line

  // add delay to not overwhelm serial monitor
  delay(200);
}

