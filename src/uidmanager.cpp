/*********************************************************************
*	File : UID Manager.h
*	Desc : UIDManager ID
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
CUIDGroup::CUIDGroup(CDebugInfo *potDebug)
{
	nType = -1;
	poDebug = potDebug;
	poUIDName = 0;
	poUIDList = 0;

	uGlobUID	= 0;
	bGlobUID	= 0;
	nGlobUID[0] = 0;
	nGlobUID[1] = 0;
};


//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CUIDGroup::~CUIDGroup()
{
	if(poUIDName != 0)
	{
		delete poUIDName;
	}

	if(poUIDList != 0)
	{
		DLP_CPtrListItem *plt = poUIDList->poGetHeadItem();
		while (plt)
			{
				DLP_CPtrListItem *nextPlt = plt->pGetNext();
				CUniqueID *poUID = (CUniqueID *)plt->pGetPtr();
				delete poUID;
				plt = nextPlt;
			}

		poUIDList->vRemoveAll();
		delete poUIDList;
		poUIDList = 0;
	}

}


//////////////////////////////////////////////////////
//
//	Create Group Item
//
//////////////////////////////////////////////////////
CUIDGroup *CUIDGroup::poCreateGroup(char *name,int ntType)
{

	if(poDebug != 0)
	{
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Created Group %s\n",name);
	}

	nType = ntType;
	poUIDName = new CStr(name);
	poUIDList = new DLP_CPtrList(poDebug);

return this;
}

//////////////////////////////////////////////////////
//
//	vResetUID Group Item
//
//////////////////////////////////////////////////////
void CUIDGroup::vResetUID(void)
{
	uGlobUID	= 0;
	bGlobUID	= 0;
	nGlobUID[0] = 0;
	nGlobUID[1] = 0;
}

//////////////////////////////////////////////////////
//
//	Get Next UID
//
//////////////////////////////////////////////////////
CUniqueID *CUIDGroup::poGetNextUID(void)
{
	if(poUIDList == 0)
	{
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"NULL UID Group int get Next UID %s\n",this->poUIDName->GetString());
		return 0;
	}

	unsigned long uVal = 0;
	unsigned char bUID = 0;
	unsigned char nUID0 = 0;
	unsigned char nUID1 = 0;

	CUniqueID *poUID = 0;
	switch(nType)
	{
		case UID_CHAR:
			{
				bUID = bGlobUID;
				if(bUID >= 255)
				{
					poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Char UID to Large UID %s\n",poUIDName->GetString());
					return 0;
				}
				bGlobUID++;
				
				poUID = new CUniqueID(nType,poDebug,bUID);
				this->poUIDList->vAddTail(poUID);

			}
			break;

		case UID_DCHAR:
			{

				nUID0 = nGlobUID[0];
				nUID1 = nGlobUID[1];


				if(nUID0 >= 254)
				{
					poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"DChar UID to Large UID %s\n",poUIDName->GetString());
					return 0;
				}

				if(nGlobUID[1] >= 254)
				{
					nGlobUID[0]++;
					nGlobUID[1] = 0;
				}
				else
				{
					nGlobUID[1]++;
				}

				poUID = new CUniqueID(nType,poDebug,nUID0,nUID1);
				this->poUIDList->vAddTail(poUID);

			}
			break;

		case UID_ULONG:
			{
				uVal = uGlobUID;

				poUID = new CUniqueID(nType,poDebug,uVal);
				poUIDList->vAddTail(poUID);
				uGlobUID++;
			}
			break;

	};

return poUID;
}


//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CUIDManager::CUIDManager(CDebugInfo *potDebug)
{
	poDebug = potDebug;
	poUIDGroupList = new DLP_CPtrList(poDebug);
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CUIDManager::~CUIDManager()
{

	DLP_CPtrListItem *plt = poUIDGroupList->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CUIDGroup *poGUID = (CUIDGroup *)plt->pGetPtr();
			delete poGUID;
			plt = nextPlt;
		}

	poUIDGroupList->vRemoveAll();
	delete poUIDGroupList;
	poUIDGroupList = 0;

}

//////////////////////////////////////////////////////
//
//	Check for group
//
//////////////////////////////////////////////////////
int CUIDManager::nCheckForDupe(char *name)
{
	CStr oStr(name);
	DLP_CPtrListItem *plt = poUIDGroupList->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CUIDGroup *poGUID = (CUIDGroup *)plt->pGetPtr();
			
			if(oStr.CompareNoCase(poGUID->poUIDName->GetString()) == 0)
			{
				return 1;
			}
			plt = nextPlt;
		}


return 0;
}


//////////////////////////////////////////////////////
//
//	Get Group
//
//////////////////////////////////////////////////////
CUIDGroup * CUIDManager::poGetGroup(char *name)
{
	CStr oStr(name);
	DLP_CPtrListItem *plt = poUIDGroupList->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CUIDGroup *poGUID = (CUIDGroup *)plt->pGetPtr();
			
			if(oStr.CompareNoCase(poGUID->poUIDName->GetString()) == 0)
			{
				return poGUID;
			}
			plt = nextPlt;
		}

poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"0Could Not find UID Group %s\n",name);
return 0;
}

//////////////////////////////////////////////////////
//
//	Get Group
//
//////////////////////////////////////////////////////
CUIDGroup* CUIDManager::poCreateGroup(char *name,int nType)
{

	int nVal = this->nCheckForDupe(name);
	if(nVal == 1)
	{
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Found Dupe UID Group %s\n",name);
		return 0;
	}


	CUIDGroup *poGroup = new CUIDGroup(poDebug);
	poGroup->poCreateGroup(name,nType);

	poUIDGroupList->vAddTail(poGroup);
	poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Added UID Group %s to UID Manager\n",name);

return poGroup;
}


//////////////////////////////////////////////////////
//
//	Get uValue
//
//////////////////////////////////////////////////////
CUniqueID * CUIDManager::poGetNextUniqueID(char *name)
{

	//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Here 1 %s\n",name);

	CUIDGroup *poGroup = 0;
	poGroup = poGetGroup(name);

	if(poGroup == 0)
	{
		return 0;
	}
	CUniqueID *poUID = 0;
	poUID = poGroup->poGetNextUID();

return poUID;
}

//////////////////////////////////////////////////////
//
//	Remove Group
//
//////////////////////////////////////////////////////
void CUIDManager::vRemoveGroup(char *name)
{
	CStr oStr(name);
	DLP_CPtrListItem *plt = poUIDGroupList->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CUIDGroup *poGUID = (CUIDGroup *)plt->pGetPtr();
			
			if(oStr.CompareNoCase(poGUID->poUIDName->GetString()) == 0)
			{

				poUIDGroupList->nRemoveListItem(plt);
				return;
			}
			plt = nextPlt;
		}

poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Could Not remove UID Group %s\n",name);
return;
}

//////////////////////////////////////////////////////
//
//	Reset Group
//
//////////////////////////////////////////////////////
void CUIDManager::vResetGroup(char *name)
{
	CStr oStr(name);
	DLP_CPtrListItem *plt = poUIDGroupList->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CUIDGroup *poGUID = (CUIDGroup *)plt->pGetPtr();
			
			if(oStr.CompareNoCase(poGUID->poUIDName->GetString()) == 0)
			{
				poGUID->vResetUID();
				return;
			}
			plt = nextPlt;
		}

poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Could Not reset UID Group %s\n",name);
return;
}

//////////////////////////////////////////////////////
//
//	Calc DChar
//
//////////////////////////////////////////////////////
unsigned long CUIDManager::uCalcDChar(unsigned char nUID0,unsigned char nUID1)
{
	unsigned long uVal = (((nUID0) * 255)+nUID1);
	return uVal;
}


//////////////////////////////////////////////////////
//
//	Calc DChar
//
//////////////////////////////////////////////////////
void CUIDManager::vCalcDChar(unsigned long uVal,unsigned char *nUID0,unsigned char *nUID1)
{
	unsigned char u1 = 0;
	unsigned char u2 = 0;

	u1 = (unsigned char)(uVal/255);
	u2 = (unsigned char)(uVal - (u1*255));

	*nUID0 = u1;
	*nUID1 = u2;
}

//////////////////////////////////////////////////////
//
//	Remove Group
//
//////////////////////////////////////////////////////
void CUIDManager::vAddToGroup(char *name,CUniqueID *poUID)
{

	//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Here 0 %s\n",name);

	CUIDGroup *poGroup = 0;
	poGroup = poGetGroup(name);

	if(poGroup == 0)
	{
		poGroup = poCreateGroup(name,poUID->nType);
	}

	poGroup->poUIDList->vAddTail(poUID);

}