/*********************************************************************
*	File : NewCol.cpp 
*	Desc : Collection Class - Array, List, Hash Table
*
*	Comment:
*
*
*
*	Date: 11/01/2000
*********************************************************************/ 
#include <stdio.h>
#include "NewCol.h"
#include <string.h>
#include "utildefines.h"
#include "DebugInfo.h"
#include "timermanobj.h"
#include "dlprnd.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "netmutex.h"


int DLP_CPtrListItem::nObjectCount = 0;
int DLP_CPtrList::nObjectCount = 0;
int DLP_CPtrArray::nObjectCount = 0;
int DLP_CHashTable::nObjectCount = 0;

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
DLP_CPtrListItem::DLP_CPtrListItem(void)
{
	pNext = 0;
	pPrev = 0;
	pPtr  = 0;
    nObjectCount++;
}


//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
DLP_CPtrListItem::~DLP_CPtrListItem(void)
{
    pNext = 0;
    pPrev = 0;
    pPtr = 0;
    nObjectCount--;
}

//////////////////////////////////////////////////////
//
//	Get Next
//
//////////////////////////////////////////////////////
DLP_CPtrListItem *DLP_CPtrListItem::pGetNext(void)
{
    return pNext;
}


//////////////////////////////////////////////////////
//
//	Get Prev
//
//////////////////////////////////////////////////////
DLP_CPtrListItem *DLP_CPtrListItem::pGetPrev(void)
{
    return pPrev;
}


//////////////////////////////////////////////////////
//
//	Get Ptr
//
//////////////////////////////////////////////////////
void *DLP_CPtrListItem::pGetPtr(void)
{
    return pPtr;
}


//////////////////////////////////////////////////////
//
//	Set Next
//
//////////////////////////////////////////////////////
void DLP_CPtrListItem::vSetNext(DLP_CPtrListItem *item)
{
    pNext = item;
}


//////////////////////////////////////////////////////
//
//	Set Prev
//
//////////////////////////////////////////////////////
void DLP_CPtrListItem::vSetPrev(DLP_CPtrListItem *item)
{
    pPrev = item;
}


