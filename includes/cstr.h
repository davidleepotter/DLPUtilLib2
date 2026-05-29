/*********************************************************************
*	File : CStr.cpp 
*	Desc : CString Class
*
*
*
*
*
*	Date: 8/01/2001
*********************************************************************/ 
#include <stdio.h>
#include <ctype.h>


#ifdef WIN32
	#include <windows.h>
#endif

#ifndef _CSTR_INCLUDED_
#define _CSTR_INCLUDED_

class DLP_CPtrArray;

#ifndef WIN32
	#include <unistd.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include "windows.h"
	#define UINT unsigned int
	#define BOOL bool
	#define BYTE char
#endif

#include <memory.h>
#include <wchar.h>

typedef unsigned int CPOS;

class CNetMutex;

class SDesc 
{
public:

	SDesc(int nLen)
	{
		m_WideText	= 0;
		m_Length	= 0;
		m_Alloc		= nLen+1;
		m_Text		= new char [m_Alloc];
		memset(m_Text,0,m_Alloc);
	}

	~SDesc()
	{
		if(m_Text != 0)
		{
			delete [] m_Text;
			m_Text = 0;
		}

		if(m_WideText != 0)
		{
			delete [] m_WideText;
			m_WideText = 0;
		}

	}

	// Data zone
	char*		m_Text;			// 0-terminated string, or next free SDesc* if m_Usage==0
	wchar_t*	m_WideText;		// 0-terminated wide string, or next free SDesc* if m_Usage==0
	CPOS		m_Length;		// Actual string length, excl. 0
	CPOS		m_Alloc;		// Allocated length, excl. 0
} ;




/*********************************************************************
* Class:	CStr
* Purpose:	Represents a string in your program.  Does not have virtual
*			methods and contains exactly 4 bytes (a pointer to a
*			data structure held internally by the string manager).
*			Can be typecasted to 'const char*' to obtain a read-only
*			zero-terminated copy of the string.  Can be copied and
*			assigned freely.
*********************************************************************/

class CStr
{
// Data block and internal access methods
private:
	SDesc* data;
	static unsigned int nGlobalCnt;
	unsigned long nUID;
	CNetMutex *poMutex;

// Construction, copying, assignment
public:
	CStr();
	CStr(const CStr& source);
	CStr(const char* s, CPOS prealloc = 0);
	CStr(CPOS prealloc);
	
	void operator=(const CStr *source);
	void operator=(const CStr& source);
	void operator=(const char* s);
	void operator=(char* s);

	~CStr();

	BOOL IsEmpty(void) const;
	CPOS GetLength(void) const;
	const char* GetString(void) const;			// Same as above
	char *GetString(void);
	//WCHAR * GetStringWide(void);


	void vFormatTString(LPCWSTR lpWideCharStr,int nLen)
	{

		CHAR *pVar =  new char[nLen+2];
		memset(pVar,0,nLen+1);
		WideCharToMultiByte( CP_ACP, 0, lpWideCharStr, -1, pVar, nLen, NULL, NULL );

		this->Format("%s",pVar);

		delete [] pVar;
	}


	/*********************************************************************
	* Proc:		Get String
	*********************************************************************/ 
	WCHAR * GetStringWide(void)
	{
		if(data->m_WideText != 0)
		{
			delete [] data->m_WideText;
			data->m_WideText = 0;
		}

			data->m_WideText = new WCHAR[data->m_Length+1];
			wmemset(data->m_WideText,0,data->m_Length+1);
		

			int nChars = 0;
			nChars = MultiByteToWideChar( 
				CP_ACP
				,0
				,data->m_Text
				,-1
				,data->m_WideText
				,data->m_Length+1); 
		
		
		return (WCHAR *)data->m_WideText;
	}

	char GetFirstChar() const;
	char GetLastChar() const;
	char operator[](CPOS idx) const;
	char *GetAtPos(CPOS idx) const;
	char GetAt(CPOS idx) const;				// Same as above
	void GetLeft (CPOS chars, CStr *result);
	void GetRight (CPOS chars, CStr *result);
	void GetMiddleStartEnd(CPOS start, CPOS end, CStr *result);
	void GetMiddle (CPOS start, CPOS chars, CStr *result);
	CStr Mid(CPOS start, CPOS length) const;
	CStr Mid(CPOS start) const { return Mid(start, GetLength() - start); }
	int  Find (char ch, CPOS startat = 0) const;
	int  Find(char *lpszSub, int nStart) const;
	int  ReverseFind (char ch, CPOS startat = (CPOS) -1) const;
	int  Compare (const char* match) const;			// -1, 0 or 1
	int  CompareNoCase (const char* match) const;	// -1, 0 or 1
	int nStringToIP(unsigned char *ptr);
	int nStringToIP(int *ptr);
 
