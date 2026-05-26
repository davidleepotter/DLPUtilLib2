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

#include "debuginfo.h"
#include "utildefines.h"
#include "cstr.h"
#include "newcol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDebugInfo::CDebugInfo()
{
	fp = 0;
	tempBuffer = new char[10240];
	outFile = new CStr;
	
	nCleanID		= 0;
	nInfoID			= 0;
	nErrorID		= 0;
	nDataID			= 0;
	callback		= 0;
	nLock			= 0;
	nDefaultLevel	= DEBUG_LEVEL1;
	poPrintList		= new DLP_CPtrList(0);

#ifdef WIN32
	oCritSect = new CRITICAL_SECTION;
	InitializeCriticalSection(oCritSect);
#endif

#ifndef WIN32
	oCritSect = new pthread_mutex_t;
 
 int res = pthread_mutex_init(oCritSect, NULL);
    if (res != 0) 
	{
	}

#endif
}
 
CDebugInfo::CDebugInfo(char *filename)
{
	fp = 0;
	tempBuffer = new char[10240];
	outFile = new CStr;
	
	nCleanID		= 0;
	nInfoID			= 0;
	nErrorID		= 0;
	nDataID			= 0;
	callback		= 0;
	nLock			= 0;
	nDefaultLevel	= DEBUG_LEVEL1;
	poPrintList		= new DLP_CPtrList(0);

	vOpenDebugFile(filename);
#ifdef WIN32
	oCritSect = new CRITICAL_SECTION;
	InitializeCriticalSection(oCritSect);
#endif

#ifndef WIN32
	oCritSect = new pthread_mutex_t;
 
 int res = pthread_mutex_init(oCritSect, NULL);
    if (res != 0) 
	{
	}

#endif
}

CDebugInfo::CDebugInfo(char *filename,char *path)
{
	fp = 0;
	tempBuffer = new char[10240];
	outFile = new CStr;
	
	nCleanID		= 0;
	nInfoID			= 0;
	nErrorID		= 0;
	nDataID			= 0;
	callback		= 0;
	nLock			= 0;
	nDefaultLevel	= DEBUG_LEVEL1;
	poPrintList		= new DLP_CPtrList(0);

	vOpenDebugFile(path,filename);
#ifdef WIN32
	oCritSect = new CRITICAL_SECTION;
	InitializeCriticalSection(oCritSect);
#endif

#ifndef WIN32
	oCritSect = new pthread_mutex_t;
 
 int res = pthread_mutex_init(oCritSect, NULL);
    if (res != 0) 
	{
	}

#endif
}



CDebugInfo::~CDebugInfo()
{

	vFlushAllPrints();
	DLP_CPtrListItem *plt = poPrintList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CStr *poStr = (CStr *)plt->pGetPtr();
			delete poStr;

			plt = nextPlt;
	    }

	poPrintList->vRemoveAll();
	delete poPrintList;
	poPrintList = 0;



callback = 0;
delete outFile;
delete [] tempBuffer;

	if(fp !=0)
	{
		fclose(fp);		
	}

#ifdef WIN32
	DeleteCriticalSection(oCritSect);
	delete oCritSect;
#endif

#ifndef WIN32
	pthread_mutex_destroy(oCritSect);
	delete oCritSect;
#endif

}


//////////////////////////////////////////////////////
//
//	Open Debug File
//
//////////////////////////////////////////////////////
void CDebugInfo::vOpenDebugFile(char *filename)
{

	#ifdef WIN32
		outFile->Format(".\\Logs\\%s",filename);
	#endif
		
	#ifndef WIN32
		outFile->Format("%s",filename);
	#endif

	outFile->vToLower();

	//If already open close it
	if(fp != 0)
	{
		fclose(fp);
	}

	//Open it for write
	fp = fopen(outFile->GetString(),"w+");

	if(fp == 0)
	{
		return;
	}
	fclose(fp);
	
	//and open it for reading
	fp = fopen(outFile->GetString(),"r+");
	fflush(fp);

}

