/*
 * RoboSailSerialParser
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * (See description for base class, SerialParser.)  The RobSailSerialParser implementation works for 
 * the "real" output from one of the standard RoboSail Arduino programs used in the RoboSail program.
 */
import processing.serial.*;

public class RoboSailSerialParser extends SerialParser
{
  RoboSailSerialParser(PApplet parent, String comPort)
  {
    super(parent, comPort);
  }
  
  // parse the serial output, in the format of the Arduino program's output, which is
  // int the format:
  // 
  // [joystick x value (0-1023)] [joystick y value (0-1023)] [B1][B2][B3][B4][B5]
  //
  // where B1-B1 are 0 or 1 values representing the state of the 5 buttons 
  // 
  // this function must be "hooked up" to the global serialEvent() function by the main 
  // application, e.g. 
  // void serialEvent(Serial sPort) { serialParser.serialEvent(sPort); }
  void serialEvent(Serial sPort)
  {
    if (sPort != null)
    {
      String errorMsg = "Error parsing serial output.";
      String variable;
      float value;
      
      String string = sPort.readStringUntil(linefeed);
      String[] sensorReadings = splitTokens(string, ",");
      for (int i = 0; i < sensorReadings.length; i++)
      {
        String[] variableAndValue = splitTokens(sensorReadings[i], ":");
        if (variableAndValue.length == 2)
        {
          variable = trim(variableAndValue[0]);
          
          try
          {
            value = float(trim(variableAndValue[1]));
            
            if (variable == "wind angle")
            {
              windAngleDeg = value;
            }
            else if (variable == "rudder angle out")
            {
              rudderAngleDeg = value;
            }
            else if (variable == "sail angle out")
            {
              sailAngleDeg = value;
            }
            else if (variable == "roll")
            {
              rollDeg = value;
            }
            else if (variable == "pitch")
            {
              rollDeg = value;
            }
            else if (variable == "yaw")
            {
              rollDeg = value;
            }
            // else data we don't care about - just keep parsing...          
          }
          catch (Exception e)
          {
            println(errorMsg + " " + e.getMessage());
          }
        }
        else
        {
          println(errorMsg);
        }
      }
    }
  }
}
