/*********************************************************************
*	File : CProfiler.cpp 
*	Desc : Profiler Class
*
*	Comment: 
*
*
*
*	Date: 8/02/2004
*********************************************************************/ 
#include "UtilHeader.h"


//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CProFileItem::CProFileItem(char *name,char *file,int nLine,int nEnter)
{
	Count			= 0;					//Number of bytes in per record
	Line			= 0;					//Line number in source file
	ProcessID		= GetCurrentProcessId();//Current process ID
	ThreadID		= GetCurrentThreadId(); //Current thread ID
	Cycles			= 0;					//NULL for function start
	poSource		= new CStr;				//Source file path and name
	poFunction		= new CStr;				//Function
	nEntered		= 0;
	nStartVal		= 0;
	nStartTime		= 0;
	nStopTime		= 0;
	nDuration		= 0;
	
	poFunction->Format("%s",name);
	poSource->Format("%s",file);
	Line = nLine;

	if(nEnter == 1)
	{
		//vEnter();
	}

}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CProFileItem::~CProFileItem()
{
	if(nEntered == 1)
	{
		//vExit();
	}

	delete poSource;
	delete poFunction;
}
/*
//////////////////////////////////////////////////////
//
//	vEnter
//
//////////////////////////////////////////////////////
void CProFileItem::vEnter(void)
{
	//Use local variable to reference class member
	unsigned __int64 nStart = 0;	

	__asm cpuid						
	__asm rdtsc						
	__asm mov dword ptr [nStart+0], eax
	__asm mov dword ptr [nStart+4], edx
	m_nTotal = nStart;	//Cheat a little burn off 4 intructions
	nEntered = 1;
}

//////////////////////////////////////////////////////
//
//	vExit
//
//////////////////////////////////////////////////////
void CProFileItem::vExit(void)
{
	unsigned __int64 nEnd = 0;	//End cycle count

	__asm cpuid					
	__asm rdtsc					
	__asm mov dword ptr [nEnd+0], eax	//Read low
	__asm mov dword ptr [nEnd+4], edx	//Read hi	 

	Cycles = nEnd-m_nTotal;
}
*/
 
//////////////////////////////////////////////////////
//
//	nGetCycles
//
//////////////////////////////////////////////////////
int CProFileItem::nGetCycles(void)
{
	return Cycles;
}


//////////////////////////////////////////////////////
//
//	nGetDuration
//
//////////////////////////////////////////////////////
unsigned long CProFileItem::nGetDuration(void)
{
	return nDuration;
}

//////////////////////////////////////////////////////
//
//	vPrintItemInfo
//
//////////////////////////////////////////////////////
void CProFileItem::vPrintItemInfo(CDebugInfo *poDebug)
{  
	poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO
		,"Name:%s\nSrc:%s\nLine:%d\nCycles:%d\nProcId:%d\nThreadID:%d\nDuration:%d\n"
		,poSource->GetString()
		,poFunction->GetString()
		,Line
		,Cycles
		,ProcessID
		,ThreadID
		,nDuration);

}


//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CProfiler::CProfiler(CDebugInfo *potDebug)
{
	poDebug = potDebug;
	poProfileArray =  new DLP_CPtrArray;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CProfiler::~CProfiler()
{
	for(int i=0; i<poProfileArray->nGetSize(); i++)
	{
		CProFileItem *poItem = (CProFileItem *)poProfileArray->pGetAt(i);
		delete poItem;
	}

	poProfileArray->vRemoveAll();
	delete poProfileArray;
}

//////////////////////////////////////////////////////
//
//	vAddItem
//
//////////////////////////////////////////////////////
void CProfiler::vAddItem(CProFileItem *poItem)
{
	poProfileArray->nAdd(poItem);

}

//////////////////////////////////////////////////////
//
//	vRemoveItem
//
//////////////////////////////////////////////////////
void CProfiler::vRemoveItem(CProFileItem *potItem)
{
	for(int i=0; i<poProfileArray->nGetSize(); i++)
	{
		CProFileItem *poItem = (CProFileItem *)poProfileArray->pGetAt(i);
		
		if(poItem == potItem)
		{
			delete poItem;
			poProfileArray->vRemoveAt(i);
			return;
		}
	}

}

//////////////////////////////////////////////////////
//
//	vDumpItems
//
//////////////////////////////////////////////////////
void CProfiler::vDumpItems(void)
{
	for(int i=0; i<poProfileArray->nGetSize(); i++)
	{
		CProFileItem *poItem = (CProFileItem *)poProfileArray->pGetAt(i);
		poItem->vPrintItemInfo(poDebug);
	}

}