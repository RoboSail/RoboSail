#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
float ax, ay, az, cx, cy, cz;
float xh,yh,ayf,axf;
float pitch, roll, pitch_deg, roll_deg;
float pitch_a, roll_a, pitch_deg_a, roll_deg_a;

double heading;
#define RAD_CONV 57.2957 // 180/Pi, to convert radians to degrees

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Tilt Compensated Compass  Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!accel.begin() || !mag.begin())  // also initalizes mag and accel sensors
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
    
  /* Enable auto-gain */
  mag.enableAutoRange(true);
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t event_a; 
  sensors_event_t event_c; 

  accel.getEvent(&event_a); 
  mag.getEvent(&event_c);
  
  ax = event_a.acceleration.x;
  ay = event_a.acceleration.y;
  az = event_a.acceleration.z;
  cx = event_c.magnetic.x;
  cy = event_c.magnetic.y;
  cz = event_c.magnetic.z;
  
  ay = -ay;
  az = -az;
  cy = -cy;
  cz = -cz;
  
  
  /* Display the results (acceleration is measured in m/s^2) */
  //Serial.print("Xaccel: "); Serial.print(ax); Serial.print("  ");
  //Serial.print("Yaccel: "); Serial.print(ay); Serial.print("  ");
  //Serial.print("Zaccel: "); Serial.print(az); Serial.print("  ");Serial.println("m/s^2 ");
//  Serial.print("X mag: ");  Serial.println(cx);
//  Serial.print("Y mag: ");  Serial.println(cy);
  //Serial.print("X mag      ------------ : ");  Serial.print(cx);
//Serial.print("-------------Y mag  ---------: ");  Serial.println(cy);
//Serial.print("Z mag: ");  Serial.println(cz);

  // Normalize acceleration ameasurements so they range from 0 to 1
  float axnorm = ax/sqrt(ax*ax+ay*ay+az*az);
  float aynorm = ay/sqrt(ax*ax+ay*ay+az*az);             /// 
  float cxnorm = cx/sqrt(cx*cx+cy*cy+cz*cz);
  float cynorm = cy/sqrt(cx*cx+cy*cy+cz*cz);
  float cznorm = cz/sqrt(cx*cx+cy*cy+cz*cz);
//  
  pitch_a = asin(-axnorm);
//    roll_a = asin(-ay);
  roll_a = asin(aynorm/cos(pitch_a));

//  pitch_a = asin(ay/cos(roll_a));
// roll  = atan2(-ay, az);
//  pitch = atan2(ax, sqrt(ay*ay + az*az));
    
roll_deg_a = roll_a *RAD_CONV; roll_deg = roll *RAD_CONV;
pitch_deg_a = pitch_a *RAD_CONV; pitch_deg = pitch *RAD_CONV;

// Serial.print("Pitch --------- : ");  Serial.print(pitch_deg);
//   Serial.print("----------Roll--------: ");  Serial.println(roll_deg);

 
 Serial.print("Axnorm ************ : ");  Serial.print(axnorm);
 Serial.print("  Aynorm ************ : ");  Serial.println(aynorm);
 
 Serial.print("Pitch_a ------- : ");  Serial.print(pitch_deg_a);
  Serial.print("----------Roll_a------: ");  Serial.println(roll_deg_a);
//  xh=cx*cos(pitch)+cy*sin(roll)*sin(pitch)-cz*cos(axf)*sin(ayf);
//  yh=cy*cos(axf)+cz*sin(axf);

//xh=cx*cos(pitch)+cy*sin(pitch)*sin(roll)-cz*cos(roll)*sin(pitch); // from Tim Zaman
//  yh=cy*cos(roll)+cz*sin(roll);

float xcomp=-cy*cos(pitch_a)+cz*sin(pitch_a); //from LSM303DLH p/r/h calc doc
//yh=cy*cos(roll)+cz*sin(roll); // reversed x,y and roll, pitch and is same as above from TZ
  
float ycomp=cy*sin(roll_a)*sin(pitch_a) + cx*cos(roll_a)+cz*sin(roll_a)*cos(pitch_a); //from LSM303DLH p/r/h calc doc
//xh=cy*sin(roll)*sin(pitch) + cx*cos(pitch)-cz*sin(pitch)*cos(roll);  // reversed x,y and roll, pitch
 
 //need to use norm values or not?
//  float xcomp = cxnorm*cos(pitch_a)+cznorm*sin(pitch_a);
//  float ycomp = cxnorm*sin(roll_a)*sin(pitch_a)+cynorm*cos(roll_a)-cznorm*sin(roll_a)*cos(pitch_a);

 Serial.println();
Serial.print("Xheading ------------ : ");  Serial.print(xh);
Serial.print("-------------Yheading---------: ");  Serial.println(yh);
//  heading=atan2((double)yh,(double)xh) * (180 / PI) -90; // angle in degrees
//heading = -(atan2(yh, xh)) * RAD_CONV;


//if    ((xh<0) && (yh<0))   {heading=-heading;}
//else if ((xh<0) && (yh>0))   {heading=-heading;}
//else if((xh>0)  && (yh<0)){heading=-heading;}
//else if(xh>0  && yh>0){heading=360-heading;}
//else if(xh==0 && yh<0){heading=90;}
//else if(xh==0 && yh>0){heading=270;}
  heading = -(atan2(cy, cx)) * RAD_CONV;
  Serial.print("Simple Compass Heading:----------------------------- ");
  Serial.println(heading); Serial.println("\n\n");
  Serial.print("Compensated Compass Heading:--------------------------------------------- ");
  float headingcomp = atan2(ycomp, xcomp) * RAD_CONV;
  Serial.println(headingcomp); Serial.println("\n\n");
//  Serial.print("Compass Heading:----------------------------- ");
//  Serial.println(heading); Serial.println();
//   Serial.print("Compass Heading comp:----------------------------- ");
//  Serial.println(headingcomp); Serial.println(); delay(500);
}
