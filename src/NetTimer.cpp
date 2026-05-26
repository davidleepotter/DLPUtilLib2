/*********************************************************************
*	File : NetTimer.cpp 
*	Desc : Timer Mix In Class
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 

#include "utildefines.h"
#include "nettimer.h"
#include "debuginfo.h"

#ifdef WIN32
	#include <windows.h>
#endif


#ifndef WIN32
	#include <unistd.h>
	#include <sys/timeb.h>
	#include <sys/time.h>
#endif


#include <stdio.h>

//Gloabal static uid
unsigned long CNetTimer::g_uID1 = 0;

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CNetTimer::CNetTimer()
{
	b_inuse			= 0;
	ul_createTime	= uGetGlobTickCount();
	ul_time_val		= 0;
	callback		= 0;
	n_param			= 0;
	b_loop			= false;
	b_paused		= false;
	n_uID			= nSetUid();
	poVoidPointer1	= 0;
	poVoidPointer2	= 0;
	ulLifeTime		= 0;
	nInTime			= 0;
	nState			= TIMER_PAUSED;

}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CNetTimer::~CNetTimer()
{

}

//////////////////////////////////////////////////////
//
//	Get The Tick Count number of ms since 1970
//
//////////////////////////////////////////////////////
unsigned long CNetTimer::uGetGlobTickCount()
{
	#ifdef WIN32
		return GetTickCount();
	#endif

	#ifndef WIN32
		timeb o_time;
		ftime(&o_time);
		return (o_time.time *1000)+o_time.millitm;
	#endif

	

}

//////////////////////////////////////////////////////
//
//	Check this timer
//
//////////////////////////////////////////////////////
bool CNetTimer::bCheckTimer(CDebugInfo *poDebug)
{
unsigned long ul_val1 = uGetGlobTickCount();
unsigned long ul_val2 = ul_createTime + ul_time_val;

	//Timer is paused
	if(this->b_paused == true)
	{
		vClearTimer();
		return true;
	}

	if(ul_val1 >= ul_val2)
	{
		if(callback !=0)
		{

			if(nInTime == 1)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR
					,"****** Timer Callback already running *****\n");

				nState	= TIMER_RUNNING;
				return true;
			}

			ulLifeTime +=  ul_val1 -  ul_createTime;

			nInTime = 1;
			(*callback)(n_param,poVoidPointer1,poVoidPointer2,o_NetTimer);
			nInTime = 0;

			vClearTimer();
		}
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////
//
//	reset Timer
//
//////////////////////////////////////////////////////
void CNetTimer::vReSetTimer()
{
	ul_createTime	= uGetGlobTickCount();
	ul_time_val		= 0;
	nState			= TIMER_PAUSED;
}

//////////////////////////////////////////////////////
//
//	Clear the timer
//
//////////////////////////////////////////////////////
void CNetTimer::vClearTimer()
{
	if(b_loop == true)
	{
		ul_createTime	= uGetGlobTickCount();
	}
	else
	{
		ul_createTime	= uGetGlobTickCount();
		b_inuse			= 0;
		ul_time_val		= 0;
		n_param			= 0;
		callback		= 0;
		n_uID			= nSetUid();
		poVoidPointer1	= 0;
		poVoidPointer2	= 0;
		ulLifeTime		= 0;
		nState			= TIMER_PAUSED;
	}
}

//////////////////////////////////////////////////////
//
//	Stop the timer
//
//////////////////////////////////////////////////////
void CNetTimer::vStopTimer()
{
		ul_createTime	= 0;
		b_inuse			= 0;
		ul_time_val		= 0;
		n_param			= 0;
		callback		= 0;
		n_uID			= nSetUid();
		poVoidPointer1	= 0;
		poVoidPointer2	= 0;
		ulLifeTime		= 0;
		nState			= TIMER_PAUSED;
}

//////////////////////////////////////////////////////
//
//	Set Timer 
//
//////////////////////////////////////////////////////
void CNetTimer::vSetTimer(unsigned long ul_val, void (*tcallback)(unsigned long,void *,void *,CNetTimer *), unsigned long n_tparam,void *potVoidPointer1,void *potVoidPointer2,CNetTimer *o_tNetTimer,bool b_tloop)
{
	ul_createTime	= uGetGlobTickCount();
	ul_time_val		= ul_val;
	b_inuse			= true;
	b_paused		= false;
	n_param			= n_tparam;
	callback		= tcallback;
	b_loop			= b_tloop;
	poVoidPointer1	= potVoidPointer1;
	poVoidPointer2	= potVoidPointer2;
	o_NetTimer		= o_tNetTimer;
	nState			= TIMER_RUNNING;
}



//////////////////////////////////////////////////////
//
//	set the loop
//
//////////////////////////////////////////////////////
void CNetTimer::vSetLoop(bool b_tloop)
{
	b_loop			= b_tloop;
}

//////////////////////////////////////////////////////
//
//	ModTimer
//
//////////////////////////////////////////////////////
void CNetTimer::vModTimer(unsigned long nVal)
{
	int nLoopTrue = 0;

	if(b_loop == 1)
	{
		b_loop = 0;
		nLoopTrue =1;
	}

	ulLifeTime		= 0;
	ul_createTime	= uGetGlobTickCount();
	ul_time_val		= nVal;


	if(nLoopTrue == 1)
	{
		b_loop = 1;
	}
}

//////////////////////////////////////////////////////
//
//	Get Life Time
//////////////////////////////////////////////////////
unsigned long CNetTimer::ulGetLifeTime(void)
{
	return ulLifeTime;
}

//////////////////////////////////////////////////////
//
//	Pause the timer
//
//////////////////////////////////////////////////////
void CNetTimer::vPauseTimer(void)
{
		b_paused		= true;
		nState			= TIMER_PAUSED;

}

//////////////////////////////////////////////////////
//
//	Start Timer
//
//////////////////////////////////////////////////////
void CNetTimer::vStartTimer(void)
{
		b_paused		= false;
		nState			= TIMER_RUNNING;

}

//////////////////////////////////////////////////////
//
//	Is Paused
//////////////////////////////////////////////////////
bool CNetTimer::bIsPaused(void)
{
	return this->b_paused;
}