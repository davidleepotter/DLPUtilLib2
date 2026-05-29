/*********************************************************************
*	File : NetMutex.cpp 
*	Desc : Mutex base Class
*
*	Comment: 
*
*
*
*	Date: 11/01/2001
*********************************************************************/
#if !defined(AFX_NETMUTEX_H__239CD5F8_A7A4_4926_A32D_7B7AAE04F69C__INCLUDED_)
#define AFX_NETMUTEX_H__239CD5F8_A7A4_4926_A32D_7B7AAE04F69C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#ifndef WIN32
#include <pthread.h>
#endif
#include "utildefines.h"
#include "DebugInfo.h"
#include "cstr.h"

class CNetMutex  
{
private:

#ifdef WIN32
	CRITICAL_SECTION     *oCritSect;
#endif

#ifndef WIN32
	pthread_mutex_t		*oCritSect;
#endif
	void vDestroyMutex(void);
	void vInitMutex(void);
	static long uID;
	long uMutexCnt;
	CDebugInfo *poDebug;
	int nDebugInfo;
	CStr *poName;

public:
	CNetMutex(CDebugInfo *poDebug,int nPrintDebugInfo, char * pName);
	virtual ~CNetMutex();

	void vSetMutex(void);
	void vClearMutex(void);
	long uGetID(void);

};
//////////////////////////////////////////////////////
//
//	Set Mutex
//
//////////////////////////////////////////////////////
inline void CNetMutex::vSetMutex(void)
{
#ifdef WIN32
	EnterCriticalSection(oCritSect);
#endif

#ifndef WIN32
	pthread_mutex_lock(oCritSect);
#endif

	uMutexCnt++;
	
	if(poName != 0)
	{
		if((poDebug != 0 && nDebugInfo == MUTEX_DEBUG) && uMutexCnt > 1)
		{
			poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO,"Set Mutex %s ID[%d] Cnt[%d]\n",poName->GetString(),uID,uMutexCnt);
		}
	}


}

//////////////////////////////////////////////////////
//
//	Clear Mutex
//
//////////////////////////////////////////////////////
inline void CNetMutex::vClearMutex(void)
{
#ifdef WIN32
	LeaveCriticalSection(oCritSect);
#endif

#ifndef WIN32
	pthread_mutex_unlock(oCritSect);
#endif

	if(poName != 0)
	{
		if((poDebug != 0 && nDebugInfo == MUTEX_DEBUG) && uMutexCnt > 1)
		{
			poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO,"Cleared Mutex %s ID[%d] Cnt[%d]\n",poName->GetString(),uID,uMutexCnt);
		}
	}

uMutexCnt--;
}
#endif // !defined(AFX_NETMUTEX_H__239CD5F8_A7A4_4926_A32D_7B7AAE04F69C__INCLUDED_)
