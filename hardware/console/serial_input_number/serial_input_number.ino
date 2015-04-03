// This program reads an integer fron the Serial Monitor 
// and displays it back to the screen

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0) { 
    Serial.print("Enter a whole number, then press enter: ");
    int x = Serial.parseInt();
    
    // Now echo the number back to the console
    Serial.print("You input: ");
    Serial.println(x);
  }
}

