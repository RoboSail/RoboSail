/*
 * JoypadSerialParser
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * (See description for base class, SerialParser.)  The JoypadSerialParser implementation works for 
 * a special program running on an Arduino with a joystick shield, for debugging/writing the code
 * without the full RoboSail equipment.  (Another child class will parse the the output from one of
 * the standard RoboSail Arduino programs.)
 */
import processing.serial.*;

public class JoypadSerialParser extends SerialParser
{
  private boolean[] lastButtons = {false, false, false, false, false};
  
  JoypadSerialParser(PApplet parent, String comPort)
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
    int joyInputX = 0;
    int joyInputY = 0;
    boolean[] buttons = {false, false, false, false, false};
    
    if (sPort != null)
    {
      String xString = sPort.readStringUntil(space);
      if (xString != null)
      {
        joyInputX = int(trim(xString));
    
        String yString = sPort.readStringUntil(space);
        if (yString != null)
        {
          joyInputY = int(trim(yString));
          
          String buttonsString = sPort.readStringUntil(linefeed);
          if (buttonsString != null)
          {
            for (int i = 0; i < 5; i++)
            {
              lastButtons[i] = buttons[i];
              buttons[i] = (buttonsString.charAt(i) == '0');
            }
          }
        }
      }
    }
      
    // map the joystick values to a given number of degrees around the circle
    rollDeg = mapJoyInput(joyInputX, -90, 90);
    pitchDeg = mapJoyInput(joyInputY, -90, 90);
    yawDeg = 0;
    
    handleButtonInputs(buttons);
  }

  // read the buttons of the joystick shield
  // (TBD)
  private void handleButtonInputs(boolean[] buttons)
  {
    for (int i = 0; i < 5; i++)
    {
      if (buttons[i] == true && lastButtons[i] == false)
      {
        if (i == 0)
        {
  
        }
        if (i == 1)
        {
  
        }
        else if (i == 2)
        {
  
        }
        else if (i == 3)
        {
  
        }
        else if (i == 4)
        {
  
        }
      }
    }
  }
    
  // map the standard joystick values (0-1023) to the given range
  private float mapJoyInput(float value, float start, float stop)
  {
    final int joyInputMin = 0;
    final int joyInputMax = 1023;
    return map(value, joyInputMin, joyInputMax, start, stop);
  }
}