	void  vToLower (void);
	void  vToUpper (void);

	int nReplaceAllInstances(char *src, char *dest);

	int nReplaceString(char *src, char *dest);
	int Replace(char ch, CPOS startat /*= 0*/) const;
	void NewFromString(const char* s, CPOS slen, CPOS prealloc);
	int nLoadFromFile(char *filename);
	int nWriteToFile(char *filename);
	int nReadLine(int nInclEmpty,int nMaxVal,FILE *fp);
	int nTokenize(DLP_CPtrArray *poArray,char delimiter);
	int nTokenize(DLP_CPtrArray *poArray,char delimiter,int nAllowDupe);

	// Operators == and != are also predefined

// Global modifications
	void Empty();			// Sets length to 0, but keeps buffer around
	void Reset();			// This also releases the buffer
	void GrowTo(CPOS size);
	void Compact(CPOS only_above = 0);
	void Format(const char* fmt, ...);

// Catenation, truncation
	void operator += (const CStr& obj);
	void operator += (const char* s);
	void operator += (char nChar);
	void operator += (char *nChar);

	void AddString(const CStr& obj);		// Same as +=
	void AddString(const char* s);			// Same as +=
	void AddChar(char ch);
	int InsertChar(const char* s, CPOS startat);
	int InsertChars(const char* s, CPOS startat,int nLen);
	int EraseChar(CPOS nPos);
	int nIsAlphaNum(CPOS nPos);
	void AddChars(const char* s, CPOS startat, CPOS howmany);
	void AddStringAtLeft(const CStr& obj);
	void AddStringAtLeft(const char* s);
	void AddInt(int value);
	void AddDouble(double value, UINT after_dot);
	void RemoveLeft(CPOS count);
	void RemoveMiddle(CPOS start, CPOS count);
	void RemoveRight(CPOS count);
	void TruncateAt(CPOS idx);
	friend CStr operator+(const CStr& s1, const CStr& s2);
	friend CStr operator+(const CStr& s, const char* lpsz);
	friend CStr operator+(const char* lpsz, const CStr& s);
	
	char *poGetDateTime(SYSTEMTIME *poTime);
	char *poGetDateTime(FILETIME *poTime);
	char *poGetDateTime(void);
	char *poGetDateTimeByTick(long nTick);
	char * GetBuffer(CPOS nMinBufLength);
	void ReleaseBuffer(CPOS nNewLength = -1);
	void vHexToDec(void);
	void vDecToHex(void);
	void vGetDiskSpace(char *drive);

	void vGetPathAndFile(char *string,CStr *poPath,CStr *poFileName);
	void vGetPathAndFile(CStr *poPath,CStr *poFileName);

	static int nCompareFiles(char *file1, char *file2);
	static long nGetFileSize(char *fileName);
	// These may be reimplemented, see below
	void ThrowIfNull(void* p);
	void ThrowPgmError(void);
	void ThrowNoUnicode(void);
	void ThrowBadIndex(int nStart, int nLen) const;
	void ThrowTooLarge(void);
	unsigned int nGetGlobalCount(void);
	unsigned int nGetUniqueID(void);





// Miscellaneous implementation methods
protected:
	void Buffer (CPOS newlength);
	void CoreAddChars(const char* s, CPOS howmany);
	void FormatCore (const char* x, va_list& marker);
	BOOL FmtOneValue (const char*& x, va_list& marker);

};

BOOL operator ==(const CStr& s1, const CStr& s2);
BOOL operator ==(const CStr& s1, LPCTSTR s2);
BOOL operator ==(LPCTSTR s1, const CStr& s2);
BOOL operator !=(const CStr& s1, const CStr& s2);
BOOL operator !=(const CStr& s1, LPCTSTR s2);
BOOL operator !=(LPCTSTR s1, const CStr& s2);


#endif		// _CSTR_INCLUDED_

