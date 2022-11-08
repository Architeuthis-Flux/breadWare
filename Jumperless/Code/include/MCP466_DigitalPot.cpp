/*
  MCP466_DigitalPot.cpp - Library for Microchip MCP466X Digital Potentiometers.
  Created by Paul Krakow, January 21, 2012.
  Released into the public domain.
  
  Note: For this library to work you need to enable I2C communications 
  by calling "Wire.begin();" in the "void setup()" section of your sketch
  
*/

#include "Arduino.h"
#include "Wire.h"
#include "MCP466_DigitalPot.h"


/***********************************************************************************************************************/
//        
//        This is the MCP466_DigitalPot class constructor
// 
/***********************************************************************************************************************/

MCP466_DigitalPot::MCP466_DigitalPot(byte ChipAddress_7bit)
{
	_ChipAddress_7bit = ChipAddress_7bit;		// Store the 7-bit I2C address
}




/***********************************************************************************************************************/
//         
//        This is the method to write to a register on the MCP466
// 
/***********************************************************************************************************************/

void MCP466_DigitalPot::mcpWrite(byte Register, word Value)
{
  
  byte firstCommandByte = 0x00;  // Prep the firstCommandByte with 00b for writes
  byte secondCommandByte = 0x00; // Empty data byte
  word tempWord = Value;
  byte tempByte;
  
  // Prep the command bytes to write to the digital potentiometer
  Register *= 16;                // Shift the value of Register to the left by four bits
  firstCommandByte |= Register;  // Load the register address into the firstCommandByte
  tempWord &= 0x0100;            // Clear the top 7 bits and the lower byte of the input value to pick up the two data bits
  tempWord /= 256;               // Shift the top byte of the input value to the right by one byte
  tempByte = byte(tempWord);     // Store the top byte of the input value in a byte sized variable
  firstCommandByte |= tempByte;  // Load the two top input data bits into the firstCommandByte
  tempWord = Value;              // Load the input value into the tempWord
  tempWord &= 0x00FF;              // Clear the top byte
  secondCommandByte = byte(tempWord);  // Store the lower byte of the input value in the secondCommandByte

  
  // Write to the digital potentiometer
  Wire.beginTransmission(_ChipAddress_7bit);   // Begin the I2C transmission
  Wire.write(firstCommandByte);                // Queue up the firstCommandByte
  Wire.write(secondCommandByte);               // Queue up the secondCommandByte
  Wire.endTransmission();                      // Transmit the write command
 
}




/***********************************************************************************************************************/
//         
//        This is the method to read from a register on the MCP466
// 
/***********************************************************************************************************************/

word MCP466_DigitalPot::mcpRead(byte Register)
{
  byte firstCommandByte = 0x0C;    // Prep the firstCommandByte with 11b for reads
  byte firstByte;
  byte secondByte;
  word outputWord = 0x00;
  
  
  // Prep the command bytes to read from the digital potentiometer
  Register *= 16;                // Shift the value of Register to the left by four bits
  firstCommandByte |= Register;  // Load the register address into the firstCommandByte
  
  // Issue the Read command to the digital potentiometer
  Wire.beginTransmission(_ChipAddress_7bit);  // Begin the I2C transmission
  Wire.write(firstCommandByte);              // Queue up the firstCommandByte
  Wire.endTransmission();                    // Transmit the read command
  
  // Collect the two bytes of data from the digital potentiometer
  byte two = 0x02;
  Wire.requestFrom(_ChipAddress_7bit, two);    // Queue up the request for two bytes of data
  firstByte = Wire.read();                  // Queue up the request for the firstByte of data
  secondByte = Wire.read();                 // Queue up the request for the secondByte of data
  Wire.endTransmission();                   // Transmit the request for data
  
  
  // Prepare the outputWord from the two bytes that were read
  outputWord = word(firstByte);            // Load the firstByte into the outputWord
  outputWord *= 256;                        // Shift the firstByte left by one byte
  outputWord += word(secondByte);           // Load the secondByte into the outputWord
  
  return(outputWord);
 
}




/***********************************************************************************************************************/
//         
//        This is the method to increment a register on the MCP466
// 
/***********************************************************************************************************************/

void MCP466_DigitalPot::mcpUp(byte Register)
{
  byte firstCommandByte = 0x04;    // Prep the firstCommandByte with 01b to increment the reg 
  
  // Point the command byte to the correct register
  Register *= 16;                // Shift the value of Register to the left by four bits
  firstCommandByte |= Register;  // Load the register address into the firstCommandByte
  
  // Issue the Increment command to the digital potentiometer
  Wire.beginTransmission(_ChipAddress_7bit);  // Begin the I2C transmission
  Wire.write(firstCommandByte);              // Queue up the firstCommandByte
  Wire.endTransmission();                    // Transmit the increment command
}




/***********************************************************************************************************************/
//         
//        This is the method to decrement a register on the MCP466
// 
/***********************************************************************************************************************/

void MCP466_DigitalPot::mcpDown(byte Register)
{
  byte firstCommandByte = 0x08;    // Prep the firstCommandByte with 10b to decrement the reg 
  
  // Point the command byte to the correct register
  Register *= 16;                // Shift the value of Register to the left by four bits
  firstCommandByte |= Register;  // Load the register address into the firstCommandByte
  
  // Issue the Decrement command to the digital potentiometer
  Wire.beginTransmission(_ChipAddress_7bit);  // Begin the I2C transmission
  Wire.write(firstCommandByte);              // Queue up the firstCommandByte
  Wire.endTransmission();                    // Transmit the decrement command
}