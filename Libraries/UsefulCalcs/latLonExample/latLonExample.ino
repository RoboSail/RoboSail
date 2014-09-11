#include <UsefulCalcs.h>

// Initialize the calculator object
bool verbose = true;
UsefulCalcs calc(verbose);

// Create the variables you need (these are placeholders)
float pos[2];
float lat = 42.36;
float lon = -71.06;


void setup() {
  Serial.begin(9600);
}


void loop() {
  // Call the calculator object
  calc.latLonToUTM(lat, lon, pos);
  Serial.print("Pos x: ");  Serial.println(pos[0]);
  Serial.print("Pos y: ");  Serial.println(pos[1]);
  delay(500);
}
