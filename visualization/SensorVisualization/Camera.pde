/*
 * Camera
 *
 * (c) 2016 Brian Dolan, RoboSail
 *
 * This class represents a camera in 3D space, including support for smooth/animated
 * camera movements.
 */
import processing.opengl.*;

public class Camera
{
  PVector location;
  PVector targetLocation;
  PVector lookAtTarget;
//  float rotateXRad = 0;
//  float rotateYRad = 0;

  Camera(PVector location, PVector targetLocation, PVector lookAtTarget)
  {
    this.location = location;
    this.targetLocation = targetLocation;
    this.lookAtTarget = lookAtTarget;
  }
  
  // this must be called at the beginning of the draw() function, before other objects are drawn
  void defineCamera()
  {
    // x to the right, y up, z into the camera
    camera(location.x, location.y, -location.z, lookAtTarget.x, lookAtTarget.y, -lookAtTarget.z, 0, -1, 0);
    scale(1, 1, -1);
    
    animateLocation();
  }

  private void animateLocation()
  {
    location.x += (targetLocation.x - location.x)/16;
    location.y += (targetLocation.y - location.y)/16;
    location.z += (targetLocation.z - location.z)/16;
  }

//  void defineCamera()
//  {
//    // TODO: clean this up
//    translate(width/2, height/2, 0); // set origin to the center of the screen  
//    scale(1, -1, 1); // x to the right, y up, z into the camera
//    scale(1.8, 1.8, 1.8); // make everything bigger
//    translate(-location.x, -location.y, -location.z);
//
//    // handle rotation before translation
//    //beginCamera();
//    //animateRotation();
//    //rotateY(rotateYRad);
//    rotateX(rotateXRad);
//    //endCamera();
//    
//    //rotateXRad += 0.01;
//    //rotateYRad += 0.01;
//    // (we don't rotate about the Z axis, because we always want the camera "level" with the "ground"
//
//    animateLocation();   
//    translate(location.x, location.y, location.z);
//
//    float fov = PI/3.0;
//    float cameraZ = (height/2.0) / tan(PI * 60.0 / 360.0); 
//    float aspect = float(width)/float(height);
//    perspective(fov, aspect, cameraZ/10.0, cameraZ*10.0);
//    println(cameraZ);
//
//    translate(-location.x, -location.y, -location.z);
//  }
//
//  void animateRotation()
//  {
//    float rotateXTarg;
//    float rotateYTarg;
//
//    // the default camera angle is looking directly in the -Z direction
//    PVector defaultCameraLookVector = new PVector(0, 0, -1);
//
//    // first, find the vector from the current camera position to the target to look at
//    PVector lookVector = PVector.sub(location, lookAtTarget);
//
//    // get the projection of the lookVector onto the YZ plane
//    PVector lookVectorYZ = new PVector(0, lookVector.y, lookVector.z);
//
//    // get the projection of the lookVector onto the XZ plane
//    PVector lookVectorXZ = new PVector(lookVector.x, 0, lookVector.z);
//
//    rotateXTarg = PVector.angleBetween(defaultCameraLookVector, lookVectorYZ);
//    rotateYTarg = 0;//PVector.angleBetween(defaultCameraLookVector, lookVectorXZ);//0;
//
//    //map(inputY, INPUTYMIN, INPUTYMAX, -HALF_PI, HALF_PI);
//    //rotateYTarg = PVector.angleBetween(location, targetLocation);
//    //0;//map(inputX, INPUTXMIN, INPUTXMAX, HALF_PI, -HALF_PI);
//
//    // rotate around x: current camera angle -z, 
//
//    //rotateXRad += (rotateXTarg - rotateXRad)/16;
//    //rotateYRad += (rotateYTarg - rotateYRad)/16;
//    rotateXRad = rotateXTarg;
//    rotateYRad = rotateYTarg;
//  }
}

