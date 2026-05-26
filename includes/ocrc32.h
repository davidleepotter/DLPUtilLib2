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
#if !defined(AFX_OCRC32_H__CE87DDAE_DD0F_45BB_B188_C08DEC65E258__INCLUDED_)
#define AFX_OCRC32_H__CE87DDAE_DD0F_45BB_B188_C08DEC65E258__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COCRC32  
{

	protected: 
		unsigned long crc32_table[256];  // Lookup table array 
		void Init_CRC32_Table();  // Builds lookup table array 
		unsigned long Reflect(unsigned long ref, char ch);  // Reflects CRC bits in the lookup table 
  
	public:
		COCRC32();
		virtual ~COCRC32();
		unsigned long Get_CRC(unsigned char *data, int n_len);  // Creates a CRC from a text string 

};

#endif // !defined(AFX_OCRC32_H__CE87DDAE_DD0F_45BB_B188_C08DEC65E258__INCLUDED_)
