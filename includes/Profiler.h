// Profiler.h: interface for the CProfiler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILER_H__E0AB55E1_A394_41CC_A8F6_8AAC3687A43D__INCLUDED_)
#define AFX_PROFILER_H__E0AB55E1_A394_41CC_A8F6_8AAC3687A43D__INCLUDED_

class CDebugInfo;
class DLP_CPtrArray;
class CProfiler;

#include "TimerManObj.h"

#ifdef WIN32
#pragma warning( disable : 4244)
#endif

class CProFileItem
{
private:

	int nEntered;
	unsigned short Count;		//Number of bytes in per record
	unsigned short Line;		//Line number in source file
	unsigned long  ProcessID;	//Current process ID
	unsigned long  ThreadID;	//Current thread ID
	//Start and finsih clock ticks
	unsigned __int64 nStartVal;		//End cycle count
	int Cycles;	//NULL for function start
	CStr  *poSource;			//Source file path and name
	CStr *poFunction;			//Function that was profiled
	unsigned long nStartTime;
	unsigned long nStopTime;
	unsigned long nDuration;

public:
	friend CProfiler;
	CProFileItem(char *name,char *file,int nLine,int nEnter);
	~CProFileItem();

	inline void vEnter(void)
		{
			unsigned __int64 nStart;		//End cycle count
			
			__asm rdtsc						
			__asm mov dword ptr [nStart+0], eax
			__asm mov dword ptr [nStart+4], edx

			nStartTime = CTimerManObj::nGetCurrent();

			nStartVal = nStart;
		};


	inline void vExit(void)
	{
		unsigned __int64 nEnd;		//End cycle count
		nStopTime = CTimerManObj::nGetCurrent();
		nDuration = nStopTime - nStartTime;

		//__asm cpuid					
		__asm rdtsc					
		__asm mov dword ptr [nEnd+0], eax	//Read low
		__asm mov dword ptr [nEnd+4], edx	//Read hi	 

		Cycles = (int)nEnd-nStartVal;


	}


	unsigned long nGetDuration(void);
	int nGetCycles(void);
	void vPrintItemInfo(CDebugInfo *poDebug);


};


class CProfiler  
{
private:
	CDebugInfo *poDebug;
	DLP_CPtrArray *poProfileArray;

public:
	CProfiler(CDebugInfo *poDebug);
	virtual ~CProfiler();

	void vAddItem(CProFileItem *poItem);
	void vRemoveItem(CProFileItem *poItem);
	void vDumpItems(void);

};

#endif // !defined(AFX_PROFILER_H__E0AB55E1_A394_41CC_A8F6_8AAC3687A43D__INCLUDED_)
