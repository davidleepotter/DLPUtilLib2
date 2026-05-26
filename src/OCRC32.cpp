/*********************************************************************
*	File : OCRC32.cpp 
*	Desc : CRC32 Class
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 

#include "ocrc32.h"
#include <string.h>
#include <stdio.h>

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
COCRC32::COCRC32()
{
	Init_CRC32_Table();
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
COCRC32::~COCRC32()
{

}

//////////////////////////////////////////////////////
//
//	Init the CRC32 table only once
//
//////////////////////////////////////////////////////
void COCRC32::Init_CRC32_Table() 
{// Call this function only once to initialize the CRC table. 

      // This is the official polynomial used by CRC-32 
      // in PKZip, WinZip and Ethernet. 
      unsigned long ulPolynomial = 0x04c11db7; 

      // 256 values representing ASCII character codes. 
      for(int i = 0; i <= 0xFF; i++) 
      { 
            crc32_table[i]=Reflect(i, 8) << 24; 
            
			for (int j = 0; j < 8; j++) 
			{
                  crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0); 
            }

			crc32_table[i] = Reflect(crc32_table[i], 32); 
      } 
} 

//////////////////////////////////////////////////////
//
//	Reflect CRC32 Data
//
//////////////////////////////////////////////////////
unsigned long COCRC32::Reflect(unsigned long ref, char ch) 
{// Used only by Init_CRC32_Table(). 

      unsigned long value(0); 

      // Swap bit 0 for bit 7 
      // bit 1 for bit 6, etc. 
      for(int i = 1; i < (ch + 1); i++) 
      { 
            if(ref & 1) 
			{
                  value |= 1 << (ch - i); 
            }
				  
			ref >>= 1; 
      } 
      return value; 
} 


//////////////////////////////////////////////////////
//
//	Get the CRC
//
//////////////////////////////////////////////////////
unsigned long COCRC32::Get_CRC(unsigned char* data, int tlen) 
{// Pass a data string to this function and it will return the CRC. 

      // Once the lookup table has been filled in by the two functions above, 
      // this function creates all CRCs using only the lookup table. 

      // Be sure to use unsigned variables, 
      // because negative values introduce high bits 
      // where zero bits are required. 

	  if(tlen <= 0)
	  {
		return 0;
	  }

	  if(data == 0)
	  {
		return 0;
	  }

      // Start out with all bits set high. 
      unsigned long ulCRC(0xffffffff); 
   
      // Perform the algorithm on each character 
      // in the string, using the lookup table values. 
      while(tlen--) 
	  {
            ulCRC = (ulCRC >> 8) ^ crc32_table[(ulCRC & 0xFF) ^ *data++]; 
      }
			
			
	  // Exclusive OR the result with the beginning value. 
      return ulCRC ^ 0xffffffff; 
} 

