/*********************************************************************
*	File : DebugInfo.cpp 
*	Desc : Debug Info out to a log file
*
*	Comment:
*
*
*
*	Date: 8/01/2001
*********************************************************************/ 

#if !defined(AFX_DEBUGINFO_H__437F0082_BE8D_4B77_8954_A87A006856D3__INCLUDED_)
#define AFX_DEBUGINFO_H__437F0082_BE8D_4B77_8954_A87A006856D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStr;

#include <stdio.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#ifndef WIN32
#include <pthread.h>
#include <stdarg.h>
#endif

class DLP_CPtrList;

class CDebugInfo 
{
private:
	DLP_CPtrList *poPrintList;

	CStr *outFile;
	CStr *dataBuffer;
	FILE *fp;
	char *tempBuffer;
	unsigned int nCleanID;
	unsigned int nInfoID;
	unsigned int nErrorID;
	unsigned int nDataID;
	int nDefaultLevel;
	int nLock;

#ifdef WIN32
	CRITICAL_SECTION     *oCritSect;
#endif

#ifndef WIN32
	pthread_mutex_t		*oCritSect;
#endif

	void vSetDebugMutex(void);
	void vClearDebugMutex(void);
public:
	CDebugInfo();
	CDebugInfo(char *filename);
	CDebugInfo(char *filename,char *path);
	virtual ~CDebugInfo();
	void vPrintf(int nLevel,int nType,const char* fmt,...);
	void vOpenDebugFile(char *filename);
	void vOpenDebugFile(char *path,char *filename);
	void vSetDefaultLevel(int nLevel);
	void vPrintString(CStr *poString);
	void vFlushAllPrints(void);
	//Call Back
	void (*callback)(int nType, int nId, char *string);

};



#endif // !defined(AFX_DEBUGINFO_H__437F0082_BE8D_4B77_8954_A87A006856D3__INCLUDED_)
