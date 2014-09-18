/*Serial_Input program reads a Fahrenheit value fron the Serial Monitor 
 converts it to Celsiuse and displays it back to the screen
*/
int F;
int C;
void setup()
{
  Serial.begin(9600); // Sets up communication with the serial monitor
}

void loop()
{
  Serial.println("\nEnter a Fahrenheit value: ");
  while(Serial.available()==0) {} // Loop here until there is a character in the serial monitor
  F = Serial.parseInt();
  Serial.println(F);
  C = convert(F);
  Serial.print("value in Celsius is: ");
  Serial.println(C);
}

int convert(int f)
{
  int c = (f-32) * 5 / 9;
  return(c);
}
