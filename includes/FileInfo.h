#if !defined(AFX_FILEINFO_H__4C620721_CFC7_11D2_AA42_A54123628E22__INCLUDED_)
#define AFX_FILEINFO_H__4C620721_CFC7_11D2_AA42_A54123628E22__INCLUDED_

#include <windows.h>

class DLP_CPtrArray;
class CStr;
class CDebugInfo;
class CDLPFileInfo;
class CDLPFileManager;

// Ensure uint64_t is defined for Windows builds
#ifndef _UINT64_DEFINED
typedef unsigned long long uint64_t;
#define _UINT64_DEFINED
#endif

// Ensure __int64 is defined for non-MSVC builds
#if defined(_MSC_VER)
// MSVC has __int64 built-in
#elif !defined(__DLP_INT64_DEFINED)
#define __DLP_INT64_DEFINED
typedef long long __int64;
#endif

typedef __int64 t64;

// Additional Windows types needed for Linux (only define if not already provided by Windows SDK)
#ifndef _ULONGLONG_DEFINED
typedef uint64_t ULONGLONG;
#define _ULONGLONG_DEFINED
#endif
#ifndef __LPARAM_DEFINED
#if defined(__LP64__) || defined(_LP64)
typedef long LPARAM;
#else
typedef int LPARAM;
#endif
#define __LPARAM_DEFINED
#endif
#ifndef __WPARAM_DEFINED
#if defined(__LP64__) || defined(_LP64)
typedef long WPARAM;
#else
typedef int WPARAM;
#endif
#define __WPARAM_DEFINED
#endif
#ifndef __LRESULT_DEFINED
typedef void* LRESULT;
#define __LRESULT_DEFINED
#endif

enum CFileInfoArraySort {
    AP_NOSORT = 0,
    AP_SORTBYNAME = 1,
    AP_SORTBYTYPE = 2,
    AP_SORTBYDATE = 3,
    AP_SORTBYSIZE = 4
};

#ifndef _DLP_NTSTATUS_DEFINED
typedef DWORD NTSTATUS;
#define _DLP_NTSTATUS_DEFINED
#endif

// WIN32_FIND_DATAA is already in windows.h, add alias
#ifndef WIN32
typedef WIN32_FIND_DATAA WIN32_FIND_DATA;
#endif

#define SECS_TO_FT_MULT 10000000

class CDLP_TIME
{
private:
	SYSTEMTIME oSystemTime;

public:
	CDLP_TIME();
	virtual ~CDLP_TIME();
	void vGetCurrentTime(void);
	void vSetTime(const FILETIME *poTime);
	CStr * poGetDate(void);

	void T64ToFileTime(t64 *pt,FILETIME *pft);
	void FileTimeToT64(FILETIME *pft,t64 *pt);
	t64 FindTimeTBase(void);
	void SystemTimeToT64(SYSTEMTIME *pst,t64 *pt);
	void T64ToSystemTime(t64 *pt,SYSTEMTIME *pst);
	t64 time_64(t64 *pt);
};