//////////////////////////////////////////////////////
//
//	Open Debug File
//
//////////////////////////////////////////////////////
void CDebugInfo::vOpenDebugFile(char *pPath,char *filename)
{

CStr oPath;

	if(pPath != 0)
	{
		oPath.Format("%s",pPath);
	}
			
			
	if(oPath.GetLength() > 0)
	{
		#ifdef WIN32
			outFile->Format("%s\\Logs\\%s",oPath.GetString(),filename);
		#endif
			
		#ifndef WIN32
			outFile->Format("%s/%s",oPath.GetString(),filename);
		#endif
	}
	else
	{
		#ifdef WIN32
			outFile->Format(".\\Logs\\%s",filename);
		#endif
			
		#ifndef WIN32
			outFile->Format("%s",filename);
		#endif
	}

	outFile->vToLower();

	//If already open close it
	if(fp != 0)
	{
		fclose(fp);
	}

	//Open it for write
	fp = fopen(outFile->GetString(),"w+");


	if(fp == 0)
	{
		return;
	}

	fclose(fp);
	
	//and open it for reading
	fp = fopen(outFile->GetString(),"r+");
	fflush(fp);

}

//////////////////////////////////////////////////////
//
//	Print Debug Info
//
//////////////////////////////////////////////////////
void CDebugInfo::vPrintf(int nLevel,int nType,const char* fmt,...)
{
	if(oCritSect == 0)
	{
		return;
	}

	vSetDebugMutex();
	if(nLock == 1)
	{
		int nError = 1;
		fflush(fp);
		vClearDebugMutex();
		return;
	}

	if(fp == 0)
	{
		fflush(fp);
		vClearDebugMutex();
		nLock = 0;
		return;
	}

	if(nLevel > nDefaultLevel)
	{
		fflush(fp);
		vClearDebugMutex();
		nLock = 0;
		return;
	}

	nLock = 1;
	//CStr *poString = new CStr;

	memset(tempBuffer,0,10240);

    va_list ap;
    va_start(ap,fmt);

    vsprintf(tempBuffer,fmt,ap);
	va_end(ap);

	unsigned int nID;	
	switch(nType)
	{
		case DEBUG_CLEAN:
			{
				++nCleanID;
				nID = nCleanID;

			}
			break;


		case DEBUG_INFO:
			{
				fwrite("INFO - ",strlen("INFO - "),1,fp);
				++nInfoID;
				nID = nInfoID;

			}
			break;

		case DEBUG_ERROR:
			{
				fwrite("ERROR - ",strlen("ERROR - "),1,fp);
				++nErrorID;
				nID = nErrorID;

			}
			break;
		case DEBUG_DATA:
			{
				fwrite("DATA - ",strlen("DATA - "),1,fp);
				++nDataID;
				nID = nDataID;
			}
			break;

		default:
			{
			}
			break;	
	}


	//Add to PrintList
	//poPrintList->vAddTail(poString);
	

	fwrite(tempBuffer,strlen(tempBuffer),1,fp);
	fflush(fp);
	
	if(callback != 0)
	{
		(*callback)(nType,nID,&tempBuffer[0]);
	}

	fflush(fp);

	nLock = 0;
	vClearDebugMutex();
}


//////////////////////////////////////////////////////
//
//	Set Default Level
//
//////////////////////////////////////////////////////
void CDebugInfo::vSetDefaultLevel(int nLevel)
{
	nDefaultLevel = nLevel;

}


//////////////////////////////////////////////////////
//
//	Set Mutex
//
//////////////////////////////////////////////////////
void CDebugInfo::vSetDebugMutex(void)
{
#ifdef WIN32
	EnterCriticalSection(oCritSect);
#endif

#ifndef WIN32
	pthread_mutex_lock(oCritSect);
#endif

}

//////////////////////////////////////////////////////
//
//	Clear Mutex
//
//////////////////////////////////////////////////////
void CDebugInfo::vClearDebugMutex(void)
{
#ifdef WIN32
	LeaveCriticalSection(oCritSect);
#endif

#ifndef WIN32
	pthread_mutex_unlock(oCritSect);
#endif
	
}

//////////////////////////////////////////////////////
//
//	Print String
//
//////////////////////////////////////////////////////
void CDebugInfo::vPrintString(CStr *poString)
{
	if(fp == 0)
	{
		return;
	}
}


//////////////////////////////////////////////////////
//
//	Flush All Prints
//
//////////////////////////////////////////////////////
void CDebugInfo::vFlushAllPrints(void)
{
	DLP_CPtrListItem *plt = poPrintList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CStr *poStr = (CStr *)plt->pGetPtr();
			vPrintString(poStr);
			plt = nextPlt;
	    }

}
