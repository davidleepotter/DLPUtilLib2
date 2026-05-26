// PoolObj.h: interface for the CPoolObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POOLOBJ_H__376C876C_B3E6_4E83_BF11_C481638DC4F4__INCLUDED_)
#define AFX_POOLOBJ_H__376C876C_B3E6_4E83_BF11_C481638DC4F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "netmutex.h"

class DLP_CPtrList;
class DLP_CPtrArray;
class CDebugInfo;


class CPoolObj : public CNetMutex  
{
public:
	CPoolObj(CDebugInfo *potDebug);
	virtual ~CPoolObj();

	virtual void vInitPool(int nSize) = 0;
	virtual void * poAddObject(void) = 0;
	virtual int nReleaseObj(void *poObj) = 0;

	virtual void * poGetNextFree(void);
	virtual int nGetInUseSize(void);
	virtual int nGetFreeSize(void);
	virtual int nGetSize(void);
	
	DLP_CPtrList *poInUseList;	
	DLP_CPtrList *poFreeList;
	void vSetSize(int nSize);


private:
	int nSize;
	CDebugInfo *poGlobDebug;

};

class CPoolBaseObj
{

public:
	friend CPoolObj;
	CPoolBaseObj(void *ptrDataObj);
	virtual ~CPoolBaseObj();
	void *poGetDataPtr(void);
	int nGetInUse(void);
	void vSetInUse(int nVal);
	unsigned int nGetUniqueID(void);
	void vClear(void);

private:
	unsigned long uLifeTime;
	int nInUse;
	unsigned int nUID;
	static unsigned int gUID;
	void *poObjPtr;

};

#endif // !defined(AFX_POOLOBJ_H__376C876C_B3E6_4E83_BF11_C481638DC4F4__INCLUDED_)
