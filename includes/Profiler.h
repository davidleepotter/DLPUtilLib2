// Profiler.h: interface for the CProfiler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILER_H__E0AB55E1_A394_41CC_A8F6_8AAC3687A43D__INCLUDED_)
#define AFX_PROFILER_H__E0AB55E1_A394_41CC_A8F6_8AAC3687A43D__INCLUDED_

class CDebugInfo;
class DLP_CPtrArray;
class CProfiler;

#include "timermanobj.h"

// Linux stubs for MSVC-specific types and intrinsics
#if defined(_MSC_VER)
// MSVC has __int64 built-in
#elif !defined(__DLP_INT64_DEFINED)
#define __DLP_INT64_DEFINED
typedef long long __int64;
#endif

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
	uint64_t nStartVal;		//End cycle count
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
			uint64_t nStart = rdtsc_stub();
			
			nStartTime = CTimerManObj::nGetCurrent();

			nStartVal = nStart;
		};


	inline void vExit(void)
	{
		uint64_t nEnd = rdtsc_stub();
		nStopTime = CTimerManObj::nGetCurrent();
		nDuration = nStopTime - nStartTime;

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
