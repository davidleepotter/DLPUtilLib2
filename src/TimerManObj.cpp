/*********************************************************************
*	File : TimerManObj.cpp 
*	Desc : Timer Mix In Class
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 
#include "utildefines.h"
#include "timermanobj.h"
#include "nettimer.h"
#include "DebugInfo.h"
#include "NewCol.h"

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CTimerManObj::CTimerManObj(CDebugInfo *potDebug) : CNetMutex(potDebug,MUTEX_DEBUG,"TimerManager")
{
	poDebug = potDebug;
	uTimeResult = 0;

	nAbsolute = 0;
	uVal =0;

	poTimerInUse = new DLP_CPtrList(potDebug);
	poTimerFree = new DLP_CPtrList(potDebug);

	nInited = 0;
}

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CTimerManObj::CTimerManObj(CDebugInfo *potDebug,int nTimers) : CNetMutex(potDebug,MUTEX_DEBUG,"TimerManager")
{
	poDebug = potDebug;
	uTimeResult = 0;

	nAbsolute = 0;
	uVal =0;

	poTimerInUse = new DLP_CPtrList(potDebug);
	poTimerFree = new DLP_CPtrList(potDebug);

	nInited = 0;

	vInitTimers(nTimers);
}


//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CTimerManObj::~CTimerManObj()
{
	DLP_CPtrListItem *plt = poTimerInUse->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNetTimer *poTimer = (CNetTimer *)plt->pGetPtr();
			delete poTimer;
			plt = nextPlt;
	    }

	poTimerInUse->vRemoveAll();
	delete poTimerInUse;
	poTimerInUse = 0;

	plt = poTimerFree->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNetTimer *poTimer = (CNetTimer *)plt->pGetPtr();
			delete poTimer;
			plt = nextPlt;
	    }

	poTimerFree->vRemoveAll();
	delete poTimerFree;
	poTimerFree = 0;
}

//////////////////////////////////////////////////////
//
//	Clear Timer
//
//////////////////////////////////////////////////////
void CTimerManObj::vInitTimers(int nMaxTimers)
{
	for(int x=0; x<nMaxTimers; x++)
	{
		CNetTimer *poTimer = new CNetTimer;	
		poTimerFree->vAddTail(poTimer);
	}

	poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Inited Timer Pool to %d Timers\n",nMaxTimers);
}

//////////////////////////////////////////////////////
//
//	Clear Timer
//
//////////////////////////////////////////////////////
void CTimerManObj::vClearTimer(CNetTimer *poTimer1)
{

//vSetMutex();

	DLP_CPtrListItem *plt = poTimerInUse->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNetTimer *poTimer = (CNetTimer *)plt->pGetPtr();
			
			if(poTimer->n_uID == poTimer1->n_uID)
			{
				poTimerInUse->nRemoveListItem(plt);
				poTimer->vClearTimer();
				poTimerFree->vAddTail(poTimer);
				//vClearMutex();
				return;
			}

			plt = nextPlt;
	    }

poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Could Not find timer to clear= %d\n",poTimer1->n_uID);
//vClearMutex();
}

//////////////////////////////////////////////////////
//
//	Stop Timer
//
//////////////////////////////////////////////////////
void CTimerManObj::vStopTimer(CNetTimer *poTimer1)
{
	if(poTimerInUse == 0)
	{
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO
			,"Timer Not in use so I will not remove = %d\n"
			,poTimer1->n_uID);
		return;
	}

//vSetMutex();
	DLP_CPtrListItem *plt = poTimerInUse->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNetTimer *poTimer = (CNetTimer *)plt->pGetPtr();
			
			if(poTimer->n_uID == poTimer1->n_uID)
			{
				poTimerInUse->nRemoveListItem(plt);
				poTimer->vStopTimer();
				poTimerFree->vAddTail(poTimer);
				//vClearMutex();
				return;
			}

			plt = nextPlt;
	    }

poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_ERROR,"Could Not find timer to stop = %d\n",poTimer1->n_uID);
//vClearMutex();
}

//////////////////////////////////////////////////////
//
//	Get the timer
//
//////////////////////////////////////////////////////
CNetTimer * CTimerManObj::poGetTimer(unsigned long n_val)
{
//vSetMutex();
	DLP_CPtrListItem *plt = poTimerInUse->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNetTimer *poTimer = (CNetTimer *)plt->pGetPtr();
			
			if(poTimer->n_uID == n_val)
			{
				return poTimer;
				//vClearMutex();
			}

			plt = nextPlt;
	    }

poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Could Not find timer = %d\n",n_val);
//vClearMutex();
return 0;
}

//////////////////////////////////////////////////////
//
//	Get Next Un Set Timer
//
//////////////////////////////////////////////////////
CNetTimer * CTimerManObj::poGetNextUnSetTimer()
{
//vSetMutex();
	//Check for rogue timers
	DLP_CPtrListItem *plt = poTimerInUse->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNetTimer *poTimer = (CNetTimer *)plt->pGetPtr();
			
			if(poTimer->b_inuse == 0)
			{
				poTimerInUse->nRemoveListItem(plt);
				poTimer->vClearTimer();
				poTimerFree->vAddTail(poTimer);
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Cleaned Rogue Timer %d\n",poTimer->n_uID);
			}

			plt = nextPlt;
	    }


	plt = poTimerFree->poGetHeadItem();
	CNetTimer *poTimer = 0;

	if(plt == 0)
	{
		poTimer = new CNetTimer;
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Out of timers adding one %d\n",poTimerFree->nGetCount());
	}
	else
	{
		poTimer = (CNetTimer *)plt->pGetPtr();
		poTimerFree->nRemoveListItem(plt);
	}		

	poTimer->vClearTimer();
	poTimer->b_inuse = 1;
	poTimerInUse->vAddTail(poTimer);

//vClearMutex();
return poTimer;
}

//////////////////////////////////////////////////////
//
//	Get Num Used Timers
//
//////////////////////////////////////////////////////
int CTimerManObj::nGetNumUsedTimers(void)
{
	return poTimerInUse->nGetCount();
}


//////////////////////////////////////////////////////
//
//	Check all timers
//
//////////////////////////////////////////////////////
void CTimerManObj::vCheckTimers(void)
{
//vSetMutex();
	DLP_CPtrListItem *plt = poTimerInUse->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNetTimer *poTimer = (CNetTimer *)plt->pGetPtr();
			
			if(poTimer != 0)
			{
				poTimer->bCheckTimer(poDebug);
			}

			plt = nextPlt;
	    }
//vClearMutex();
}


//////////////////////////////////////////////////////
//
//	Check all timers
//
//////////////////////////////////////////////////////
void CTimerManObj::vKillAllTimers(void)
{
//vSetMutex();
	DLP_CPtrListItem *plt = poTimerInUse->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNetTimer *poTimer = (CNetTimer *)plt->pGetPtr();
			
			poTimerInUse->nRemoveListItem(plt);
			poTimerFree->vAddTail(poTimer);
			poTimer->vStopTimer();
			plt = nextPlt;
	    }
//vClearMutex();
}

//////////////////////////////////////////////////////
//
//	Reset result
//
//////////////////////////////////////////////////////
void CTimerManObj::vResetResult(void)
{
	uTimeResult =  nGetCurrent();
}

//////////////////////////////////////////////////////
//
//	Get Time Result
//
//////////////////////////////////////////////////////
int CTimerManObj::nGetResult(int nVal1)
{

unsigned long uCurTime = nGetCurrent();
int nVal = (uCurTime - uTimeResult);

	if(nVal1 == 1)
	{
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Time Result = %d\n",nVal);
	}

return nVal;
}



#ifdef WIN32
	//////////////////////////////////////////////////////
	//
	// Set Current
	//
	//////////////////////////////////////////////////////
	void CTimerManObj::vSetCurrent(void)
		{
			//vSetMutex();
			nAbsolute = 1;
			uVal = GetTickCount();
			//vClearMutex();
		}

	//////////////////////////////////////////////////////
	//
	//	Get Curret
	//
	//////////////////////////////////////////////////////
	unsigned int CTimerManObj::nGetCurrent(void) 
		{
			return GetTickCount();
		}
#endif



#ifndef WIN32
	#include <unistd.h>
	#include <sys/timeb.h>
	#include <sys/time.h>

	//////////////////////////////////////////////////////
	//
	//	Set Current
	//
	//////////////////////////////////////////////////////
	void CTimerManObj::vSetCurrent(void)
	{
		//vSetMutex();

			nAbsolute = 1;
   			timeb o_time;
			ftime(&o_time);
			uVal = (o_time.time *1000)+o_time.millitm;
		//vClearMutex();
	}


	//////////////////////////////////////////////////////
	//
	//	Get Current
	//
	//////////////////////////////////////////////////////
	unsigned int CTimerManObj::nGetCurrent(void) 
	{
   			timeb o_time;
			ftime(&o_time);
			return (o_time.time *1000)+o_time.millitm;
	}

#endif
