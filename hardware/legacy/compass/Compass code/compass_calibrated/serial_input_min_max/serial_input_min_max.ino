 /*Serial_Input program reads an integer fron the Serial Monitor 
and displays it back to the screen
*/
#define MIN  250 // this is the 'minimum' pulse length count for boat servos
#define MAX  500 // this is the 'maximum' pulse length count for boat servos

void setup()
{
  Serial.begin(9600); // Sets up communication with the serial monitor
}

void loop()
{
  Serial.println("Enter a number between 0 and 32768: ");
  while(Serial.available()==0) {} // Loop here until there is a character in the serial monitor
//  if (Serial.available()>0) // Checks for a character in the serial monitor
//  {
  int x = Serial.parseInt();
  Serial.println(x);
//  }
}