class CFileInfo 
{  
private:
	CStr *poFileName;
	CStr *poFileDrive;
	CStr *poFileDir;
	CStr *poFileTitle;
	CStr *poFileExt;
	CStr *poFileRoot;
	CStr *poFilePath;

public:
	friend CDLPFileManager;
	friend CDLP_TIME;
	friend CDLPFileInfo;
   CFileInfo();
   CFileInfo(const CFileInfo& finf);
   ~CFileInfo();
   void Create(const WIN32_FIND_DATA* pwfd, const char *strPath, LPARAM lParam=0);
   void Create(const char *strFilePath, LPARAM lParam = 0);
   DWORD GetChecksum(const ULONGLONG uhUpto=0, const BOOL bRecalc = FALSE, 
      const volatile BOOL* pbAbort=NULL, volatile ULONG* pulCount = NULL);
   DWORD GetCRC(const ULONGLONG dhUpto=0, const BOOL bRecalc = FALSE,
      const volatile BOOL* pbAbort=NULL, volatile ULONG* pulCount = NULL);
   DWORD GetLength(void) const { return (DWORD) m_uhFileSize; };
   ULONGLONG GetLength64(void) const { return m_uhFileSize; };
   char * pGetFileDrive(void);
   char * pGetFileDir(void);
   char * pGetFileTitle(void);
   char * pGetFileExt(void);
   char * pGetFileRoot(void); 
   char * pGetFileName(void);
   char * pGetFilePath(void);
   CDLP_TIME * poGetCreationTime(void) { return &m_timCreation; };
   CDLP_TIME * poGetLastAccessTime(void) { return &m_timLastAccess; };
   CDLP_TIME * poGetLastWriteTime(void) { return &m_timLastWrite; };
   DWORD GetAttributes(void) const { return m_dwAttributes; };
   BOOL IsDirectory(void) const { return m_dwAttributes & FILE_ATTRIBUTE_DIRECTORY; };
   BOOL IsArchived(void) const { return m_dwAttributes & FILE_ATTRIBUTE_ARCHIVE; };
   BOOL IsReadOnly(void) const { return m_dwAttributes & FILE_ATTRIBUTE_READONLY; };
   BOOL IsCompressed(void) const { return m_dwAttributes & FILE_ATTRIBUTE_COMPRESSED; };
   BOOL IsSystem(void) const { return m_dwAttributes & FILE_ATTRIBUTE_SYSTEM; };
   BOOL IsHidden(void) const { return m_dwAttributes & FILE_ATTRIBUTE_HIDDEN; };
   BOOL IsTemporary(void) const { return m_dwAttributes & FILE_ATTRIBUTE_TEMPORARY; };
   BOOL IsNormal(void) const { return m_dwAttributes == 0; };
   LPARAM m_lParam;
private:
   CStr m_strFilePath;
   DWORD m_dwAttributes;
   ULONGLONG m_uhFileSize;
   CDLP_TIME m_timCreation;
   CDLP_TIME m_timLastAccess;
   CDLP_TIME m_timLastWrite;
   DWORD m_dwChecksum;
   DWORD m_dwCRC;
   DWORD m_uhCRCBytes;
   DWORD m_uhChecksumBytes;
}; 

class CFileInfoArray 
{
private:
	CDebugInfo *poDebug;

public:
	DLP_CPtrArray *poArray;
   CFileInfoArray(CDebugInfo *poDebug);
   virtual ~CFileInfoArray();
   enum { 
      AP_NOSORT=0,
      AP_SORTASCENDING=0,
      AP_SORTDESCENDING=1,
      AP_SORTBYSIZE=2,
      AP_SORTBYNAME=4
   };
   int AddDir(const char * strDirName, const char * strMask, const BOOL bRecurse, 
      LPARAM lAddParam=(LPARAM)(void*)(long)AP_NOSORT, const BOOL bIncludeDirs=FALSE, 
      const volatile BOOL* pbAbort = NULL, volatile ULONG* pulCount = NULL);
   int AddFile(const char * strFilePath, LPARAM lAddParam);
protected:
   virtual int AddFileInfo(CFileInfo *finf, LPARAM lAddParam);
};


class CDLPFileInfo
{
private:
	CDebugInfo *poDebug;
	CFileInfo *poFileInfo;
	
public:
	unsigned long nUID;
	CStr *poFileName;	
	unsigned long nSize;
	CStr *poDate;		
	CStr *poVersion;		
	CStr *poPassword;	
	CStr *poCRC;
	CDLPFileInfo(CDebugInfo *poDebug);
	virtual ~CDLPFileInfo();
	unsigned long nGetUID(void);
	   void vSetInfo(unsigned long nUID,
					   			char *poFileName,	
					   			unsigned long nSize,
					   			char *poDate,		
					   			char *poVersion,		
					   			char *poPassword,	
					   			char *poCRC
					   			);
};


#endif // !defined(AFX_FILEINFO_H__4C620721_CFC7_11D2_AA42_A54123628E22__INCLUDED_)
