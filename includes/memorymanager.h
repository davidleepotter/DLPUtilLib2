/*********************************************************************
*	File : CMemoryManager.h
*	Desc : CMemoryManager
*
*	Comment: 
*
*
*
*	Date: 12/22/2003
*********************************************************************/
#if !defined(AFX_MEMORYMANAGER_H__7B2D8F1F_6A06_4A83_A1AF_B2D8F8771A56__INCLUDED_)
#define AFX_MEMORYMANAGER_H__7B2D8F1F_6A06_4A83_A1AF_B2D8F8771A56__INCLUDED_

class CDebugInfo;
class DLP_CPtrList;
class CMemoryManager;


class CMemoryObject
{
private:
	void *memoryPtr;
	long m_nSize;
	long nUID;

public:
	friend CMemoryManager;
	CMemoryObject();
	void * poAllocate(long nSize);
	int nFreeObject(void);
	void *poMemoryPtr(void);
	long nGetSize(void);

	virtual ~CMemoryObject();
};


class CMemoryManager
{
private:
	CDebugInfo *m_poDebug;
	DLP_CPtrList *m_poMemoryList;

	long m_nInUseCnt;
	long m_nFreeCnt;
	long m_nInUseSize;

public:
	friend CMemoryObject;

	CMemoryManager(CDebugInfo *potDebug);
	virtual ~CMemoryManager(void);
	void *poAllocateMemory(size_t nSize);
	CMemoryObject *poFindMemoryObj(void *ptr);
	int nRemoveMemoryObj(long nUid);

};

//------------------------------------------------------------------------------
// operator new overload
//------------------------------------------------------------------------------

#endif