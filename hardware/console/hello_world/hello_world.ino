// Print "Hello World!" to the console

void setup() {
  // Sets up communication with the serial monitor
  Serial.begin(9600);
}

void loop() {
  Serial.println("Hello World!");
  delay(1000);
}

