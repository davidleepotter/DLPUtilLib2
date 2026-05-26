#pragma once
/*********************************************************************
*	File : DLPFile.h
*	Desc : DLP File Class
*
*	Comment: 
*
*
*
*	Date: 2/7/2005
*********************************************************************/
#include <list>
#include <string>
#include <algorithm>

class CDebugInfo;
class DLP_CPtrArray;
class CDLPFile;
class ARACrypt;

class FileGlobBase
{
public:
	FileGlobBase();

	void MatchPattern( const char* inPattern );
	void AddExclusivePattern( const char* name );
	void AddIgnorePattern( const char* name );

	virtual void FoundMatch( const char* name ) = 0;

protected:
	bool MatchExclusivePattern( const char* name );
	bool MatchIgnorePattern( const char* name );
	void GlobHelper( const char* inPattern );

private:
	typedef std::list< std::string > StringList;

	StringList m_exclusiveFilePatterns;
	StringList m_ignorePatterns;
};

class CDLPFileItem
{
private:
	CDebugInfo *poDebug;
	CDLPFile *poParent;
	CStr *poPath;
	CStr *poFileName;
	long nSize;
	long nOffset;
	
	long nMinSize;

	char *data;
	CStr *poModifiedDate;
	CStr *poCreationDate;

public:
	friend CDLPFile;
	CDLPFileItem(CDLPFile* potParent,CDebugInfo *potDebug);
	~CDLPFileItem();

	char *poGetModifiedDate(void)
	{
		return poModifiedDate->GetString();
	}

	char *poGetCreationDate(void)
	{
		return poCreationDate->GetString();
	}

	long nGetSize(void)
	{
		return nSize;
	}

	long nGetOffSet(void)
	{
		return nOffset;
	}
	
	char *pGetData(void)
	{
		return data;
	}
	
	char *poGetFileName(void)
	{
		return poFileName->GetString();
	}

};

class CDLPFile : public FileGlobBase
{
private:
	CDebugInfo *poDebug;
	DLP_CPtrArray *poFileArray;
	virtual void FoundMatch( const char* fileName );
	int nWriteDataHeader(FILE *fp,int nMinSize);
	ARACrypt *poCrypt;
public:
	friend CDLPFileItem;
	CDLPFile(CDebugInfo *potDebug);
	virtual ~CDLPFile(void);

	void vLoadFile(char *name);
	void vFreeData(void);

	
	void (*vFileAdded)(void *poVoidPointer1,void *poVoidPointer2);
	int nCreateCBNFile(char *srcdir,char *destName, char *key,int nMinSize);
	CDLPFileItem *poFindFile(char *srcName,char *name, char *key);
	CDLPFileItem *poFindFile(unsigned char *pData,char *name, char *key);
	CDLPFileItem *poFindFile(char *srcName);

	int nGetMaxNumberofItems(unsigned char *pData);

	long nFileSize;
	unsigned char *pData;

};

/*
class CDLPFileList : public FileGlobBase
{
private:
	CDebugInfo *poDebug;
	DLP_CPtrArray *poFileArray;
	virtual void FoundMatch( const char* fileName );

public:
	CDLPFileList(CDebugInfo *potDebug)
	~CDLPFileList()

};
*/