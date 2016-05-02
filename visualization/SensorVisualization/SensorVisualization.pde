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
 */
// TODO: better-looking boat
//       clean up Camera class - camera() vs. perspective() approach
//       support for Processing 3: textMode(SCREEN), water transparency
//
import processing.serial.*;
import processing.opengl.*;

////////////////////////////////////////
// change this as appropriate:
String comPort = "COM1";
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

  //serialParser = new JoypadSerialParser(this, comPort); // for debugging/writing without equipment
  serialParser = new RoboSailSerialParser(this, comPort); // must be after size()
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
  sailboat.setSailAngle(serialParser.sailAngleDeg);
  sailboat.setRPY(serialParser.rollDeg, serialParser.pitchDeg, serialParser.yawDeg);

  //debugTests();
}

//void debugTests()
//{
//  serialParser.sailAngleDeg = 45;
//
//  serialParser.windAngleDeg++;
//  serialParser.windAngleDeg = ((serialParser.windAngleDeg + 180) % 360) - 180;
//
//  serialParser.sailAngleDeg++;
//  serialParser.sailAngleDeg %= 90;
//
//  serialParser.rudderAngleDeg++;
//  serialParser.rudderAngleDeg = ((serialParser.rudderAngleDeg + 180) % 360) - 180;
//}

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
  hud.add("Wind ang.", serialParser.windAngleDeg, deg);
  hud.add("Rudder ang.", serialParser.rudderAngleDeg, deg);
  hud.add("Sail ang.", serialParser.sailAngleDeg, deg);
  hud.draw();
}
