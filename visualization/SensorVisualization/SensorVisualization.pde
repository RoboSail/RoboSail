/*
 * SensorVisualization
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * NOTE: This program currently requires Processing version 1.5.1.  Later versions of Processing
 * currently are not supported.
 *
 * This program displays a 3D visualization of a RoboSail sailboat's sensor values, including
 * roll, pitch, yaw, (from the accelerometer) and wind direction (from the wind direction encoder).
 * It reads the values through the serial port, and is made to work with the RoboSail Arduino
 * programs.  It is intended to be used a tool to aid in writing and debugging code on the RoboSail
 * Arduino processor.
 *
 * Angles of the rudder, sail, and wind sensor on the sailboat are expressed like this:
 *
 *            (overhead view)
 *                   /                    ^ 90 degrees
 *            ,,----/-------/             |
 *      bow  <     +      + |  stern      +----> 0 degrees
 *            ``------------+             |
 *                                        v -90 degrees
 */
// TODO: better-looking boat
//       support for Processing 3: textMode(SCREEN), water transparency, axes labelling
//       2D overhead view of angles?
//       read in from recorded file? support for data via ZigBee?
//       show GPS coordinates
//       animate location of sailboat based on GPS coordinates, show historical path
//       show compass direction?
import processing.serial.*;
import processing.opengl.*;

////////////////////////////////////////
// change this as appropriate:
String comPort = "COM7";
////////////////////////////////////////

SerialParser serialParser;
Camera camera;
Axes axes;
Sailboat sailboat;
Extrusion water;
HUD hud;
PImage backgroundImage;

// initialization (called once)
void setup()
{
  size(1024, 768, P3D);

  serialParser = new JoypadSerialParser(this, comPort); // for debugging/writing without equipment
  //serialParser = new RoboSailSerialParser(this, comPort); // must be after size()
  axes = new Axes(300, 200, 300);
  sailboat = new Sailboat(new PVector(150, 0, 150));
  water = new Extrusion(axes.xMax, axes.zMax);
  hud = new HUD();
  camera = new Camera(
    new PVector(250, 200, 500), // initial location
    PVector.add(sailboat.location, new PVector(0, 50, 0))); // look at the sailboat
  backgroundImage = loadImage("bg1024-768.jpg");
}

// drawing function (called for every frame)
void draw()
{
  camera.defineCamera();
  background(backgroundImage);
  lights();
  axes.draw();
  water.draw();
  updateSailboatParameters();
  sailboat.draw();  
  drawHUD();
}

void updateSailboatParameters()
{
  sailboat.setWindDirection(serialParser.windAngleDeg);
  sailboat.setRudderAngle(serialParser.rudderAngleDeg);
  sailboat.setSailAngleLimit(serialParser.sailAngleLimitDeg);
  sailboat.setRPY(serialParser.rollDeg, serialParser.pitchDeg, serialParser.yawDeg);

  debugTests();
}

void debugTests()
{
  serialParser.sailAngleLimitDeg = 45;
  serialParser.windAngleDeg = normalizeAngle(serialParser.windAngleDeg + 1);
  serialParser.rudderAngleDeg = normalizeAngle(serialParser.rudderAngleDeg + 1);
}

void serialEvent(Serial sPort)
{
  // having this as a global function is required when using the Serial class 
  // just pass this along to the serialParser's function
  serialParser.serialEvent(sPort);
}

// draw the "heads-up display"
void drawHUD()
{
  // degree symbol (Unicode encoding)
  String deg = str(char(176));
  
  hud.clear();
  hud.add("Roll", serialParser.rollDeg, deg);
  hud.add("Pitch", serialParser.pitchDeg, deg);
  hud.add("Yaw", serialParser.yawDeg, deg);
  hud.add("Wind", serialParser.windAngleDeg, deg);
  hud.add("Rudder", serialParser.rudderAngleDeg, deg);
  hud.add("Sail limit", serialParser.sailAngleLimitDeg, deg);
  hud.add("X", sailboat.location.x, "");
  hud.add("Z", sailboat.location.z, "");
  hud.draw();
}
