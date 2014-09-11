/*
HMC5883L.cpp - Class file for the HMC5883L Triple Axis Magnetometer Arduino Library.
Copyright (C) 2011 Love Electronics (loveelectronics.co.uk)

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

 WARNING: THE HMC5883L IS NOT IDENTICAL TO THE HMC5883!
 Datasheet for HMC5883L:
 http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/HMC5883L_3-Axis_Digital_Compass_IC.pdf
 http://c48754.r54.cf3.rackcdn.com/HMC5883L.pdf

*/

#include "HMC5883L.h"
#include "Arduino.h"

HMC5883L::HMC5883L()
{
  m_Scale = 0.92;
}

MagnetometerRaw HMC5883L::ReadRawAxis()
{
  uint8_t buffer[6];
  Read(DataRegisterBegin, 6, buffer);
  MagnetometerRaw raw = MagnetometerRaw();
  raw.XAxis = (buffer[0] << 8) | buffer[1];
  raw.ZAxis = (buffer[2] << 8) | buffer[3];
  raw.YAxis = (buffer[4] << 8) | buffer[5];
  return raw;
}

MagnetometerScaled HMC5883L::ReadScaledAxis()
{
  MagnetometerRaw raw = ReadRawAxis();
  MagnetometerScaled scaled = MagnetometerScaled();
  scaled.XAxis = raw.XAxis * m_Scale;
  scaled.ZAxis = raw.ZAxis * m_Scale;
  scaled.YAxis = raw.YAxis * m_Scale;
  return scaled;
}

bool HCMfloatEq(float a, float b){
  float epsilon = 0.1;
  float diff = a - b;
  if(abs(diff) <= epsilon)
    return true;
  return false;
}

int HMC5883L::SetScale(float gauss)
{
	uint8_t regValue = 0x00;
	if(HCMfloatEq(gauss,0.88))
	{
		regValue = 0x00;
		m_Scale = 0.73;
	}
	else if(HCMfloatEq(gauss,1.3))
	{
		regValue = 0x01;
		m_Scale = 0.92;
	}
	else if(HCMfloatEq(gauss,1.9))
	{
		regValue = 0x02;
		m_Scale = 1.22;
	}
	else if(HCMfloatEq(gauss,2.5))
	{
		regValue = 0x03;
		m_Scale = 1.52;
	}
	else if(HCMfloatEq(gauss,4.0))
	{
		regValue = 0x04;
		m_Scale = 2.27;
	}
	else if(HCMfloatEq(gauss,4.7))
	{
		regValue = 0x05;
		m_Scale = 2.56;
	}
	else if(HCMfloatEq(gauss,5.6))
	{
		regValue = 0x06;
		m_Scale = 3.03;
	}
	else if(HCMfloatEq(gauss,8.1))
	{
		regValue = 0x07;
		m_Scale = 4.35;
	}
	else
		return ErrorCode_1_Num;
	// Setting is in the top 3 bits of the register.
	regValue = regValue << 5;
	Write(ConfigurationRegisterB, regValue);
  return 0;
}

void HMC5883L::SetMeasurementMode(uint8_t mode)
{
	Write(ModeRegister, mode);
}

uint8_t HMC5883L::EnsureConnected()
{
	uint8_t data;
	Read(IdentityRegister, 1, &data);

	if(data == IdentityRegisterValue)
		IsConnected = 1;
	else
		IsConnected = 0;

	return IsConnected;
}

void HMC5883L::Write(int address, int data)
{
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

void HMC5883L::Read(int address, int length, uint8_t *buffer)
{
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(address);
  Wire.endTransmission();

  Wire.beginTransmission(HMC5883L_Address);
  Wire.requestFrom(HMC5883L_Address, length);

  if(Wire.available() == length)
  {
	  for(uint8_t i = 0; i < length; i++)
	  {
		  buffer[i] = Wire.read();
	  }
  }
  Wire.endTransmission();
}

char* HMC5883L::GetErrorText(int errorCode)
{
	if(ErrorCode_1_Num == 1)
		return ErrorCode_1;

	return "Error not defined.";
}
