#ifndef UTILHEADER_H
#define UTILHEADER_H
/*********************************************************************
*	File : UtilHeader.h 
*	Desc : Global Header
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#ifdef WIN32
	#pragma warning( disable : 4786 )
#endif

#include "utildefines.h"

#include "objectcount.h"
#include "DLPRect.h"
#include "nettimer.h"
#include "timermanobj.h"
#include "netmutex.h"
#include "NewCol.h"
#include "cstr.h"
#include "ocrc32.h"
#include "ntoklib.h"
#include "DebugInfo.h"
#include "stringpool.h"
#include "PoolObj.h"
#include "Profiler.h"
#include "FileInfo.h"

#include "aracrypt.h"
#include "dlprnd.h"
#include "uidmanager.h"
#include "uniqueid.h"
#include "treeprint.h"
#include "DLP_Color.h"
#include "BuildConfig.h"
#include "memorymanager.h"
#include "driveinfo.h"
#include "KeyAuthObj.h"
#include "DLPFile.h"

#include "dbase.h"
#include "dbconpool.h"
#include "dbquery.h"
#endif