/*********************************************************************
*	File :	StringPool.cpp 
*	Desc :	String Pool Class
*			
*
*
*
*
*	Date: 5/13/2002
*********************************************************************/ 
#include "utilheader.h"

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CStringPool::CStringPool(CDebugInfo *potDebug) : CPoolObj(potDebug)
{
	poDebug = potDebug;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CStringPool::~CStringPool()
{
	int nCnt = 0;
	DLP_CPtrListItem *plt = poInUseList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CPoolBaseObj *poVoidObj = (CPoolBaseObj *)plt->pGetPtr();
			
			if(poVoidObj == 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Void Object is NULL in String Pool Destructor\n");
				continue;
			}
			
			if(poVoidObj->poGetDataPtr() == 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Data Object is NULL in String Pool Destructor\n");
				continue;
			}

			CStr *poStr = (CStr *)poVoidObj->poGetDataPtr();
			delete poStr;
			delete poVoidObj;

			nCnt++;
			plt = nextPlt;
	    }


	poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Released %d InUse String Pool Objects\n",nCnt);

	poInUseList->vRemoveAll();
	
	nCnt = 0;
	plt = poFreeList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CPoolBaseObj *poVoidObj = (CPoolBaseObj *)plt->pGetPtr();

			if(poVoidObj == 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Void Object is NULL in String Pool Destructor\n");
				continue;
			}
			
			if(poVoidObj->poGetDataPtr() == 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Data Object is NULL in String Pool Destructor\n");
				continue;
			}


			CStr *poStr = (CStr *)poVoidObj->poGetDataPtr();
			delete poStr;
			delete poVoidObj;
			plt = nextPlt;
			nCnt++;

	    }

	poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Released %d Free String Pool Objects\n",nCnt);
	poFreeList->vRemoveAll();
}

//////////////////////////////////////////////////////
//
//	Release Message back to free list
//
//////////////////////////////////////////////////////
int CStringPool::nReleaseObj(void *poObj)
{
vSetMutex();
	CStr *poStr = (CStr *)poObj;

	DLP_CPtrListItem *plt = poInUseList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CPoolBaseObj *poListObj = (CPoolBaseObj *)plt->pGetPtr();
			CStr *poStr1 = (CStr *)poListObj->poGetDataPtr();

			if(poStr->nGetUniqueID() == poStr1->nGetUniqueID())
			{
				poInUseList->nRemoveListItem(plt);
			
				if(poListObj->nGetInUse() == 0)
				{
					poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Pool Object %d Not InUse Clear Error\n",poListObj->nGetUniqueID());
				}
				
				//Clear and set in Use to 0
				poListObj->vClear();
				poFreeList->vAddHead(poListObj);

				poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Cleared Pool Objects %d\n",poListObj->nGetUniqueID());
				vClearMutex();
				return 1;
			}


			plt = nextPlt;
	    }

poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Could not Clear Pool String [%d]%s\n",poStr->nGetUniqueID(),poStr->GetString());
vClearMutex();
return 0;
}

//////////////////////////////////////////////////////
//
//	Init Pool
//
//////////////////////////////////////////////////////
void CStringPool::vInitPool(int ntSize)
{
		vSetSize(ntSize);

		//Create Default Messages
		for(int x=0; x<nGetSize(); x++)
		{
			CStr *poStr = new CStr;
			CPoolBaseObj *poVoidObj = new CPoolBaseObj((void *)poStr);
			poFreeList->vAddTail(poVoidObj);
		}

		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Inited %d Strings for Pool\n",nGetSize());

}

//////////////////////////////////////////////////////
//
//	Add Object
//
//////////////////////////////////////////////////////
void * CStringPool::poAddObject(void)
{
	CStr *poStr = new CStr;
	CPoolBaseObj *poVoidObj = new CPoolBaseObj((void *)poStr);

	return poVoidObj;
}

//////////////////////////////////////////////////////
//
//	Test
//
//////////////////////////////////////////////////////
void CStringPool::vTest(void)
{
	int x = 0;
	DLP_CPtrListItem *plt = poInUseList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CPoolBaseObj *poVoidObj = (CPoolBaseObj *)plt->pGetPtr();
			
			if(poVoidObj == 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Void Object is NULL in String Pool Destructor\n");
				continue;
			}
			
			if(poVoidObj->poGetDataPtr() == 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Data Object is NULL in String Pool Destructor\n");
				continue;
			}

			CStr *poStr = (CStr *)poVoidObj->poGetDataPtr();
			poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"InUse String[%d] = %s\n",x,poStr->GetString());

			x++;
			plt = nextPlt;
	    }

	poInUseList->vRemoveAll();
	
	x = 0;
	plt = poFreeList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CPoolBaseObj *poVoidObj = (CPoolBaseObj *)plt->pGetPtr();

			if(poVoidObj == 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Void Object is NULL in String Pool Destructor\n");
				continue;
			}
			
			if(poVoidObj->poGetDataPtr() == 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Data Object is NULL in String Pool Destructor\n");
				continue;
			}

			CStr *poStr = (CStr *)poVoidObj->poGetDataPtr();
			poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Free String[%d] = %s\n",x,poStr->GetString());
			x++;

			plt = nextPlt;

	    }

}
