# Microsoft Developer Studio Project File - Name="DLPUtilLib2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DLPUtilLib2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DLPUtilLib2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DLPUtilLib2.mak" CFG="DLPUtilLib2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DLPUtilLib2 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DLPUtilLib2 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DLPUtilLib2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GR /GX /O2 /Ob2 /I "$(DLPUtilLib2)\DLPUtilLib2\Includes" /I "$(DLPUtilLib2)\DLPUtilLib2\includes\mysqlwin" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX"UtilHeader.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libs\DLPUtilLib2Rel.lib"

!ELSEIF  "$(CFG)" == "DLPUtilLib2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "$(DLPUtilLib2)\Includes" /I "$(DLPUtilLib2)\includes\mysqlwin" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"./Libs/DLPUtilLib2.lib"

!ENDIF 

# Begin Target

# Name "DLPUtilLib2 - Win32 Release"
# Name "DLPUtilLib2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\aracrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\src\BuildConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cstr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DBConPool.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dbflatfile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dbmysql.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dbodbc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dbquery.cpp
# End Source File
# Begin Source File

SOURCE=.\src\debuginfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DLP_Color.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DLPFile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DLPFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DLPRect.cpp
# End Source File
# Begin Source File

SOURCE=.\src\dlprnd.cpp
# End Source File
# Begin Source File

SOURCE=.\src\driveInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FileTransfer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GenNetMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\HTTPRequest.cpp
# End Source File
# Begin Source File

SOURCE=.\src\KeyAuthObj.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NetFTP.cpp
# End Source File
# Begin Source File

SOURCE=.\src\netmutex.cpp
# End Source File
# Begin Source File

SOURCE=.\src\nettimer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\newcol.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ntoklib.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ocrc32.cpp
# End Source File
# Begin Source File

SOURCE=.\src\odbcrecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\src\poolobj.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Profiler.cpp
# End Source File
# Begin Source File

SOURCE=.\src\socketportal.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SocketPortal2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SocketPortalLite.cpp
# End Source File
# Begin Source File

SOURCE=.\src\stringpool.cpp
# End Source File
# Begin Source File

SOURCE=.\src\timermanobj.cpp
# End Source File
# Begin Source File

SOURCE=.\src\treeprint.cpp
# End Source File
# Begin Source File

SOURCE=.\src\uidmanager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\uniqueid.cpp
# End Source File
# Begin Source File

SOURCE=.\src\WebAuth.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Includes\ARACrypt.h
# End Source File
# Begin Source File

SOURCE=.\includes\BuildConfig.h
# End Source File
# Begin Source File

SOURCE=.\Includes\CallBack.h
# End Source File
# Begin Source File

SOURCE=.\Includes\CStr.h
# End Source File
# Begin Source File

SOURCE=.\Includes\Dbase.h
# End Source File
# Begin Source File

SOURCE=.\includes\DBConPool.h
# End Source File
# Begin Source File

SOURCE=.\Includes\DBFlatFile.h
# End Source File
# Begin Source File

SOURCE=.\Includes\DBMySql.h
# End Source File
# Begin Source File

SOURCE=.\Includes\DBODBC.h
# End Source File
# Begin Source File

SOURCE=.\includes\dbquery.h
# End Source File
# Begin Source File

SOURCE=.\Includes\DebugInfo.h
# End Source File
# Begin Source File

SOURCE=.\includes\deflate.h
# End Source File
# Begin Source File

SOURCE=.\includes\DLP_Color.h
# End Source File
# Begin Source File

SOURCE=.\includes\DLPFile.h
# End Source File
# Begin Source File

SOURCE=.\includes\DLPFileManager.h
# End Source File
# Begin Source File

SOURCE=.\includes\DLPRect.h
# End Source File
# Begin Source File

SOURCE=.\includes\dlprnd.h
# End Source File
# Begin Source File

SOURCE=.\includes\driveinfo.h
# End Source File
# Begin Source File

SOURCE=.\includes\drivestructs.h
# End Source File
# Begin Source File

SOURCE=.\includes\FileInfo.h
# End Source File
# Begin Source File

SOURCE=.\includes\FileTransfer.h
# End Source File
# Begin Source File

SOURCE=.\includes\GenNetMsg.h
# End Source File
# Begin Source File

SOURCE=.\includes\HTTPRequest.h
# End Source File
# Begin Source File

SOURCE=.\includes\infblock.h
# End Source File
# Begin Source File

SOURCE=.\includes\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\includes\inffast.h
# End Source File
# Begin Source File

SOURCE=.\includes\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\includes\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\includes\infutil.h
# End Source File
# Begin Source File

SOURCE=.\includes\KeyAuthObj.h
# End Source File
# Begin Source File

SOURCE=.\includes\memorymanager.cpp
# End Source File
# Begin Source File

SOURCE=.\includes\memorymanager.h
# End Source File
# Begin Source File

SOURCE=.\includes\NetFTP.h
# End Source File
# Begin Source File

SOURCE=.\Includes\NetMutex.h
# End Source File
# Begin Source File

SOURCE=.\Includes\NetTimer.h
# End Source File
# Begin Source File

SOURCE=.\Includes\NewCol.h
# End Source File
# Begin Source File

SOURCE=.\Includes\NToklib.h
# End Source File
# Begin Source File

SOURCE=.\includes\objectcount.h
# End Source File
# Begin Source File

SOURCE=.\Includes\OCRC32.h
# End Source File
# Begin Source File

SOURCE=.\Includes\ODBCRecordset.h
# End Source File
# Begin Source File

SOURCE=.\includes\poolobj.h
# End Source File
# Begin Source File

SOURCE=.\includes\Profiler.h
# End Source File
# Begin Source File

SOURCE=.\includes\socketportal.h
# End Source File
# Begin Source File

SOURCE=.\includes\SocketPortal2.h
# End Source File
# Begin Source File

SOURCE=.\includes\SocketPortalLite.h
# End Source File
# Begin Source File

SOURCE=.\includes\stringpool.h
# End Source File
# Begin Source File

SOURCE=.\Includes\TimerManObj.h
# End Source File
# Begin Source File

SOURCE=.\includes\treeprint.h
# End Source File
# Begin Source File

SOURCE=.\includes\trees.h
# End Source File
# Begin Source File

SOURCE=.\includes\uidmanager.h
# End Source File
# Begin Source File

SOURCE=.\includes\uniqueid.h
# End Source File
# Begin Source File

SOURCE=.\Includes\UtilDefines.h
# End Source File
# Begin Source File

SOURCE=.\Includes\UtilHeader.h
# End Source File
# Begin Source File

SOURCE=.\includes\WebAuth.h
# End Source File
# Begin Source File

SOURCE=.\includes\zconf.h
# End Source File
# Begin Source File

SOURCE=.\includes\zlib.h
# End Source File
# Begin Source File

SOURCE=.\includes\zutil.h
# End Source File
# End Group
# End Target
# End Project
