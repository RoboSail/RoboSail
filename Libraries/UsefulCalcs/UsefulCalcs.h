/*
	UsefulCalcs.h - a library for useful sailing functions
	for Diane Brancazio
	Written first by Eric Schneider, 2014-27-07
*/

#ifndef UsefulCalcs_h
#define UsefulCalcs_h

#include "Arduino.h"

class UsefulCalcs
{
  public:
    UsefulCalcs(bool verbose);
    void latLonToUTM(float lat, float lon, float *pos);
  private:
    bool _verbose;
    float pi;
};

#endif