//////////////////////////////////////////////////////
//
//	Set Pointer
//
//////////////////////////////////////////////////////
void DLP_CPtrListItem::vSetPtr(void *p)
{
    pPtr = p;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DLP_CPtrList
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
DLP_CPtrList::DLP_CPtrList(CDebugInfo *poDebug)
{
	poMutex = new CNetMutex(0,MUTEX_NO_DEBUG,"PtrList");
	nCount	= 0;
	poHead	= 0;
	poTail	= 0;
	nLocked = 0;
    nObjectCount++;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
DLP_CPtrList::~DLP_CPtrList(void)
{
    nObjectCount--;
    vRemoveAll();
    nCount = -1;
    poHead = 0;
    poTail = 0;
	delete poMutex;
	poMutex = 0;
}

//////////////////////////////////////////////////////
//
//	Add Head
//
//////////////////////////////////////////////////////
void DLP_CPtrList::vAddItemHead(DLP_CPtrListItem *item)
{
    if (!item)
    {
        return;
    }
    
	poMutex->vSetMutex();
	if (nCount)
	{
        item->vSetNext(poHead);
        poHead->vSetPrev(item);
    }
    else
    {
        item->vSetNext(0);
        poTail = item;
    }
    item->vSetPrev(0);
    poHead = item;
    nCount++;

	poMutex->vClearMutex();
return;
}


//////////////////////////////////////////////////////
//
//	Add Tail
//
//////////////////////////////////////////////////////
void DLP_CPtrList::vAddItemTail(DLP_CPtrListItem *item)
{
	/////////////////////////////
	//Do Not Mutex
	/////////////////////////////

    if (!item)
    {
        return;
    }
   
    if (nCount)
    {
        item->vSetPrev(poTail);
        poTail->vSetNext(item);
    }
    else
    {
        item->vSetPrev(0);
        poHead = item;
    }

    item->vSetNext(0);
    poTail = item;
    nCount++;
}

//////////////////////////////////////////////////////
//
//	Remove Item
//
//////////////////////////////////////////////////////
int DLP_CPtrList::nRemoveInternalItem(DLP_CPtrListItem *item)
{
	/////////////////////////////
	//Do Not Mutex
	/////////////////////////////
    if (!item || !nCount)
    {
        return 0;
    }
  
    if (nCount == 1)
    {
        poHead = poTail = 0;
        nCount = 0;
        delete item;
	    return 1;
    }
    
    if (poHead == item)
    {
        poHead = item->pGetNext();
        poHead->vSetPrev(0);
    }
    else if (poTail == item)
    {
        poTail = item->pGetPrev();
        poTail->vSetNext(0);
    }
    else
    {
        item->pGetPrev()->vSetNext(item->pGetNext());
        item->pGetNext()->vSetPrev(item->pGetPrev());
    }
    nCount--;
    delete item;

return 1;
}

//////////////////////////////////////////////////////
//
//	Find Indentical
//
//////////////////////////////////////////////////////
void *DLP_CPtrList::pFindIdentical(void *p)
{
	poMutex->vSetMutex();

    DLP_CPtrListItem *item = poHead;
    while (item)
	{
		if (p == item->pGetPtr()) 
	    {
			poMutex->vClearMutex();
			return item->pGetPtr();
	    }
		item = item->pGetNext();
	}
	
	poMutex->vClearMutex();
    return 0;
}

//////////////////////////////////////////////////////
//
//	Get Head Item
//
//////////////////////////////////////////////////////
DLP_CPtrListItem *DLP_CPtrList::poGetHeadItem(void)
{
    return poHead;
}


//////////////////////////////////////////////////////
//
//	Get Tail Item
//
//////////////////////////////////////////////////////
DLP_CPtrListItem *DLP_CPtrList::poGetTailItem(void)
{
    return poTail;
}


//////////////////////////////////////////////////////
//
//	Get Head
//
//////////////////////////////////////////////////////
void *DLP_CPtrList::pGetHead(void)
{
    return poHead ? poHead->pGetPtr() : 0;
}


//////////////////////////////////////////////////////
//
//	Get Tail
//
//////////////////////////////////////////////////////
void *DLP_CPtrList::pGetTail(void)
{
    return poTail ? poTail->pGetPtr() : 0;
}


//////////////////////////////////////////////////////
//
//	is Empty
//
//////////////////////////////////////////////////////
int DLP_CPtrList::nIsEmpty(void) const
{
	poMutex->vSetMutex();

	if(nCount == 0)
	{
		poMutex->vClearMutex();
		return 1;
	}

	poMutex->vClearMutex();
    return 0;
}

//////////////////////////////////////////////////////
//
//	Get Count
//
//////////////////////////////////////////////////////
int DLP_CPtrList::nGetCount(void) const
{
    return nCount;
}

//////////////////////////////////////////////////////
//
//	Add Head
//
//////////////////////////////////////////////////////
void DLP_CPtrList::vAddHead(void *p)
{
    if (p == 0)
	{				
        return;
    }

	poMutex->vSetMutex();

    DLP_CPtrListItem *item = new DLP_CPtrListItem;
    item->vSetPtr(p);
	vAddItemHead(item);

	poMutex->vClearMutex();
return;
}

//////////////////////////////////////////////////////
//
//	Add Tail
//
//////////////////////////////////////////////////////
void DLP_CPtrList::vAddTail(void *p)
{
    if (p == 0)
    {
        return;
    }
    
	poMutex->vSetMutex();

	DLP_CPtrListItem *item = new DLP_CPtrListItem;
    item->vSetPtr(p);
    vAddItemTail(item);

	poMutex->vClearMutex();
return;
}

//////////////////////////////////////////////////////
//
//	Remove All
//
//////////////////////////////////////////////////////
void DLP_CPtrList::vRemoveAll(void)
{
	poMutex->vSetMutex();

	DLP_CPtrListItem *item = poHead;
    
    while (item)
    {
        DLP_CPtrListItem *poNextItem = item->pGetNext();
        
        delete item;
        item = poNextItem;
    }
    poHead = poTail = 0;
    nCount = 0;

	poMutex->vClearMutex();
return;
}


//////////////////////////////////////////////////////
//
//	Remove Head
//
//////////////////////////////////////////////////////
void * DLP_CPtrList::pRemoveHead(void)
{    
	poMutex->vSetMutex();

	DLP_CPtrListItem *p = poHead;
    void *retval = 0;
    
    if (poHead)
    {
        nCount--;
        poHead = poHead->pGetNext();
        
        if (!poHead)
        {
            poTail = 0;
        }
        else
        {
            poHead->vSetPrev(0);
        }
        retval = p->pGetPtr();
        delete p;
    }

	poMutex->vClearMutex();
return retval;
}


//////////////////////////////////////////////////////
//
//	Remove Tail
//
//////////////////////////////////////////////////////
void * DLP_CPtrList::pRemoveTail(void)
{
	poMutex->vSetMutex();

    DLP_CPtrListItem *p = poTail;
    void *retval = 0;
    
    if (poTail)
    {
        nCount--;
        poTail = poTail->pGetPrev();
        
        if (!poTail)
        {
            poHead = 0;
        }
        else
        {
            poTail->vSetNext(0);
        }
        retval = p->pGetPtr();
        delete p;
    }
	
	poMutex->vClearMutex();
return retval;
}


//////////////////////////////////////////////////////
//
//	Remove Item
//
//////////////////////////////////////////////////////
void DLP_CPtrList::vRemoveItem(void *p)
{    
	poMutex->vSetMutex();
	DLP_CPtrListItem *item = poHead;
    
    while (item)
    {
        if (p == item->pGetPtr())
        {
            nRemoveInternalItem(item);
            poMutex->vClearMutex();
			return;
        }
        item = item->pGetNext();
    }

	poMutex->vClearMutex();
return;
}

//////////////////////////////////////////////////////
//
//	Remove Item
//
//////////////////////////////////////////////////////
int DLP_CPtrList::nRemoveListItem(DLP_CPtrListItem *item)
{
	poMutex->vSetMutex();

    if (!item || !nCount)
    {
		poMutex->vClearMutex();
        return 0;
    }
  
    if (nCount == 1)
    {
        poHead = poTail = 0;
        nCount = 0;
		poMutex->vClearMutex();
        delete item;
	    return 1;
    }
    
    if (poHead == item)
    {
        poHead = item->pGetNext();
        poHead->vSetPrev(0);
    }
    else if (poTail == item)
    {
        poTail = item->pGetPrev();
        poTail->vSetNext(0);
    }
    else
    {
        item->pGetPrev()->vSetNext(item->pGetNext());
        item->pGetNext()->vSetPrev(item->pGetPrev());
    }
    nCount--;
    delete item;

	poMutex->vClearMutex();
return 1;
}

//////////////////////////////////////////////////////////////////////////////////////
//	DLP_CPtrArray
//////////////////////////////////////////////////////////////////////////////////////
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
DLP_CPtrArray::DLP_CPtrArray()
{
	poMutex = new CNetMutex(0,MUTEX_NO_DEBUG,"PtrArray");
	nObjectCount++;
    m_pData = NULL;
    m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
DLP_CPtrArray::~DLP_CPtrArray()
{
    nObjectCount--;
    delete[](char *)m_pData;
	delete poMutex;
	poMutex = 0;
}

//////////////////////////////////////////////////////
//
//	vRandomize
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vRandomize(void)
{
	CDLPRandom oRand ;
	oRand.setSeed(CTimerManObj::nGetCurrent());
	
	for(int i=0; i<(nGetSize()*100); i++)
	{
		int nRVal = oRand.randomInt(0,this->nGetSize()-1);
		int nRVal1 = oRand.randomInt(0,this->nGetSize()-1);

		void *ptr = pGetAt(nRVal);
		void *ptr1 = pGetAt(nRVal1);

		if(nRVal != nRVal1)
		{
			if(ptr != 0 && ptr1 != 0)
			{
				vSetAt(nRVal,ptr1);
				vSetAt(nRVal1,ptr);
			}
		}

	}
}

//////////////////////////////////////////////////////
//
//	Insert At
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vInsertAt(int nIndex, void *newElement, int nCount)
{
	poMutex->vSetMutex();

    if (nIndex >= m_nSize)
    {
        // adding after the end of the array
        vSetSize(nIndex + nCount);    // grow so nIndex is valid
    }
    else
    {
        // inserting in the middle of the array
        int     nOldSize = m_nSize;
        
        vSetSize(m_nSize + nCount);    // grow it to new size
        
        // shift old data up to fill gap
        memmove(&m_pData[nIndex + nCount], &m_pData[nIndex], (nOldSize - nIndex) * sizeof(void *));
        
        // re-init slots we copied from
        memset(&m_pData[nIndex], 0, nCount * sizeof(void *));
    }
    
    // insert new value in the gap
    while (nCount--)
    {
        m_pData[nIndex++] = newElement;
    }

	poMutex->vClearMutex();
}

//////////////////////////////////////////////////////
//
//	Set Size
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vSetSize(int nNewSize, int nGrowBy)
{
	//////////////////
	//Do Not Mutex
	//////////////////

    if (nGrowBy != -1)
    {
        m_nGrowBy = nGrowBy;    // set new size
    }
    
    if (nNewSize == 0)
    {
        // shrink to nothing
        delete[](char *)m_pData;
        m_pData = NULL;
        m_nSize = m_nMaxSize = 0;
    }
    else if (m_pData == NULL)
    {
        // create one with exact size
        m_pData = (void **) new char[nNewSize *sizeof(void *)];
        memset(m_pData, 0, nNewSize * sizeof(void *));    // zero fill
        m_nSize = m_nMaxSize = nNewSize;
    }
    else if (nNewSize <= m_nMaxSize)
    {
        // it fits
        if (nNewSize > m_nSize)
        {
            // initialize the new elements
            memset(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(void *));
        }
        m_nSize = nNewSize;
    }
    else
    {
        // otherwise, grow array
        int     nGrowBy = m_nGrowBy;
        
        if (nGrowBy == 0)
        {
            // heuristically determine growth when nGrowBy == 0
            //  (this avoids heap fragmentation in many situations)
            nGrowBy = min(1024, max(4, m_nSize / 8));
        }
        
        int     nNewMax;
        
        if (nNewSize < m_nMaxSize + nGrowBy)
        {
            nNewMax = m_nMaxSize + nGrowBy;    // granularity
        }
        else
        {
            nNewMax = nNewSize;    // no slush
        }
        
        void     **pNewData = (void **) new char[nNewMax *sizeof(void *)];
        
        // copy new data from old
        memcpy(pNewData, m_pData, m_nSize * sizeof(void *));
        
        // construct remaining elements
        memset(&pNewData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(void *));
        
        // get rid of old stuff (note: no destructors called)
        delete[](char *)m_pData;
        m_pData = pNewData;
        m_nSize = nNewSize;
        m_nMaxSize = nNewMax;
    }
}


//////////////////////////////////////////////////////
//
//	Element At
//
//////////////////////////////////////////////////////
void *& DLP_CPtrArray::pElementAt(int nIndex)
{
    return m_pData[nIndex];
}


//////////////////////////////////////////////////////
//
//	Get At
//
//////////////////////////////////////////////////////
void * DLP_CPtrArray::pGetAt(int nIndex) const
{
    return m_pData[nIndex];
}


//////////////////////////////////////////////////////
//
//	SetAt
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vSetAt(int nIndex, void *newElement)
{
	poMutex->vSetMutex();
    m_pData[nIndex] = newElement;
	poMutex->vClearMutex();
}


//////////////////////////////////////////////////////
//
//	Append
//
//////////////////////////////////////////////////////
int DLP_CPtrArray::nAppend(const DLP_CPtrArray &src)
{
	poMutex->vSetMutex();

    int     nOldSize = m_nSize;
    
    vSetSize(m_nSize + src.m_nSize);
    memcpy(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(void *));
	
	poMutex->vClearMutex();

return nOldSize;
}


//////////////////////////////////////////////////////
//
//	Copy
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vCopy(const DLP_CPtrArray &src)
{
	poMutex->vSetMutex();

    vSetSize(src.m_nSize);
    memcpy(m_pData, src.m_pData, src.m_nSize * sizeof(void *));

	poMutex->vClearMutex();

}

//////////////////////////////////////////////////////
//
//	Free Extra
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vFreeExtra()
{
	poMutex->vSetMutex();

    if (m_nSize != m_nMaxSize)
    {
        // shrink to desired size
        void     **pNewData = NULL;
        
        if (m_nSize != 0)
        {
            pNewData = (void **) new char[m_nSize *sizeof(void *)];
            
            // copy new data from old
            memcpy(pNewData, m_pData, m_nSize * sizeof(void *));
        }
        
        // get rid of old stuff (note: no destructors called)
        delete[](char *)m_pData;
        m_pData = pNewData;
        m_nMaxSize = m_nSize;
    }

	poMutex->vClearMutex();
}


//////////////////////////////////////////////////////
//
//	Add
//
//////////////////////////////////////////////////////
int DLP_CPtrArray::nAdd(void *newElement)
{
	poMutex->vSetMutex();

    int     nIndex = m_nSize;
    vSetSize(nIndex + 1);
    m_pData[nIndex] = newElement;

	poMutex->vClearMutex();

return nIndex + 1;
}


//////////////////////////////////////////////////////
//
//	Set Grow
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vSetAtGrow(int nIndex, void *newElement)
{
	poMutex->vSetMutex();

    if (nIndex >= m_nSize)
    {
        vSetSize(nIndex + 1);
    }
    m_pData[nIndex] = newElement;

	poMutex->vClearMutex();
}

//////////////////////////////////////////////////////
//
//	Remove All
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vRemoveAll(void)
{
	////////////////////////////////////////
	//Do Not Mutex The Remove At is Mutexed
	///////////////////////////////////////
	if(nGetSize() <= 0)
	{
		return;
	}

	for(;;)
	{
		int nVal = nGetSize() - 1;
        vRemoveAt(nVal);

		if(nVal == 0)
		{
			break;
		}
    }
}


//////////////////////////////////////////////////////
//
//	Remove At
//
//////////////////////////////////////////////////////
void DLP_CPtrArray::vRemoveAt(int nIndex, int nCount)
{
	poMutex->vSetMutex();

    // just remove a range
    int     nMoveCount = m_nSize - (nIndex + nCount);
    
    if (nMoveCount)
    {
        memcpy(&m_pData[nIndex], &m_pData[nIndex + nCount], nMoveCount * sizeof(void *));
    }
    m_nSize -= nCount;

	poMutex->vClearMutex();
}



//////////////////////////////////////////////////////
//
//	Get Data
//
//////////////////////////////////////////////////////
void ** DLP_CPtrArray::pGetData()
{
    return(void**)m_pData;
}

//////////////////////////////////////////////////////
//
//	Get Data
//
//////////////////////////////////////////////////////
void * DLP_CPtrArray::pGetDataAddress()
{
    return (void*) m_pData[0];
}

//////////////////////////////////////////////////////
//
//	Get Data
//
//////////////////////////////////////////////////////
int DLP_CPtrArray::nGetSize() const
{
    return m_nSize;
}

//////////////////////////////////////////////////////////////////////////////////////
//	Hash Table
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//
//	Hash Constructor
//
//////////////////////////////////////////////////////
DLP_CHashTable::DLP_CHashTable(unsigned long _size) : size(_size)
{
	poMutex = new CNetMutex(0,MUTEX_NO_DEBUG,"DLP_CHashTable");
	nColCnt = 0;
	
	for(int i=0; i<3; i++)
	{
		poBuckets[i] = new DLP_CPtrArray;
		poBuckets[i]->vSetSize(_size);
	}
    nObjectCount++;

}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
DLP_CHashTable::~DLP_CHashTable(void)
{
	for(int j=0; j<3; j++)
	{
		for(int i=0; i<poBuckets[j]->nGetSize(); i++)
		{
		DLP_CHashItem * poTHash = (DLP_CHashItem *) poBuckets[j]->pGetAt(i);
		delete poTHash;	
		}

		poBuckets[j]->vRemoveAll();
		delete poBuckets[j];
		poBuckets[j] = 0;
	}

    nObjectCount--;
	delete poMutex;
	poMutex = 0;
}

//////////////////////////////////////////////////////
//
//	Get DLP_CHashItem
//
//////////////////////////////////////////////////////
DLP_CHashItem * DLP_CHashTable::poGetHashItem(unsigned long key) const
{
    int     i = nHashFunc(key);
    return(DLP_CHashItem *) poBuckets[0]->pGetAt(i);
}

//////////////////////////////////////////////////////
//
//	Get DLP_CHashItem
//
//////////////////////////////////////////////////////
DLP_CHashItem * DLP_CHashTable::poGetHashItem(char *str) const
{
	DLP_CHashItem *poDLP_CHashItem = 0;

    int     i = nHashFunc(str);

	//Get from Main Hash
	poDLP_CHashItem = (DLP_CHashItem *)poBuckets[0]->pGetAt(i);
	
	if(poDLP_CHashItem != 0)
	{
		//Compare
		if(poDLP_CHashItem->poString->Compare(str) == 0)
		{
			return poDLP_CHashItem;
		}
		else
		{
			//Get from hash 1
			poDLP_CHashItem = (DLP_CHashItem *)poBuckets[1]->pGetAt(i);
			if(poDLP_CHashItem->poString->Compare(str) == 0)
			{
				return poDLP_CHashItem;
			}
			else
			{
				//Get from hash 2
				poDLP_CHashItem = (DLP_CHashItem *)poBuckets[2]->pGetAt(i);
				if(poDLP_CHashItem->poString->Compare(str) == 0)
				{
					return poDLP_CHashItem;
				}
				else
				{
					return 0;
				}

			}
		}
	}

return(0);
}


//////////////////////////////////////////////////////
//
//	nHashFunc
//
//////////////////////////////////////////////////////
int DLP_CHashTable::nHashFunc(unsigned long key) const
{
    return key % size;
}

//////////////////////////////////////////////////////
//
//	nHashFunc
//
//////////////////////////////////////////////////////
int DLP_CHashTable::nHashFunc(char *string) const
{
	unsigned long hash = 0;
	int c = 0;

#if 1
	CStr oStr;
	CStr oStr1;
	oStr.Format("%s",string);
	int nCnt = 0;
	while (c = (unsigned) *string++)
	{
		hash += (c * nCnt);
		
		int nVal = isalnum(oStr.GetAt(nCnt));
		if(nVal == 1)
		{
			oStr1.Format("%c",c);
			hash += atoi(oStr1.GetString());
		}

	   nCnt++;
	}
#endif

#if 0
		hash = 5381;
        while (c = *string++)
		{
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		}
#endif

#if 0
        while (c = *string++)
		{
            hash = c + (hash << 6) + (hash << 16) - hash;
		}
#endif

	//if (hash < 0)
	//{
	//	hash = -hash;
	//}

return hash % size;
}

//////////////////////////////////////////////////////
//
//	Get DLP_CHashItem Idx
//
//////////////////////////////////////////////////////
void * DLP_CHashTable::poGetHashItemByIdx(int nArrayNum,int nIdx) const
{
poMutex->vSetMutex();
    DLP_CHashItem * h = (DLP_CHashItem *) poBuckets[nArrayNum]->pGetAt(nIdx);
    
    if (h != 0)
    {
		poMutex->vClearMutex();
        return(void*)h->data;
    }
    else
    {
		poMutex->vClearMutex();
        return 0;
    }

poMutex->vClearMutex();
return 0;
}

//////////////////////////////////////////////////////
//
//	Hash Lookup
//
//////////////////////////////////////////////////////
void * DLP_CHashTable::vHashLookup(unsigned long key) const
{
poMutex->vSetMutex();
    DLP_CHashItem * h = poGetHashItem(key);
    
    if (h != 0)
    {
		poMutex->vClearMutex();
        return h->data;
    }
poMutex->vClearMutex();
return 0;
}


//////////////////////////////////////////////////////
//
//	Hash Lookup
//
//////////////////////////////////////////////////////
void * DLP_CHashTable::vHashLookup(char *str) const
{
poMutex->vSetMutex();
	DLP_CHashItem *poDLP_CHashItem = 0;

    int     i = nHashFunc(str);

	//Get from Main Hash
	poDLP_CHashItem = (DLP_CHashItem *)poBuckets[0]->pGetAt(i);
	
	if(poDLP_CHashItem != 0)
	{
		//Compare
		if(poDLP_CHashItem->poString->Compare(str) == 0)
		{
			//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
			//"Found has[%s] in Array 0\n"
			//,str);

			poMutex->vClearMutex();
			return poDLP_CHashItem->data;
		}
		else
		{
			//Get from hash 1
			poDLP_CHashItem = (DLP_CHashItem *)poBuckets[1]->pGetAt(i);
			if(poDLP_CHashItem->poString->Compare(str) == 0)
			{
				//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
				//"Found has[%s] in Array 1\n"
				//,str);

				poMutex->vClearMutex();
				return poDLP_CHashItem->data;
			}
			else
			{
				//Get from hash 2
				poDLP_CHashItem = (DLP_CHashItem *)poBuckets[2]->pGetAt(i);
				if(poDLP_CHashItem->poString->Compare(str) == 0)
				{
					//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
					//"Found has[%s] in Array 2\n"
					//,str);

					poMutex->vClearMutex();
					return poDLP_CHashItem->data;
				}
				else
				{

					//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,
					//"Could Not find Hash [%s] in any Array\n"
					//,str);

					poMutex->vClearMutex();
					return 0;
				}

			}
		}
	}

poMutex->vClearMutex();
return(0);
}

//////////////////////////////////////////////////////
//
//	Hash Insert
//
//////////////////////////////////////////////////////
int DLP_CHashTable::nHashInsert(char *str, void *data)
{ 
    //A key of < 0 should not be used but may not be and error
    if (str == 0)
    {
        return 2;
    }

	poMutex->vSetMutex();

		int nHashNum = 0;
		int nKey = nHashFunc(str);

		//Hash 0
		DLP_CHashItem *poTHash = (DLP_CHashItem *)poBuckets[0]->pGetAt(nKey);

		if(poTHash != 0)
		{
			if(poTHash->key != -1)
			{
				//Hash 1
				DLP_CHashItem * poTHash = (DLP_CHashItem *)poBuckets[1]->pGetAt(nKey);
				if(poTHash != 0)
				{
					//Hash 2
					DLP_CHashItem * poTHash = (DLP_CHashItem *)poBuckets[2]->pGetAt(nKey);
					if(poTHash != 0)
					{
						//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,
						//"Inserting ERROR hash[%s]\n"
						//,str);

						poMutex->vClearMutex();
						return -2;
					}
					else
					{
						//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
						//"Inserting hash[%s] into Array 2\n"
						//,str);

						poMutex->vClearMutex();
						nHashNum = 2;
					}
				}
				else
				{
						//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
						//"Inserting hash[%s] into Array 1\n"
						//,str);
						
						poMutex->vClearMutex();
						nHashNum = 1;
				}
			}

		}


	DLP_CHashItem *h = new DLP_CHashItem;
	h->poString =  new CStr;
	h->poString->Format("%s",str);	
	h->key = nKey;
	h->data = data;

	if(nHashNum == 0)
	{
		//poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
		//"Inserting hash[%s] into Array 0\n"
		//,str);

		poBuckets[0]->vSetAt(nKey, (DLP_CHashItem *) h);
	}
	if(nHashNum == 1)
	{
		nColCnt++;
		poBuckets[1]->vSetAt(nKey, (DLP_CHashItem *) h);
	}
	if(nHashNum == 2)
	{
		nColCnt++;
		poBuckets[2]->vSetAt(nKey, (DLP_CHashItem *) h);
	}

poMutex->vClearMutex();
return nKey;
}

//////////////////////////////////////////////////////
//
//	Hash Insert
//
//////////////////////////////////////////////////////
int DLP_CHashTable::nHashInsert(unsigned long key, void *data)
{
    //A key of < 0 should not be used but may not be and error
    if (key <= 0)
    {
        return 2;
    }
  
	poMutex->vSetMutex();
    int i = nHashFunc(key);
    
	DLP_CHashItem * poTHash = poGetHashItem(key);
	if(poTHash != 0)
	{
		if(poTHash->data != 0)
		{
			poMutex->vClearMutex();
			return -2;
		}
	}

    DLP_CHashItem *h = new DLP_CHashItem;
    h->key = key;
    h->data = data;
    poBuckets[0]->vSetAt(i, (DLP_CHashItem *) h);

poMutex->vClearMutex();
return i;
}

//////////////////////////////////////////////////////
//
//	hashDelete
//
//////////////////////////////////////////////////////
void * DLP_CHashTable::poHashDelete(char *str)
{
poMutex->vSetMutex();
	DLP_CHashItem *poDLP_CHashItem = 0;

    int     i = nHashFunc(str);

	//Get from Main Hash
	poDLP_CHashItem = (DLP_CHashItem *)poBuckets[0]->pGetAt(i);
	
	if(poDLP_CHashItem != 0)
	{
		//Compare
		if(poDLP_CHashItem->poString->Compare(str) == 0)
		{
			delete poDLP_CHashItem;
			poBuckets[0]->vSetAt(i, 0);
			poMutex->vClearMutex();
			return poDLP_CHashItem->data;
		}
		else
		{
			//Get from hash 1
			poDLP_CHashItem = (DLP_CHashItem *)poBuckets[1]->pGetAt(i);
			if(poDLP_CHashItem->poString->Compare(str) == 0)
			{
				delete poDLP_CHashItem;
				poBuckets[1]->vSetAt(i, 0);
				poMutex->vClearMutex();
				return poDLP_CHashItem->data;
			}
			else
			{
				//Get from hash 2
				poDLP_CHashItem = (DLP_CHashItem *)poBuckets[2]->pGetAt(i);
				if(poDLP_CHashItem->poString->Compare(str) == 0)
				{
					delete poDLP_CHashItem;
					poBuckets[2]->vSetAt(i, 0);
					poMutex->vClearMutex();
					return poDLP_CHashItem->data;
				}
				else
				{
					return 0;
				}

			}
		}
	}

poMutex->vClearMutex();
return(0);
}

//////////////////////////////////////////////////////
//
//	hashDelete
//
//////////////////////////////////////////////////////
void * DLP_CHashTable::poHashDelete(unsigned long key)
{
poMutex->vSetMutex();
    DLP_CHashItem * poTHash = poGetHashItem(key);
    
    if (poTHash != 0)
    {
        int      i = nHashFunc(key);

		//printf("Indx = %d[%d]\n",i,key);
		//fflush(stdout);

        void     *poData = poTHash->data;
        
        delete poTHash;
        poBuckets[0]->vSetAt(i, 0);

		poMutex->vClearMutex();
        return poData;
    }

poMutex->vClearMutex();
return 0;
}


//////////////////////////////////////////////////////
//
//	hashDelete
//
//////////////////////////////////////////////////////
void * DLP_CHashTable::poHashDeleteIdx(int nIdx)
{
poMutex->vSetMutex();
    DLP_CHashItem * poTHash = (DLP_CHashItem *) poBuckets[0]->pGetAt(nIdx);
    
    if (poTHash != 0)
    {
        void     *poData = poTHash->data;
        
        delete poTHash;
        poBuckets[0]->vSetAt(nIdx, 0);

		poMutex->vClearMutex();
        return poData;
    }

poMutex->vClearMutex();
return 0;
}


//////////////////////////////////////////////////////
//
//	hashDelete
//
//////////////////////////////////////////////////////
void DLP_CHashTable::vHashDeleteAll(void)
{

	for(int j=0; j<3; j++)
	{
		for (int x = 0; x < poBuckets[j]->nGetSize(); x++)
		{
			DLP_CHashItem * poTHash = (DLP_CHashItem *) poBuckets[j]->pGetAt(x);
	        
			if (poTHash != 0)
			{
				delete poTHash;
				poBuckets[j]->vSetAt(x, 0);
			}
		}
	}

 

return;
}

//////////////////////////////////////////////////////
//
//	Get Count
//
//////////////////////////////////////////////////////
int DLP_CHashTable::nGetCollisionCount(void)
{
	return nColCnt;
}

//////////////////////////////////////////////////////
//
//	Get Count
//
//////////////////////////////////////////////////////
int DLP_CHashTable::nGetCount(int nArrayNum)
{
poMutex->vSetMutex();
    int     nVal = poBuckets[nArrayNum]->nGetSize();
poMutex->vClearMutex();    
return nVal;
}

//////////////////////////////////////////////////////
//
//	Get Count
//
//////////////////////////////////////////////////////
int DLP_CHashTable::nGetHashCount(int nArrayNum)
{
poMutex->vSetMutex();
	int nCnt = 0;
	for(int x=0; x<poBuckets[nArrayNum]->nGetSize(); x++)
	{
		DLP_CHashItem *pItem = 0;
		pItem = (DLP_CHashItem *) poBuckets[nArrayNum]->pGetAt(x);
		if(pItem != 0)
		{
			nCnt++;
		}
 
	}
poMutex->vClearMutex();
return nCnt;
}

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
DLP_CHashItem::DLP_CHashItem()
{
	nIsString = 0;
	key = -1;
	data = 0;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
DLP_CHashItem::~DLP_CHashItem()
{
	if(poString != 0)
	{
		delete poString;
		poString = 0;
	}

}

//////////////////////////////////////////////////////
//
//	Compare
//
//////////////////////////////////////////////////////
int DLP_CHashItem::compare(DLP_CPtrListItem *item) const
{
	return key == ((DLP_CHashItem *)item)->key;
}



//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
DLP_CListAccess::DLP_CListAccess(DLP_CPtrList *potList,CDebugInfo *potDebug)
{

	poMutex = new CNetMutex(potDebug,MUTEX_DEBUG,"PtrListAccess");
	poList = potList;
	poDebug = potDebug;

	for(int x=0; x<10; x++)
	{	
		GlobPlt[x] = 0;
		GlobNextPlt[x] = 0;
	}

}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
DLP_CListAccess::~DLP_CListAccess()
{
	
}

//////////////////////////////////////////////////////
//
//	Get First Item
//
//////////////////////////////////////////////////////
void * DLP_CListAccess::poGetFirstItem(int nQueryNum)
{
	if(poList == 0)
	{
		return 0;
	}

	if(poList->nGetCount() <= 0)
	{
		return 0;
	}

	poMutex->vSetMutex();

	GlobPlt[nQueryNum] = poList->poGetHeadItem();

	if(GlobPlt[nQueryNum] == 0)
	{
		poMutex->vClearMutex();
		return 0;
	}

	void *poItem = (void *)GlobPlt[nQueryNum]->pGetPtr();
	poMutex->vClearMutex();
	return poItem;
}


//////////////////////////////////////////////////////
//
//	Get Last Item
//
//////////////////////////////////////////////////////
void * DLP_CListAccess::poGetLastItem(int nQueryNum)
{
	if(poList == 0)
	{
		return 0;
	}

	if(poList->nGetCount() <= 0)
	{
		return 0;
	}

	poMutex->vSetMutex();
	GlobPlt[nQueryNum] = poList->poGetTailItem();

	if(GlobPlt[nQueryNum] == 0)
	{
		poMutex->vClearMutex();
		return 0;
	}

	void *poItem = (void *)GlobPlt[nQueryNum]->pGetPtr();

	poMutex->vClearMutex();
	return poItem;
}


//////////////////////////////////////////////////////
//
//	Get Next Item
//
//////////////////////////////////////////////////////
void * DLP_CListAccess::poGetNextItem(int nQueryNum)
{
	if(poList == 0)
	{
		return 0;
	}

	if(poList->nGetCount() <= 0)
	{
		return 0;
	}

	if(GlobPlt[nQueryNum] == 0)
	{
		return 0;
	}

	poMutex->vSetMutex();
	GlobNextPlt[nQueryNum] = GlobPlt[nQueryNum]->pGetNext();

	if(GlobNextPlt[nQueryNum] == 0)
	{
		GlobPlt[nQueryNum] = 0;
		poMutex->vClearMutex();
		return 0;
	}	
	
	void *poItem = (void *)GlobNextPlt[nQueryNum]->pGetPtr();
	GlobPlt[nQueryNum] = GlobNextPlt[nQueryNum];

	poMutex->vClearMutex();
	return poItem;
}

//////////////////////////////////////////////////////
//
//	Get Count
//
//////////////////////////////////////////////////////
int DLP_CListAccess::nGetCount(void)
{
	if(poList == 0)
	{
		return 0;
	}

	return poList->nGetCount();
}

