Serial Monitor README file
© 2014-2016 RoboSail

The Serial Monitor is a window on the computer screen that takes in and 
puts out data in a serial fashion, that is, byte by byte.
The Arduino can receive data from and display data to the Serial Monitor.

These programs demonstrate examples of reading in data as numbers or characters,
and displaying data to the screen as strings of characters.

Arduino recognizes standard characters and numbers, 
Here are some special characaters NL = newline (move the cursor to the next line), 
CR = Carriage Return (move the cursor to the far left)
To get the cursor to the start of a new line, you need both NL and CR

In all programs you need to initialize the Serial Monitor and set up a comunication rate
This is done with the statement in the setup() area
Serial.begin(115200);

The program Hello World demonstrates simply printing a string of characters in quotes
with this statement
  Serial.println("Hello World!");
the ln at the end of println means "print line" and includes Both CR and NL after the print

The program SerialInputNumber demopnstrates how to read in an integer.  
This is useful for entering a choice.
It uses the Arduino function Serial.parseInt() to convert the data string to a 16 bit integer.
x = Serial.parseInt();


