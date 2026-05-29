/*********************************************************************
*	File :	Pool Base Object.cpp 
*	Desc :	Base Pool Class
*			
*
*
*
*
*	Date: 5/13/2002
*********************************************************************/ 
#include "UtilHeader.h"

unsigned int CPoolBaseObj::gUID = 0;

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CPoolBaseObj::CPoolBaseObj(void *ptrDataObj)
{
	nUID = gUID;
	gUID++;
	nInUse = 0;
	uLifeTime = CTimerManObj::nGetCurrent();
	poObjPtr = ptrDataObj;

}


//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CPoolBaseObj::~CPoolBaseObj()
{
	gUID--;
	vClear();
}

//////////////////////////////////////////////////////
//
//	Clear
//
//////////////////////////////////////////////////////
void CPoolBaseObj::vClear()
{
	nInUse = 0;
	uLifeTime = CTimerManObj::nGetCurrent();
}


//////////////////////////////////////////////////////
//
//	Get Data Ptr
//
//////////////////////////////////////////////////////
void * CPoolBaseObj::poGetDataPtr(void)
{
	return this->poObjPtr;	
}

//////////////////////////////////////////////////////
//
//	Get InUse
//
//////////////////////////////////////////////////////
unsigned int CPoolBaseObj::nGetUniqueID(void)
{
	return this->nUID;
}

//////////////////////////////////////////////////////
//
//	Get InUse
//
//////////////////////////////////////////////////////
int CPoolBaseObj::nGetInUse(void)
{
	return nInUse;
}

//////////////////////////////////////////////////////
//
//	Set InUse
//
//////////////////////////////////////////////////////
void CPoolBaseObj::vSetInUse(int nVal)
{
	nInUse = nVal;
}

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CPoolObj::CPoolObj(CDebugInfo *potDebug) : CNetMutex(potDebug,MUTEX_NO_DEBUG,"PoolObj")
{
	poGlobDebug = potDebug;
	poInUseList = new DLP_CPtrList(potDebug);
	poFreeList = new DLP_CPtrList(potDebug);
}

//////////////////////////////////////////////////////
//
//	Destructor
//  Everything free in derived class
//////////////////////////////////////////////////////
CPoolObj::~CPoolObj()
{
	//Delete the lists, the derived class 
	//deletes all objects in list
	delete poInUseList;
	delete poFreeList;
}

//////////////////////////////////////////////////////
//
//	Get Next Free Obj
//
//////////////////////////////////////////////////////
void * CPoolObj::poGetNextFree(void)
{
	vSetMutex();

	if(poFreeList == 0 || poInUseList == 0)
	{
		poGlobDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Base Pool is Not Allocated\n");
		vClearMutex();
		return 0;
	}

	//Make sure we got all the non in use messages
	DLP_CPtrListItem *plt = poInUseList->poGetHeadItem();
	//Get the first free
	plt = poFreeList->poGetHeadItem();
	if(plt != 0)
	{
		CPoolBaseObj *poObj = (CPoolBaseObj *)plt->pGetPtr();
		poFreeList->nRemoveListItem(plt);
		poInUseList->vAddTail(poObj);
		poGlobDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Allocated Pool Object %d\n",poObj->nUID);
		poObj->nInUse = 1;
		poObj->uLifeTime = CTimerManObj::nGetCurrent();
		vClearMutex();
		return poObj->poGetDataPtr();
	}

	//Need to add some
	CPoolBaseObj *poNewObj = (CPoolBaseObj *)poAddObject();
	poNewObj->nInUse = 1;
	poInUseList->vAddTail(poNewObj);
	poNewObj->uLifeTime = CTimerManObj::nGetCurrent();
	nSize++;
	poGlobDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"No more Objects in POOL So I added one %d\n",nSize);
vClearMutex();
return poNewObj->poGetDataPtr();
}

//////////////////////////////////////////////////////
//
//	Get In Use Size
//
//////////////////////////////////////////////////////
int CPoolObj::nGetInUseSize(void)
{
	return poInUseList->nGetCount();
}

//////////////////////////////////////////////////////
//
//	Get Free Size
//
//////////////////////////////////////////////////////
int CPoolObj::nGetFreeSize(void)
{
	return poFreeList->nGetCount();
}

//////////////////////////////////////////////////////
//
//	Get In Use Size
//
//////////////////////////////////////////////////////
int CPoolObj::nGetSize(void)
{
	return nSize;
}

//////////////////////////////////////////////////////
//
//	Set Size
//
//////////////////////////////////////////////////////
void CPoolObj::vSetSize(int nTSize)
{
	nSize = nTSize;
}

