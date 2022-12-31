/*
  MCP466_DigitalPot.h - Library for Microchip MCP466X Digital Potentiometers.
  Created by Paul Krakow, January 21, 2012.
  Released into the public domain.
  
  Note: For this library to work you need to enable I2C communications 
  by calling "Wire.begin();" in the "void setup()" section of your sketch
  
*/

#ifndef MCP466_DigitalPot_h
#define MCP466_DigitalPot_h


#include "Arduino.h"
#include "Wire.h"


class MCP466_DigitalPot
{
  public:
    MCP466_DigitalPot(byte ChipAddress_7bit);
	void mcpWrite(byte Register, word Value);
	word mcpRead(byte Register);
	void mcpUp(byte Register);
	void mcpDown(byte Register);

  private:
	byte _ChipAddress_7bit;

};

#endif