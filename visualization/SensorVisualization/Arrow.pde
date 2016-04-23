/*
 * Arrow
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents a 3D arrow which consists of a "tip" (cone) and stem (cylinder).  It
 * can be drawn along any given vector.
 */
import processing.opengl.*;

public class Arrow
{
  private PVector vector;
  private color c;
  
  Arrow(PVector vector, color c)
  {
    this.vector = vector;
    this.c = c;
  } 
  
  void draw()
  {
    // the drawing routine below will draw the arrow pointing in the -Y direction
    // in order to draw the arrow along the user-specified vector, we must rotate it about a 
    // vector which is orthogonal to the Y (or -Y) direction, and the user-specified vector -
    // this orthogonal vector is found using the "cross product" of the two
    PVector yVector = new PVector(0, 1, 0);
    PVector rotateVector = yVector.cross(vector); 
    
    // rotate about the orthogonal vector we just found, by the appropriate angle
    rotateArb(PVector.angleBetween(vector, yVector) + PI, rotateVector.x, rotateVector.y, rotateVector.z);
  
    drawBasic();    
  }
  
  // this function contains the "basic" code for drawing the arrow (no rotation included)
  private void drawBasic()
  {
    TruncCone truncCone = new TruncCone();
    float arrowLength = vector.mag();
    float tipLength = 30;
    float stemLength = arrowLength - tipLength;

    pushMatrix();
    pushStyle();
    
    fill(c);
    noStroke();
      
    // draw the tip (cone)
    translate(0, -arrowLength, 0);
    truncCone.draw(0, 10, tipLength, 16);
    
    // draw the stem (cylinder)
    translate(0, tipLength, 0);
    truncCone.draw(5, 5, stemLength, 16);
  
    popStyle();
    popMatrix();
  }
}
