/*
 * HUD
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents a "heads-up display" for showing a 2D table of numbers (with labels and 
 * units of measure) on a 3D display.
 */
import processing.opengl.*;

public class HUD
{
  private PFont font;
  private ArrayList<LabelledValue> labelledValues;
  
  HUD()
  {
    font = createFont("Arial", 20);
    labelledValues = new ArrayList<LabelledValue>();
  }
  
  // add a labelled value to the list in the display
  void add(String label, float value, String units)
  {
    labelledValues.add(new LabelledValue(label, value, units));
  }
  
  // clear the displayed labelled value list
  void clear()
  {
    labelledValues.clear();
  }

  void draw()
  {
    int border = 10;
    int w = 225;
    int h = 50;
    int x, y;
    color c;

    pushStyle();
    fill(color(255));
    textMode(SCREEN);
    
    x = width - border - w;
    y = 0;
        
    textFont(font);   // Set the text font
    for (int i = 0; i < labelledValues.size(); i++)
    {
      y += border;
      
      // TODO: get the "box" drawing working again
      // (this broke after switching the Camera class from using perspective() to using 
      // the simpler camera() function)
      //noFill();
      //c = color(255);
      //stroke(c);
      //rect(x, y, w, h);
      //fill(c);
      
      LabelledValue v = labelledValues.get(i);
      text(v.label + " (" + v.units + "): " + String.format("%.2f", v.value), x + border, y + 25);
      
      y += h;
    }
    popStyle();
  }
}

// this simple class is used in the HUD class for storing the labelled values
public class LabelledValue
{
  LabelledValue(String label, float value, String units)
  {
    this.label = label;
    this.value = value;
    this.units = units;
  }
  
  String label;
  float value;
  String units;
}
