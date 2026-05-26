/*********************************************************************
*	File : TimerManObj.h
*	Desc : Timer Man Base Class
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/
#if !defined(AFX_TIMERMANOBJ_H__2BC9A9B4_1302_40B5_9E68_A7EA19636643__INCLUDED_)
#define AFX_TIMERMANOBJ_H__2BC9A9B4_1302_40B5_9E68_A7EA19636643__INCLUDED_

class CDebugInfo;
class CNetTimer;
class DLP_CPtrList;

#include "netmutex.h"

class CTimerManObj : CNetMutex  
{
private:
	CDebugInfo *poDebug;
	
	int nAbsolute;
	unsigned long uVal;
	unsigned long uTimeResult;
	DLP_CPtrList *poTimerInUse;				//Timer List
	DLP_CPtrList *poTimerFree;				//Timer List

public:

	char nInited;

	CTimerManObj(CDebugInfo *parent);
	CTimerManObj(CDebugInfo *parent,int nTimers);
	virtual ~CTimerManObj();
	CNetTimer * poGetTimer(unsigned long n_val);
	CNetTimer * poGetNextUnSetTimer(void);
	static unsigned int nGetCurrent(void);	// Get the current system tickcount in msec.

	void vResetResult(void);
	void vClearTimer(CNetTimer *poTimer);
	void vInitTimers(int nMaxTimers);
	void vCheckTimers(void);
	void vSetCurrent(void);					// Set the time to the current system time.
	void vKillAllTimers(void);
	void vStopTimer(CNetTimer *poTimer);

	int nGetNumUsedTimers(void);
	int nGetResult(int nVal1);


};

#endif // !defined(AFX_TIMERMANOBJ_H__2BC9A9B4_1302_40B5_9E68_A7EA19636643__INCLUDED_)
