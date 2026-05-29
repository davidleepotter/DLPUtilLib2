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

#include "netmutex.h"
#include "utildefines.h"
#include "DebugInfo.h"
#include "cstr.h"

long CNetMutex::uID = 0;


//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CNetMutex::CNetMutex(CDebugInfo *potDebug,int nMutexDebugInfo, char *pName)
{
	nDebugInfo = nMutexDebugInfo;
	poDebug = potDebug;

	if(nMutexDebugInfo== MUTEX_DEBUG)
	{
		poName = new CStr;
		poName->Format("%s",pName);
	}
	else if(nMutexDebugInfo	== MUTEX_NO_DEBUG)
	{
		poName = 0;
	}

	uMutexCnt = 0;
	vInitMutex();
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CNetMutex::~CNetMutex()
{
	vDestroyMutex();

	if(poName != 0)
	{
		delete poName;
	}
}

//////////////////////////////////////////////////////
//
//	Init Mutex
//
//////////////////////////////////////////////////////
void CNetMutex::vInitMutex(void)
{
#ifdef WIN32
	oCritSect = new CRITICAL_SECTION;
	InitializeCriticalSection(oCritSect);
#endif

#ifndef WIN32
	oCritSect = new pthread_mutex_t;
 
 int res = pthread_mutex_init(oCritSect, NULL);
    if (res != 0) 
	{
		if(poName != 0)
		{
			if(poDebug != 0 && nDebugInfo == MUTEX_DEBUG)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"CritSect %s initialization failed",poName->GetString());
			}
		}
	}

#endif

	if(poName != 0)
	{
		if(poDebug != 0 && nDebugInfo == MUTEX_DEBUG)
		{
			poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO,"Inited Mutex %s ID %d\n",poName->GetString(),uID);
		}
	}

	uID++;
}




//////////////////////////////////////////////////////
//
//	Set Mutex
//
//////////////////////////////////////////////////////
void CNetMutex::vDestroyMutex(void)
{

#ifdef WIN32
	DeleteCriticalSection(oCritSect);
	delete oCritSect;
#endif

#ifndef WIN32
	pthread_mutex_destroy(oCritSect);
	delete oCritSect;
#endif

	uID--;
}
