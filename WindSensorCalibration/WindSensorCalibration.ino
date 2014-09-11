/*
 Read and calibrate MA3 magnetic digital encoder
 Reads a PWM digital input on pin 7, prints the result to the serial monitor 
 USe this to find the range of values from the sensor, then map it to degrees
 Prints value from sensor and calculated heading so dead spots can be identified
 */

int ma3Pin = 7;
//int reading= 0;
int heading=0;
unsigned long duration;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600); // set serial communication at 9600 baud
  pinMode(ma3Pin, INPUT);   // make the encoder's pin an input
}

// the loop routine runs over and over again forever:
void loop() {
  // read the MA3 encoder
   duration = pulseIn(ma3Pin, HIGH);
  
  // print out the reading
  Serial.print("Reading from sensor: "); Serial.print(duration); Serial.print("  |  ");
  heading = (360 - map(duration, 0, 1010, 0, 360))-180;  //convert to heading in degrees
  Serial.print("Wind direction: ");
  Serial.println(heading);
  delay(500);        // delay in between reads for stability
}



