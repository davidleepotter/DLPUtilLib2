/*********************************************************************
*	File : UniqueID.h
*	Desc : Unique ID
*
*	Comment: 
*
*
*
*	Date: 11/18/2002
*********************************************************************/
#include "UtilHeader.h"


//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CUniqueID::CUniqueID(int ntType,CDebugInfo *potDebug,unsigned long utUID)
{
	nType = ntType;
	poDebug = potDebug;
	uUID = utUID;
}

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CUniqueID::CUniqueID(int ntType,CDebugInfo *potDebug,unsigned char btUID)
{
	nType = ntType;
	poDebug = potDebug;
	bUID = btUID;

	uUID = 0;
	nUID[0] = 0;
	nUID[1] = 0;
}

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CUniqueID::CUniqueID(int ntType,CDebugInfo *potDebug,unsigned char btUID0,unsigned char btUID1)
{
	nType = ntType;
	poDebug = potDebug;
	bUID	= 0;
	uUID	= 0;
	nUID[0] = btUID0;
	nUID[1] = btUID1;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CUniqueID::~CUniqueID()
{

}

//////////////////////////////////////////////////////
//
//	Get uValue
//
//////////////////////////////////////////////////////
unsigned long CUniqueID::uGetUniqueID(void)
{

	unsigned long uVal = 0;


	switch(nType)
	{
		case UID_CHAR:
			{
				CStr oStr;
				oStr.Format("%d",bUID);
				uVal = atol(oStr.GetString());			
			}
			break;

		case UID_DCHAR:
			{
				int nVal = (((nUID[0]) * 255)+nUID[1]);
				uVal = nVal;
			}
			break;

		case UID_ULONG:
			{
				uVal = this->uUID;
			}
			break;

	};


return uVal;
}


//////////////////////////////////////////////////////
//
//	Get Byte Val
//
//////////////////////////////////////////////////////
unsigned char CUniqueID::nGetByteVal(int nByteType)
{

	if(nByteType == 0)
	{
		return nUID[0];
	}

	else if(nByteType == 1)
	{
		return nUID[1];
	}

return 0;
}


