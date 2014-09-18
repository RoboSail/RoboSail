/*Serial_Input program reads an integer fron the Serial Monitor 
and displays it back to the screen
*/

void setup()
{
  Serial.begin(9600); // Sets up communication with the serial monitor
}

void loop()
{
  if (Serial.available()>0)
 { // Checks for a character in the serial monitor
  int x = Serial.parseInt();
  Serial.println(x);
 }
}

