/*
 * Sailboat
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents a 3D model of a sailboat, including a "floating" arrow to represent
 * wind direction.  It contains a function to draw it with a particular roll, pitch, and yaw.
 */
import processing.opengl.*;

public class Sailboat
{
  private PVector location;
  private Arrow windDirection;
  
  Sailboat(PVector location)
  {
    this.location = location;
    windDirection = new Arrow(
      new PVector(0, 0, 50), // will be replaced with value from sensor
      color(128, 255, 128));
  }
  
  // draws a sailboat model with origin at the center of the hull
  void draw()
  {
    float sailHeight = 30;
    float hullHeight = 10;
    TruncCone truncCone = new TruncCone();
  
    noStroke();
    
    pushMatrix();
    scale(4, 4, 4);
  
    // draw the hull
    fill(color(220, 120, 0));
    pushMatrix();
    translate(0, -hullHeight/2, 0); // sink down into the water by half the hull
    scale(1, 1, 5); // stretch the cylinder into a hull
    truncCone.draw(3, 5, hullHeight, 32);  
    popMatrix();
  
    // draw the sail
    fill(color(240, 240, 240));
    pushMatrix();
    translate(0, hullHeight/2 + hullHeight/3, 0);
    scale(0.2, 1, 1.8); // squish the arrow tip into a sail
    truncCone.draw(10, 0, sailHeight, 32);
    popMatrix();
      
    popMatrix();
  }
  
  // draw the sailboat with roll/pitch/yaw arguments
  // roll, pitch, and yaw in radians
  // TODO: clean this up, move roll/pitch/yaw rotations outside of sailboat class
  void drawRPY(float roll, float pitch, float yaw)
  {
    pushMatrix();
    noStroke();
  
    fill(color(220, 120, 0));
  
    translate(location.x, location.y, location.z);
    
    // roll: rotation about the Z axis
    rotateArb(roll, 0, 0, 1);
    //draw(); // for debug only
  
    // pitch: rotation about the X axis
    rotateArb(pitch, 1, 0, 0);
  
    // yaw: rotation about the Y axis
    //rotateARB(pitch, 0, 1, 0);
  
    draw();
    
    translate(0, 180, 0); // draw the arrow slightly above the sailboat
    windDirection.draw();
  
    popMatrix();
  }
}
