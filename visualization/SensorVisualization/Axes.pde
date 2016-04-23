/*
 * Axes
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents a 3D set of color X, Y, and Z axes, with text labels.
 */
import processing.opengl.*;

public class Axes
{
  private PFont axisLabelFont;
  private PFont scaleFont;

  private PVector axisXHud;
  private PVector axisYHud;
  private PVector axisZHud;
  private PVector axisOrgHud;
  
  private PVector xVector;
  private PVector yVector;
  private PVector zVector;
 
  // in this array, the x and y components represent the 2-dimensional screen coordinates for the label
  // the z component is used to hold the numerical label 
  // all 3 components as "-1" represents the vector being "uninitialized"
  private final int MAX_SCALE_LABELS = 100;
  private PVector[] scaleLabels;
  
  // the maximum
  private float xMax;
  private float yMax;
  private float zMax;

  // the arguments specifiy the length of the axes displayed 
  Axes(float xMax, float yMax, float zMax)
  {
    axisLabelFont = createFont("Arial", 20);
    scaleFont = createFont("Arial", 12);

    axisXHud = new PVector();
    axisYHud = new PVector();
    axisZHud = new PVector();
    axisOrgHud = new PVector();
    
    scaleLabels = new PVector[MAX_SCALE_LABELS];
    for (int i = 0; i < MAX_SCALE_LABELS; i++)
      scaleLabels[i] = new PVector(-1, -1, -1);
  
    this.xMax = xMax;
    this.yMax = yMax;
    this.zMax = zMax;
  
    xVector = new PVector(xMax, 0, 0);
    yVector = new PVector(0, yMax, 0);
    zVector = new PVector(0, 0, zMax);
  }

  void draw()
  {
    TruncCone truncCone = new TruncCone();
    float radius = 3;
    final float gridIncrement = 20;
    
    pushMatrix();
        
    noStroke();
   
    // x
    pushMatrix();
    fill(color(255, 0, 0));
    rotateZ(-HALF_PI);
    truncCone.draw(radius, radius, xVector.mag(), 8);
    popMatrix();
    
    // y
    pushMatrix();
    fill(color(0, 255, 0));
    truncCone.draw(radius, radius, yVector.mag(), 8);
    popMatrix();
    
    // z
    pushMatrix();
    fill(color(0, 0, 255));
    rotateX(HALF_PI);
    truncCone.draw(radius, radius, zVector.mag(), 8);
    popMatrix();
    
    pushStyle();
    textFont(scaleFont);
    fill(255);
    stroke(127, 127, 127, 60);
  
    pushMatrix();
    scale(1, -1, 1);
  
    // x-y plane
    for (int x = 0; x <= xVector.mag(); x += gridIncrement)
      line(x, 0, 0, x, -yVector.mag(), 0);
    for (int y = 0; y <= yVector.mag(); y += gridIncrement)
      line(0, -y, 0, xVector.mag(), -y, 0);
    
    // x-z plane
    for (int x = 0; x <= xVector.mag(); x += gridIncrement)
      line(x, 0, 0, x, 0, zVector.mag());
    for (int z = 0; z <= zVector.mag(); z += gridIncrement)
      line(0, 0, z, xVector.mag(), 0, z);
    
    // y-z plane
    for (int y = 0; y <= yVector.mag(); y += gridIncrement)
      line(0, -y, 0, 0, -y, zVector.mag());
    for (int z = 0; z <= zVector.mag(); z += gridIncrement)
      line(0, 0, z, 0, -yVector.mag(), z);
  
    int i = 0;
    for (int x = 0; x <= xVector.mag(); x += gridIncrement)
    {
      //text(str(float(x)).substring(0,3), x, 15, 0);
      scaleLabels[i++] = new PVector(screenX(x, 0, 0), screenY(x, 0, 0) + 15, x);
    }
    for (int y = 0; y <= yVector.mag(); y += gridIncrement)
    {
      //text(str(float(y)).substring(0,3), -15, -y, 0);
      scaleLabels[i++] = new PVector(screenX(0, -y, 0) - 15, screenY(0, -y, 0), y);
    }
    for (int z = 0; z <= zVector.mag(); z += gridIncrement)
    {
      //text(str(float(z)).substring(0,3), 0, 15, z);
      scaleLabels[i++] = new PVector(screenX(0, 0, z), screenY(0, 0, z) + 15, z);
    }
    while (i < MAX_SCALE_LABELS)
      scaleLabels[i++] = new PVector(-1, -1, -1);
  
    popMatrix();
      
    popStyle();
    
    // Store the screen X, Y positions for the X, Y, Z and origin
    axisXHud.set(screenX(xVector.x + 10, xVector.y, xVector.z), screenY(xVector.x + 10, xVector.y, xVector.z), 0);
    axisYHud.set(screenX(yVector.x, yVector.y + 10, yVector.z), screenY(yVector.x, yVector.y + 10, yVector.z), 0);     
    axisZHud.set(screenX(zVector.x, zVector.y, zVector.z + 10), screenY(zVector.x, zVector.y, zVector.z + 10), 0);
    axisOrgHud.set(screenX(0,0,0), screenY(0,0,0), 0);
    
    popMatrix();
    
    labelAxes();
  }
  
  private void labelAxes()
  {
    pushStyle();
    
    textAlign(CENTER, CENTER);
    textMode(SCREEN);
  
    // draw the "X", "Y", and "Z" labels at the end of the axes
    fill(255);
    textFont(axisLabelFont);
    text("X", axisXHud.x, axisXHud.y);
    text("Y", axisYHud.x, axisYHud.y);
    text("Z", axisZHud.x, axisZHud.y);
    
    // draw the numerical labels all along the axes
    textFont(scaleFont);
    fill(255);
    stroke(127, 127, 127, 60);
    
    for (int i = 0; i < MAX_SCALE_LABELS; i++)
      if (!(scaleLabels[i].x == -1 && scaleLabels[i].y == -1 && scaleLabels[i].z == -1))
        text(str(scaleLabels[i].z).substring(0, 3), scaleLabels[i].x, scaleLabels[i].y);
        
    popStyle();
  }
}
