/*********************************************************************
*	File : NetTimer.h 
*	Desc : Timer Mix In Class
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 
#if !defined(AFX_NETTIMER_H__BEE4E89B_D29F_40FD_8964_ED4EABD56720__INCLUDED_)
#define AFX_NETTIMER_H__BEE4E89B_D29F_40FD_8964_ED4EABD56720__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimerManObj;
class CDebugInfo;

class CNetTimer  
{

private:
	int n_param;					//Int Param - uID of msg
	void		*poVoidPointer1;	//Void Pointer 1
	void		*poVoidPointer2;	//Void Pointer 2
	CNetTimer	*o_NetTimer;		//NetTimer
	static unsigned long g_uID1;	//Global uid array
	bool 		b_paused;
	int nInTime;
	unsigned long nSetUid(void)
	{
		g_uID1++;
		return g_uID1;
	}

	bool b_inuse;					//Inuse	
	bool b_loop;					//Loop Timer
	int nState;
	unsigned long ul_time_val;		//Timer val
	unsigned long n_uID;			//local uid
	unsigned long ulLifeTime;		//LifeTime of timer
	unsigned long ul_createTime;	//Timer in ms 

	bool bCheckTimer(CDebugInfo *poDebug);
	unsigned long uGetGlobTickCount(void);
	//Call Back
	void (*callback)(unsigned long n_val,void *poVoidPointer1,void *poVoidPointer2,CNetTimer *o_NetTimer);
	void vClearTimer(void);
	void vStopTimer();

public:

	friend CTimerManObj;

	void vPauseTimer(void);
	void vStartTimer(void);
	bool bIsPaused(void);
	void vSetTimer(unsigned long ul_val,void (*callback)(unsigned long,void *,void *,CNetTimer *),unsigned long n_tparam,void *poVoidPointer1,void *poVoidPointe2,CNetTimer *o_NetTimer,bool loop);
	void vModTimer(unsigned long nVal);
	void vSetLoop(bool b_tloop);
	void vReSetTimer(void);
	unsigned long ulGetLifeTime(void);

	CNetTimer();
	virtual ~CNetTimer();

};

#endif // !defined(AFX_NETTIMER_H__BEE4E89B_D29F_40FD_8964_ED4EABD56720__INCLUDED_)
