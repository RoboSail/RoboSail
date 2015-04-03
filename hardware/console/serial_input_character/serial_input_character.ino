// This program reads a character fron the Serial Monitor 
// and displays it back to the screen

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0) { 
    // Checks for a character in the serial monitor
    int x = Serial.read();
    
    // Now echo the character back to the console
    Serial.print("Last input: ");
    Serial.println(x);
  }
}

