/*********************************************************************
*	File : CMemoryManager.cpp
*	Desc : CMemoryManager
*
*	Comment: 
*
*
*
*	Date: 12/22/2003
*********************************************************************/
#include "utilheader.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <malloc.h>

CMemoryManager *poGlobalMemoryManger = 0;

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CMemoryObject::CMemoryObject()
{
	m_nSize = 0;
	memoryPtr = 0;

}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CMemoryObject::~CMemoryObject(void)
{
	if(memoryPtr != 0)
	{
		free(memoryPtr);
		m_nSize = 0;
	}

}

//////////////////////////////////////////////////////
//
//	nFreeObject
//
//////////////////////////////////////////////////////
int CMemoryObject::nFreeObject(void)
{
	if(memoryPtr != 0)
	{
		free(memoryPtr);
		m_nSize = 0;
		return 1;
	}

return 0;
}


//////////////////////////////////////////////////////
//
//	Allocate
//
//////////////////////////////////////////////////////
void * CMemoryObject::poAllocate(long nSize)
{
	m_nSize = nSize;
	memoryPtr = malloc(nSize);
	
	return memoryPtr;
}


//////////////////////////////////////////////////////
//
//	poMemoryPtr
//
//////////////////////////////////////////////////////
void * CMemoryObject::poMemoryPtr(void)
{
	return memoryPtr;
}

//////////////////////////////////////////////////////
//
//	nGetSize
//
//////////////////////////////////////////////////////
long CMemoryObject::nGetSize(void)
{
	return m_nSize;
}


//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CMemoryManager::CMemoryManager(CDebugInfo *potDebug)
{
	m_poDebug = potDebug;
	m_poMemoryList = new DLP_CPtrList(m_poDebug);

	m_nInUseCnt		= 0;
	m_nFreeCnt		= 0;
	m_nInUseSize	= 0;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CMemoryManager::~CMemoryManager(void)
{
	DLP_CPtrListItem *plt = m_poMemoryList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CMemoryObject *poMemObj = (CMemoryObject *)plt->pGetPtr();
			delete poMemObj;
			plt = nextPlt;
	    }

	m_poMemoryList->vRemoveAll();
	delete m_poMemoryList;
	m_poMemoryList = 0;
}

//////////////////////////////////////////////////////
//
//	poAllocateMemory
//
//////////////////////////////////////////////////////
void * CMemoryManager::poAllocateMemory(size_t nSize)
{

	void* pObj = (void*)malloc(sizeof(CMemoryObject));

	CMemoryObject *poMemObj = 0;
	poMemObj = (CMemoryObject *)pObj;
	void *pMemPtr = poMemObj->poAllocate(nSize);

	m_poMemoryList->vAddTail(poMemObj);


return 0;
}

//////////////////////////////////////////////////////
//
//	poFindMemoryObj
//
//////////////////////////////////////////////////////
CMemoryObject * CMemoryManager::poFindMemoryObj(void *ptr)
{
	DLP_CPtrListItem *plt = m_poMemoryList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CMemoryObject *poMemObj = (CMemoryObject *)plt->pGetPtr();
			

			if(poMemObj->memoryPtr == ptr)
			{
				return poMemObj;
			}

			plt = nextPlt;
	    }

return 0;
}
/*
//////////////////////////////////////////////////////
//
//	OverLoaded New
//
//////////////////////////////////////////////////////
void* operator new(size_t size)
{
	void *ptr = poGlobalMemoryManger->poAllocateMemory(size);

return ptr;
}



//////////////////////////////////////////////////////
//
//	OverLoaded delete
//
//////////////////////////////////////////////////////
void operator delete(void* pMem)
{
	//
	//	Safeguard against a NULL pointer, since it is legal to pass NULL to "operator delete".
	//
	if(pMem)
	{
		CMemoryObject *poMemObj = 0;
		poMemObj = poGlobalMemoryManger->poFindMemoryObj(pMem);
		poMemObj->nFreeObject();



		delete pMem;
	}
}
*/