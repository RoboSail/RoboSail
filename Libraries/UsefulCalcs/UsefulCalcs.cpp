/*
    UsefulCalcs.h - a library for useful sailing functions
    for Diane Brancazio
    Written first by Eric Schneider, 2014-27-07
*/


#include "Arduino.h"
#include "UsefulCalcs.h"

UsefulCalcs::UsefulCalcs(bool verbose){
    _verbose = verbose;
    pi = 3.14159265359;
}


// lat_lon_to_UTM expects to get [latitude, longitude] in decimal degrees
// lat_lon_to_UTM calculates ([x, y], UTM_zone, southhemi)
// float *UTM needs to be a two-element float array
//    the code only ever uses [x, y]
//    See wikipedia on UTM for what UTM_zone and southhemi are for
//    Code by Eric Schneider, originally written for https://github.com/runnedrun/olinoboat
void UsefulCalcs::latLonToUTM(float lat, float lon, float *pos) {

    if (_verbose) {
        Serial.print("\nlatLonToUTM recieved latitude: ");
        Serial.print(lat, 4);
        Serial.print(", longitude: ");
        Serial.println(lon, 4);
    }

    float sm_a = 6378137.0;
    float sm_b = 6356752.314;
    float sm_EccSquared = 6.69437999013e-03;

    float UTMScaleFactor = 0.9996;

    //Calculate the UTM zone
    float zone = floor(((lon + 180.0) / 6.0) + 1.0);

    // Convert lat and lon to radians
    lat = lat/180.0*pi;
    lon = lon/180.0*pi;


    // UTMCentralMeridian
    // Determines the central meridian for the given UTM zone.
    // Inputs:
    //   zone - An integer value designating the UTM zone, range [1,60].
    // Returns:
    //   The central meridian for the given UTM zone, in radians
    //   Range of the central meridian is the radian equivalent of [-177,+177].

    float cmeridian = (-183.0 + (zone * 6.0)) / 180*pi;


    // ArcLengthOfMeridian
    // Computes the ellipsoidal distance from the equator to a point at a
    // given latitude, in meters
    // Inputs:
    //   lat - Latitude of the point, in radians.
    // Globals:
    //   sm_a - Ellipsoid model major axis.
    //   sm_b - Ellipsoid model minor axis.

    float n = (sm_a - sm_b) / (sm_a + sm_b);
    float alpha = ((sm_a + sm_b) / 2.0) * (1.0 + (pow(n, 2.0) / 4.0) + (pow(n, 4.0) / 64.0));
    float beta = (-3.0 * n / 2.0) + (9.0 * pow(n, 3.0) / 16.0) + (-3.0 * pow(n, 5.0) / 32.0);
    float gamma = (15.0 * pow(n, 5.0) / 16.0) + (-15.0 * pow(n, 4.0) / 32.0);
    float delta = (-35.0 * pow(n, 3.0) / 48.0) + (105.0 * pow(n, 5.0) / 256.0);
    float epsilon = (315.0 * pow(n, 4.0) / 512.0);

    float arcLength = alpha * (lat + (beta * sin (2.0 * lat)) + (gamma * sin (4.0 * lat)) + (delta * sin (6.0 * lat)) + (epsilon * sin (8.0 * lat)));


    // MapLatLonToXY
    // Converts a latitude/longitude pair to x and y coordinates in the
    // Transverse Mercator projection. Transverse Mercator is not UTM until scaled
    // Inputs:
    //   lat - Latitude of the point, in radians.
    //   lon - Longitude of the point, in radians.
    //   cmeridian - Longitude of the central meridian to be used, in radians.

    float ep2 = (sq(sm_a) - sq(sm_b)) / sq(sm_b);
    float nu2 = ep2 * pow (cos(lat), 2.0);
    float N = sq(sm_a) / (sm_b * sqrt(1 + nu2));

    float t = tan(lat);
    float t2 = t * t;
    float tmp = (t2 * t2 * t2) - pow(t, 6.0);

    float l = lon - cmeridian;


    // Precalculate coefficients for l**n in the equations below
    // so a normal human being can read the expressions for easting
    // and northing

    float l3coef = 1.0 - t2 + nu2;
    float l4coef = 5.0 - t2 + 9 * nu2 + 4.0 * (nu2 * nu2);
    float l5coef = 5.0 - 18.0 * t2 + (t2 * t2) + 14.0 * nu2 - 58.0 * t2 * nu2;
    float l6coef = 61.0 - 58.0 * t2 + (t2 * t2) + 270.0 * nu2 - 330.0 * t2 * nu2;
    float l7coef = 61.0 - 479.0 * t2 + 179.0 * (t2 * t2) - (t2 * t2 * t2);
    float l8coef = 1385.0 - 3111.0 * t2 + 543.0 * (t2 * t2) - (t2 * t2 * t2);


    // Calculate easting (x)

    float x = N * cos(lat) * l
      + (N / 6.0 * pow(cos(lat), 3.0) * l3coef * pow(l, 3.0))
      + (N / 120.0 * pow(cos(lat), 5.0) * l5coef * pow(l, 5.0))
      + (N / 5040.0 * pow(cos(lat), 7.0) * l7coef * pow(l, 7.0));


    // Calculate northing (y)

    float y = arcLength
      + (t / 2.0 * N * pow(cos(lat), 2.0) * pow(l, 2.0))
      + (t / 24.0 * N * pow(cos(lat), 4.0) * l4coef * pow(l, 4.0))
      + (t / 720.0 * N * pow(cos(lat), 6.0) * l6coef * pow(l, 6.0))
      + (t / 40320.0 * N * pow(cos(lat), 8.0) * l8coef * pow(l, 8.0));


    // Adjust easting and northing for UTM system.
    x = round(x * UTMScaleFactor + 500000.0);
    y = round(y * UTMScaleFactor);
    if (y < 0.0)
        {y = y + 10000000.0;}
    bool southhemi;
    if (lat < 0)
        {southhemi = true;}
    else
        {southhemi = false;}

    if (_verbose) {
      Serial.print("Easting: "); Serial.print(x);
      Serial.print("  --  Northing: "); Serial.println(y);
      Serial.print("UTM Zone: "); Serial.print(zone);
      Serial.print("  --  Southern hemisphere? "); Serial.println(southhemi);
      Serial.print("\n\n");
    }

    // Return the answer
    pos[0] = x; pos[1] = y;
}