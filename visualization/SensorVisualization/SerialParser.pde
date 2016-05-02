/*
 * SerialParser
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This is an abstract base class for classes that "decode" data from the serial port, sent from 
 * an Arduino (or other device) for roll, pitch, yaw, wind direction, and/or other sensor values.  
 */
import processing.serial.*;

public abstract class SerialParser
{
  protected Serial sPort;
  
  protected final int linefeed = 10;
  protected final int space = 32;
  protected final int comma = 44;
  
  // parsed, in degrees
  public float rollDeg;
  public float pitchDeg;
  public float yawDeg;
  public float windAngleDeg;
  public float rudderAngleDeg;
  public float sailAngleDeg;

  SerialParser(PApplet parent, String comPort)
  {
    sPort = new Serial(parent, comPort, 9600);
    //sPort = new Serial(this, Serial.list()[0], 9600);
    sPort.bufferUntil(linefeed);
  }
  
  // parse the serial output, in the format of the Arduino program's output
  // 
  // this function must be "hooked up" to the global serialEvent() function by the main 
  // application, e.g. 
  // void serialEvent(Serial sPort) { serialParser.serialEvent(sPort); }
  abstract void serialEvent(Serial sPort);
}
