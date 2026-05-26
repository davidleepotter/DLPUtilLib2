// KeyAuthObj.cpp: implementation of the CKeyAuthObj class.
//
//////////////////////////////////////////////////////////////////////
#include "UtilHeader.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyAuthObj::CKeyAuthObj(CDebugInfo *potDebug)
{
	poDebug = potDebug;
}

CKeyAuthObj::~CKeyAuthObj()
{

}


//Create a Hex Key from a Dec Value
void CKeyAuthObj::vCreateKey(CDebugInfo *poDebug,CStr *poKey,int nVal)
{
	CStr oServerKey;
	oServerKey.Format("%d",nVal);
	COCRC32 oCRC;
	long nServerCRC = oCRC.Get_CRC((unsigned char *)oServerKey.GetString(),oServerKey.GetLength());

	poKey->Format("%d",nServerCRC);
	poKey->vDecToHex();
}

//////////////////////////////////////////////////////
//
//Combine two Hex Keys and Create a Single Hex Key
//
//////////////////////////////////////////////////////
void CKeyAuthObj::vCreateKey(CDebugInfo *poDebug,CStr *poInKey1,CStr *poInKey2,CStr *poOutKey)
{
	CStr oClientKey;
	oClientKey.Format("%s",poInKey2->GetString());
	oClientKey.vHexToDec();
	long nClientKey = atol(oClientKey.GetString());

	CStr oServerKey;
	oServerKey.Format(poInKey1->GetString());
	oServerKey.vHexToDec();
	long nServerKey = atol(oServerKey.GetString());

	long nCombineKey = nServerKey + nClientKey;
	CStr oCombineKey;
	oCombineKey.Format("%d",nCombineKey);
	oCombineKey.vDecToHex();

	COCRC32 oCRC;
	long nServerCRC = oCRC.Get_CRC((unsigned char *)oCombineKey.GetString(),oCombineKey.GetLength());

	poOutKey->Format("%d",nServerCRC);
	poOutKey->vDecToHex();
}

//////////////////////////////////////////////////////
//
//Create a Machine Hardware Hex Key
//
//////////////////////////////////////////////////////
void CKeyAuthObj::vCreateHWKey(CDebugInfo *poDebug,CStr *poKey)
{
	/*
	CStr oMacAdd;
	CStr oHDID;

	CDriveInfo oDrive(poDebug);
	oDrive.nGetHardDriveComputerID();

	CDriveObj *poDrive = (CDriveObj *)oDrive.poDriveArray->pGetAt(0);
	oHDID.Format("%d",poDrive->nHDID);

	CNetFTP::GetAdapterInfo(poDebug,0,&oMacAdd);

	for(int j=0; j<10; j++)
	{
		int nVal = oMacAdd.Find(':',0);
		if(nVal == -1)
		{
			break;
		}
		oMacAdd.EraseChar(nVal);
	}

	CStr oKey;
	oKey.AddString(oMacAdd.GetString());
	oKey.AddString(oHDID.GetString());

	COCRC32 oCRC;
	long nClientKey = oCRC.Get_CRC((unsigned char *)oKey.GetString(),oKey.GetLength());
	
	CStr oCombinedKey;
	oCombinedKey.Format("%x",nClientKey);
	oCombinedKey.vToUpper();

	poKey->Format("%s",oCombinedKey.GetString());
	*/
}

//////////////////////////////////////////////////////
//
// nVaidateHWKey
//
//////////////////////////////////////////////////////
int CKeyAuthObj::nVaidateHWKey(CDebugInfo *poDebug,CStr *poKey,int *pVal)
{
	//Create the Local Hex HWKey
	CStr oHWKey;
	vCreateHWKey(poDebug,&oHWKey);

	//Create the secret Hex Key
	CStr oValueKey;
	vCreateKey(poDebug,&oValueKey,12345);

	//Get the InKey Hex and dec Value
	CStr oInKey;
	oInKey.Format("%s",poKey->GetString());
	oInKey.vHexToDec();
	int nInKeyVal = atol(oInKey.GetString());

	//Create the Combine Key and Key Val
	CStr oCombineKeyVal;
	CStr oCombineKey;
	vCreateKey(poDebug,&oHWKey,&oValueKey,&oCombineKey);
	oCombineKeyVal.Format("%s",oCombineKey.GetString());
	oCombineKeyVal.vHexToDec();
	long nCombineKeyVal = atol(oCombineKeyVal.GetString());

	// Check the combine and inkey dec vals
	if(nInKeyVal == nCombineKeyVal)
	{
		int xxx = 0;
		//return 1;
	}

	// Check the combine andinkey hex values
	if(poKey->CompareNoCase(oCombineKey.GetString()) == 0)
	{
		return 1;
	}

return 0;
}



//////////////////////////////////////////////////////
//
// Key Example
//
//////////////////////////////////////////////////////
#if 0
	CStr oHWKey;
	ARACrypt oCrypt;
	oCrypt.vCreateHWKey(poDebug,&oHWKey);

	//This is created by server and on the client to validate
	CStr oValueKey;
	oCrypt.vCreateKey(poDebug,&oValueKey,12345);

	//This is created by server and given back to client on web page
	CStr oCombineKey;
	oCrypt.vCreateKey(poDebug,&oHWKey,&oValueKey,&oCombineKey);

	int nVal = 0;
	oCrypt.nVaidateHWKey(poDebug,&oCombineKey,&nVal);
#endif


