/*********************************************************************
*	File : DLPFile.cpp
*	Desc : DLP File Class
*
*	Comment: 
*
*
*
*	Date: 2/7/2005
*********************************************************************/
#include "UtilHeader.h"
#include <assert.h>
 
//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CDLPFileItem::CDLPFileItem(CDLPFile* potParent,CDebugInfo *potDebug)
{
	poParent = potParent;
	poDebug = potDebug;

	poPath = new CStr;
	poFileName = new CStr;
	nSize = -1;
	nOffset = -1;
	data = 0;

}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CDLPFileItem::~CDLPFileItem(void)
{
	delete poPath;
	poPath = 0;

	delete poFileName;
	poFileName = 0;
	
	if(data != 0)
	{
		delete [] data;
		data = 0;
	}
}

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CDLPFile::CDLPFile(CDebugInfo *potDebug)
{
	poDebug = potDebug;
	poFileArray = new DLP_CPtrArray();
	poCrypt = new ARACrypt();
	vFileAdded = 0;
	pData = 0;
	nFileSize = -1;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CDLPFile::~CDLPFile(void)
{
	for(int i=0; i<poFileArray->nGetSize(); i++)
	{
		CDLPFileItem *poItem = (CDLPFileItem *)poFileArray->pGetAt(i);
		delete poItem;
	}

	poFileArray->vRemoveAll();
	delete poFileArray;
	poFileArray = 0;
	
	delete poCrypt;
	poCrypt = 0;

	if(pData != 0)
	{
		delete [] pData;
	}

}

//////////////////////////////////////////////////////
//
//	vLoadFile
//
//////////////////////////////////////////////////////
void CDLPFile::vLoadFile(char *name)
{
	if(pData != 0)
	{
		vFreeData();
	}

	FILE *fp = 0;
	fp = fopen("name","rb+");

	CStr oStr;
	nFileSize = oStr.nGetFileSize(name);
	pData = new unsigned char[nFileSize];
	fread(pData,1,nFileSize,fp);
	fclose(fp);
}

//////////////////////////////////////////////////////
//
//	vFreeData
//
//////////////////////////////////////////////////////
void CDLPFile::vFreeData(void)
{
	if(pData != 0)
	{
		delete [] pData;
	}
	pData = 0;
	nFileSize = -1;
}


//////////////////////////////////////////////////////
//
//	FoundMatch
//
//////////////////////////////////////////////////////
void CDLPFile::FoundMatch( const char* fileName )
{
	CStr oStr;
	oStr.Format("%s",fileName);
	long nSize = 0;
	nSize = oStr.nGetFileSize((char *)fileName);

	if(nSize <= 0)
	{
		return;
	}

	CDLPFileItem *poItem = new CDLPFileItem(this,poDebug);
	oStr.vGetPathAndFile(poItem->poPath,poItem->poFileName);
	poItem->nSize = nSize;

	poFileArray->nAdd(poItem);

	if(poDebug != 0)
	{
		poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO
			,"Found File [%s][%s] - %s\n"
			,poItem->poPath->GetString()
			,poItem->poFileName->GetString()
			,fileName);
	}

};

//////////////////////////////////////////////////////
//
//	vCreateCBNFile
//
//////////////////////////////////////////////////////
int CDLPFile::nCreateCBNFile(char *srcdir, char *destName, char *key,int nMinSize)
{
	CStr oStr;
	oStr.Format("%s\\*",srcdir);
	MatchPattern(oStr.GetString());	

	FILE *fp = 0;
	fp= fopen(destName,"wb+");
	
	if(fp == 0)
	{
		if(poDebug != 0)
		{
			poDebug->vPrintf(DEBUG_LEVEL5,DEBUG_ERROR
			,"Could not open file %s\n"
			,destName);
		}

		return 0;
	}

	long nStartOffSet = 0;
	long nCurOffSet = 0;

	int nVal = 0;
	nVal = nWriteDataHeader(fp,nMinSize);

	if(poDebug != 0)
	{
		poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO
		,"HeaderSize =  %d\n"
		,nVal);
	}


	for(int i=0; i<poFileArray->nGetSize(); i++)
	{
		CDLPFileItem *poItem = (CDLPFileItem *)poFileArray->pGetAt(i);

		FILE *fp1 = 0;
		oStr.Format("%s\\%s"
		,poItem->poPath->GetString()
		,poItem->poFileName->GetString());

		fp1 = fopen(oStr.GetString(),"rb");
		
		if(fp1 == 0)
		{
			if(poDebug != 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL5,DEBUG_ERROR
				,"Could not open file %s\n"
				,oStr.GetString());
			}

			continue;
		}

		if(poItem->nSize == -1)
		{
			if(poDebug != 0)
			{
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR
				,"Bad File Read %s\n"
				,oStr.GetString());
			}

			continue;
		}

		char *temp = new char[poItem->nSize];
		memset(temp,0,poItem->nSize);
		
		char *temp1 = new char[nMinSize];
		memset(temp1,0,nMinSize);

		char *temp2 = new char[nMinSize];
		memset(temp2,0,nMinSize);


		long nVal = fread(temp,poItem->nSize,1,fp1);

		if(poDebug != 0)
		{
			poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO
				,"Read file %s - [%d][%d]\n"
				,oStr.GetString()
				,nVal
				,poItem->nSize);
		}

		fclose(fp1);

		if(poItem->nSize <= nMinSize)
		{
			assert(0);
		}

//Encrypt
#if 1

		//Get the head data
		memcpy(temp1,temp,nMinSize);

		//Get the tail data
		char *p = &temp[poItem->nSize-nMinSize];
		memcpy(temp2,p,nMinSize);

		//Encrypt head
		CStr oCryptString;
		oCryptString.NewFromString((const char *)temp1,nMinSize,nMinSize);
		poCrypt->TransformString(key,&oCryptString,nMinSize);
		const char *ptr = oCryptString.GetString();

		//Encrypte tail
		CStr oCryptString1;
		oCryptString1.NewFromString((const char *)temp2,nMinSize,nMinSize);
		poCrypt->TransformString(key,&oCryptString1,nMinSize);
		const char *ptr1 = oCryptString1.GetString();

		//Copy head and tail
		memcpy(temp,ptr,nMinSize);
		memcpy(p,ptr1,nMinSize);

#endif

		fseek(fp,poItem->nOffset,SEEK_SET);
		fwrite((char *)temp,poItem->nSize,1,fp);

		delete [] temp;
		delete [] temp1;
		delete [] temp2;
	
		if(vFileAdded != 0)
		{
			(*vFileAdded)((void *)poItem,0);
		}
	}

	fclose(fp);

return 1;
}

//////////////////////////////////////////////////////
//
//	nWriteDataHeader
//
//////////////////////////////////////////////////////
int CDLPFile::nWriteDataHeader(FILE *fp,int nMinSize)
{
	long nHeaderSize = 0;

	long nVal = fwrite((char *)&nMinSize,sizeof(int),1,fp);
	nHeaderSize += sizeof(int);

	int nSize = poFileArray->nGetSize();
	nVal = fwrite((char *)&nSize,sizeof(int),1,fp);
	nHeaderSize += sizeof(int);
	
	CStr oStr;
	
	for(int i=0; i<poFileArray->nGetSize(); i++)
	{
		CDLPFileItem *poItem = (CDLPFileItem *)poFileArray->pGetAt(i);
				
		//Offset
		nVal = sizeof(long);
		nHeaderSize += nVal;

		//Item Size
		nVal = sizeof(long);
		nHeaderSize += nVal;

		//Str Len
		nVal = sizeof(int);
		nHeaderSize += nVal;

		int nLen = poItem->poFileName->GetLength();
		//Str
		nVal = nLen;
		nHeaderSize += nVal;
	}

	long nCurOffSet = (nHeaderSize);
	int i;
	for(i=0; i<poFileArray->nGetSize(); i++)
	{
		CDLPFileItem *poItem = (CDLPFileItem *)poFileArray->pGetAt(i);
		poItem->nOffset = nCurOffSet;

		if(poDebug != 0)
		{
			poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO
				,"OffSet [%d] = [%d] - [%d] - %s \n"
				,i
				,poItem->nSize
				,poItem->nOffset
				,poItem->poFileName->GetString());
		}

		nCurOffSet += poItem->nSize;


	}	

	for(i=0; i<poFileArray->nGetSize(); i++)
	{
		CDLPFileItem *poItem = (CDLPFileItem *)poFileArray->pGetAt(i);
				
		int nLen = poItem->poFileName->GetLength();
		
		//Offset
		nVal = fwrite((char *)&poItem->nOffset,sizeof(long),1,fp);
	
		//Item Size
		nVal = fwrite((char *)&poItem->nSize,sizeof(long),1,fp);

		//Str Len
		nVal = fwrite((char *)&nLen,sizeof(int),1,fp);
	
		//Str
		nVal = fwrite((char *)poItem->poFileName->GetString(),nLen,1,fp);
	
	}

return nHeaderSize;
}


//////////////////////////////////////////////////////
//
//	poFindFile
//
//////////////////////////////////////////////////////
CDLPFileItem *CDLPFile::poFindFile(char *srcName)
{
	FILE *fp = 0;
	fp = fopen(srcName,"rb");
	
	if(fp == 0)
	{
		if(poDebug != 0)
		{
			poDebug->vPrintf(DEBUG_LEVEL5,DEBUG_ERROR
			,"Could not open file %s\n"
			,srcName);
		}

		return 0;
	}	

	CDLPFileItem *poItem = new CDLPFileItem(this,poDebug);

	CStr oStr;
	long nSize = 0;
	nSize = oStr.nGetFileSize(srcName);
		

	char *pData = new char[nSize];
	memset(pData,0,nSize);

	fread((char *)pData,nSize,1,fp);

	poItem->nOffset = 0;
	poItem->nSize	= nSize;
	poItem->poFileName->Format("%s",srcName);
	poItem->data = pData;

	fclose(fp);

return poItem;
}

//////////////////////////////////////////////////////
//
//	poFindFile
//
//////////////////////////////////////////////////////
CDLPFileItem *CDLPFile::poFindFile(char *srcName,char *fileName,char *key)
{

	FILE *fp = 0;
	CDLPFileItem *poItem = 0;
	fp = fopen(srcName,"rb");
	
	if(fp == 0)
	{
		if(poDebug != 0)
		{
			poDebug->vPrintf(DEBUG_LEVEL5,DEBUG_INFO
			,"Could not open file will look in CBN%s\n"
			,srcName);
		}

		//Only if I find it
		//Ok find non cbn file and read it
		poItem = poFindFile(fileName);
		if(poItem != 0)
		{
			poFileArray->nAdd(poItem);
			return poItem;
		}

	}

	int nMinSize = 0;
	fread((char *)&nMinSize,sizeof(int),1,fp);

	CStr oStr;
	oStr.Format("%s",fileName);
	int nVal = 0;
	fread((char *)&nVal,sizeof(int),1,fp);

	long nOffset = 0;
	long nSize = 0;
	int nLen = 0;
	int nFound = 0;
	for(int i=0; i<nVal; i++)
	{
		fread((char *)&nOffset,sizeof(long),1,fp);
		fread((char *)&nSize,sizeof(long),1,fp);
		fread((char *)&nLen,sizeof(int),1,fp);
		char *name = new char[nLen+1];
		memset(name,0,nLen+1);
		fread((char *)name,nLen,1,fp);
	
		if(poDebug != 0)
		{
			poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO
				,"Looking for [%d] = [%s] - [%s]\n"
				,i
				,oStr.GetString()
				,name);
		}

		if(oStr.CompareNoCase(name) == 0)
		{
			nFound = 1;
			break;
		}

	}
	
	if(nFound == 1)
	{
		char *data = 0;
		char *data1 = 0;
		char *data2 = 0;

		data = new char [nSize];
		data1 = new char[nMinSize];
		data2 = new char[nMinSize];

		fseek(fp,nOffset,SEEK_SET);
		
		long nVal = 0;
		nVal = fread(data,nSize,1,fp);

//Encrypt
#if 1
		//Get the head data to decrypt
		memcpy(data1,data,nMinSize);

		//Get the tail data to decrypt
		char *p = &data[nSize-nMinSize];
		memcpy(data2,p,nMinSize);

		//Decrypt head
		CStr oCryptString;
		oCryptString.NewFromString((const char *)data1,nMinSize,nMinSize);
		poCrypt->TransformString(key,&oCryptString,nMinSize);

		//decrypt tail
		CStr oCryptString1;
		oCryptString1.NewFromString((const char *)data2,nMinSize,nMinSize);
		poCrypt->TransformString(key,&oCryptString1,nMinSize);

		//Get the decrypted data ptrs
		const char *ptr = oCryptString.GetString();
		const char *ptr1 = oCryptString1.GetString();

		//Put the decrypted data back
		memcpy(data,ptr,nMinSize);
		memcpy(p,ptr1,nMinSize);
#endif

		poItem = new CDLPFileItem(this,poDebug);
		poItem->nOffset = nOffset;
		poItem->nSize = nSize;
		poItem->poFileName->Format("%s",fileName);
		poItem->data = data;

		delete [] data1;
		delete [] data2;
	}
	else
	{
		poItem = poFindFile(fileName);
	}

	fclose(fp);

poFileArray->nAdd(poItem);
return poItem;
}

//////////////////////////////////////////////////////
//
//	poFindFile
//
//////////////////////////////////////////////////////
int CDLPFile::nGetMaxNumberofItems(unsigned char *pData)
{
	long nTopOffSet = 0;
	
	int nMinSize = 0;
	memcpy(&nMinSize,&pData[nTopOffSet],sizeof(int));
	nTopOffSet += sizeof(int);

	int nNum = 0;
	memcpy(&nNum,&pData[nTopOffSet],sizeof(int));
	nTopOffSet += sizeof(int);

return nNum;
}

//////////////////////////////////////////////////////
//
//	poFindFile
//
//////////////////////////////////////////////////////
CDLPFileItem *CDLPFile::poFindFile(unsigned char *pData,char *fileName,char *key)
{
	CDLPFileItem *poItem = 0;


	CStr oStr2;
	oStr2.Format(".\\data\\%s",fileName);
	poItem = poFindFile(oStr2.GetString());

	if(poItem!= 0)
	{
		poFileArray->nAdd(poItem);
		return poItem;
	}

	if(poItem == 0)
	{
		oStr2.Format(".\\data\\content\\%s",fileName);
		poItem = poFindFile(oStr2.GetString());
	}

	if(poItem!= 0)
	{
		poFileArray->nAdd(poItem);
		return poItem;
	}

	if(poItem == 0)
	{
		oStr2.Format(".\\data\\UI\\%s",fileName);
		poItem = poFindFile(oStr2.GetString());
	}

	if(poItem!= 0)
	{
		poFileArray->nAdd(poItem);
		return poItem;
	}

	if(poItem == 0)
	{
		oStr2.Format(".\\data\\Models\\%s",fileName);
		poItem = poFindFile(oStr2.GetString());
	}

	if(poItem!= 0)
	{
		poFileArray->nAdd(poItem);
		return poItem;
	}

	if(poItem == 0)
	{
		oStr2.Format(".\\data\\Animations\\%s",fileName);
		poItem = poFindFile(oStr2.GetString());
	}

	if(poItem!= 0)
	{
		poFileArray->nAdd(poItem);
		return poItem;
	}

	CStr oStr;
	oStr.Format("%s",fileName);
	int nVal = 0;
	
	long nTopOffSet = 0;
	
	int nMinSize = 0;
	memcpy(&nMinSize,&pData[nTopOffSet],sizeof(int));
	nTopOffSet += sizeof(int);


	memcpy(&nVal,&pData[nTopOffSet],sizeof(int));
	nTopOffSet += sizeof(int);


	long nOffset = 0;
	long nSize = 0;
	int nLen = 0;
	int nFound = 0;
	for(int i=0; i<nVal; i++)
	{

		memcpy(&nOffset,&pData[nTopOffSet],sizeof(long));
		nTopOffSet += sizeof(long);

		memcpy(&nSize,&pData[nTopOffSet],sizeof(long));
		nTopOffSet += sizeof(long);

		memcpy(&nLen,&pData[nTopOffSet],sizeof(int));
		nTopOffSet += sizeof(int);

		char *name = new char[nLen+1];
		memset(name,0,nLen+1);
		memcpy(name,&pData[nTopOffSet],nLen);
		nTopOffSet += nLen;
		
		if(poDebug != 0)
		{
			poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO
				,"Looking for [%d] = [%s] - [%s]\n"
				,i
				,oStr.GetString()
				,name);
		}

		if(oStr.CompareNoCase(name) == 0)
		{
			nFound = 1;
			break;
		}

	}

	
	if(nFound == 1)
	{
		char *data = 0;
		char *data1 = 0;
		char *data2 = 0;

		data = new char [nSize];
		data1 = new char[nMinSize];
		data2 = new char[nMinSize];

		//Reset
		nTopOffSet = nOffset;
		
		memcpy(data,&pData[nTopOffSet],nSize);

//Encrypt
#if 1
		//Get the head data to decrypt
		memcpy(data1,&pData[nTopOffSet],nMinSize);
		nTopOffSet += nMinSize;

		//Get the tail data to decrypt
		char *p = &data[nSize-nMinSize];
		memcpy(data2,p,nMinSize);


		//Decrypt head
		CStr oCryptString;
		oCryptString.NewFromString((const char *)data1,nMinSize,nMinSize);
		poCrypt->TransformString(key,&oCryptString,nMinSize);

		//decrypt tail
		CStr oCryptString1;
		oCryptString1.NewFromString((const char *)data2,nMinSize,nMinSize);
		poCrypt->TransformString(key,&oCryptString1,nMinSize);

		//Get the decrypted data ptrs
		const char *ptr = oCryptString.GetString();
		const char *ptr1 = oCryptString1.GetString();

		//Put the decrypted data back
		memcpy(data,ptr,nMinSize);
		memcpy(p,ptr1,nMinSize);
#endif

		poItem = new CDLPFileItem(this,poDebug);
		poItem->nOffset = nOffset;
		poItem->nSize = nSize;
		poItem->poFileName->Format("%s",fileName);
		poItem->data = data;

		delete [] data1;
		delete [] data2;
	}

poFileArray->nAdd(poItem);
return poItem;
}



//////////////////////////////////////////////////////////////////////////////
// Forward declares.
bool WildMatch( const char* pattern, const char *string, bool caseSensitive );


/**
	Constructor.
**/
FileGlobBase::FileGlobBase()
{
	AddIgnorePattern( "./" );
	AddIgnorePattern( "../" );
}							


/**
	Finds all files matching [inPattern].  The wildcard expansion understands
	the following pattern constructs:

	- ?
		- Any single character.
	- *
		- Any character of which there are zero or more of them.
	- **
		- All subdirectories recursively.

	Additionally, if the pattern closes in a single slash, only directories
	are processed.  Forward slashes and backslashes may be used
	interchangeably.

	- *\
		- List all the directories in the current directory.  Can also be
		  *(forwardslash), but it turns off the C++ comment in this message.
	- **\
		- List all directories recursively.

	Wildcards may appear anywhere in the pattern, including directories.

	- *\*\*\*.c

	Note that *.* only matches files that have an extension.  This is different
	than standard DOS behavior.  Use * all by itself to match files, extension
	or not.

	Recursive wildcards can be used anywhere:

	c:/Dir1/**\A*\**\FileDirs*\**.mp3

	This matches all directories under c:\Dir1\ that start with A.  Under all
	of the directories that start with A, directories starting with FileDirs
	are matched recursively.  Finally, all files ending with an mp3 extension
	are matched.

	Any place that has more than two .. for going up a subdirectory is expanded
	a la 4DOS.

	...../StartSearchHere/**

	Expands to:

	../../../../StartSearchHere/**
		
	\param inPattern The pattern to use for matching.
**/
void FileGlobBase::MatchPattern( const char* inPattern )
{
	char pattern[ MAX_PATH ];

	// Give ourselves a local copy of the inPattern with all \ characters
	// changed to / characters and more than two periods expanded.
	const char* srcPtr = inPattern;

	// Is it a Windows network path?  If so, don't convert the opening \\.
	if ( srcPtr[ 0 ] == '\\'  &&  srcPtr[ 1 ] == '\\' )
		srcPtr += 2;

	const char* lastSlashPtr = srcPtr - 1;
	char* destPtr = pattern;
	int numPeriods = 0;
	while ( *srcPtr != '\0' )
	{
		char ch = *srcPtr;
		
		///////////////////////////////////////////////////////////////////////
		// Check for slashes or backslashes.
		if ( ch == '\\'  ||  ch == '/' )
		{
			*destPtr++ = '/';

			lastSlashPtr = srcPtr;
			numPeriods = 0;
		}

		///////////////////////////////////////////////////////////////////////
		// Check for .
		else if ( ch == '.' )
		{
			if ( srcPtr - numPeriods - 1 == lastSlashPtr )
			{
				numPeriods++;
				if ( numPeriods > 2 )
				{
					*destPtr++ = '/';
					*destPtr++ = '.';
					*destPtr++ = '.';
				}
				else
				{
					*destPtr++ = '.';
				}
			}
			else
			{
				*destPtr++ = '.';
			}
		}

		///////////////////////////////////////////////////////////////////////
		// Check for **
		else if ( ch == '*'  &&  srcPtr[ 1 ] == '*' )
		{
			if ( srcPtr - 1 != lastSlashPtr )
			{
				// Something like this:
				//
				// /Dir**/
				//
				// needs to be translated to:
				//
				// /Dir*/**/
				*destPtr++ = '*';
				*destPtr++ = '/';
			}

			srcPtr += 2;

			*destPtr++ = '*';
			*destPtr++ = '*';

			// Did we get a double star this round?
			if ( srcPtr[ 0 ] != '/'  &&  srcPtr[ 0 ] != '\\' )
			{
				// Handle the case that looks like this:
				//
				// /**Text
				//
				// Translate to:
				//
				// /**/*Text
				*destPtr++ = '/';
				*destPtr++ = '*';
			}
			else if ( srcPtr[ 1 ] == '\0'  ||  srcPtr[ 1 ] == '@' )
			{
				srcPtr++;

				*destPtr++ = '/';
				*destPtr++ = '*';
				*destPtr++ = '/';
			}

			// We added one too many in here... the compiler will optimize.
			srcPtr--;
		}

		///////////////////////////////////////////////////////////////////////
		// Check for @
		else if ( ch == '@' )
		{
			// Gonna finish this processing in another loop.
			break;
		}
			
		///////////////////////////////////////////////////////////////////////
		// Everything else.
		else
		{
			*destPtr++ = *srcPtr;
		}

		srcPtr++;
	}

	*destPtr = 0;

	// Check for the @.
	if ( *srcPtr == '@' )
	{
		// Clear out function registered entries.
		m_ignorePatterns.clear();
		AddIgnorePattern( "./" );
		AddIgnorePattern( "../" );

		m_exclusiveFilePatterns.clear();
	}

	while ( *srcPtr == '@' )
	{
		srcPtr++;

		char ch = *srcPtr++;
		if ( ch == '-'  ||  ch == '=' )
		{
			char buffer[ _MAX_PATH ];
			char* ptr = buffer;
			while ( *srcPtr != '@'  &&  *srcPtr != '\0' )
			{
				*ptr++ = *srcPtr++;
			}

			*ptr = 0;

			if ( ch == '-' )
				AddIgnorePattern( buffer );
			else if ( ch == '=' )
				AddExclusivePattern( buffer );
		}
		else
			break;		// Don't know what it is.
	}

	// Start globbing!
	GlobHelper( pattern );
}


/**
	Adds a pattern to the file glob database of exclusive patterns.  If any
	exclusive patterns are registered, the ignore database is completely
	ignored.  Only patterns matching the exclusive patterns will be
	candidates for matching.

	\param name The exclusive pattern.
**/
void FileGlobBase::AddExclusivePattern( const char* pattern )
{
	for ( StringList::iterator it = m_exclusiveFilePatterns.begin();
			it != m_exclusiveFilePatterns.end(); ++it )
		if ( stricmp( (*it).c_str(), pattern ) == 0 )
			return;

	m_exclusiveFilePatterns.push_back( pattern );
}


/**
	Adds a pattern to ignore to the file glob database.  If a pattern of
	the given name is found, its contents will not be considered for further
	matching.  The result is as if the pattern did not exist for the search
	in the first place.

	\param name The pattern to ignore.
**/
void FileGlobBase::AddIgnorePattern( const char* pattern )
{
	for ( StringList::iterator it = m_ignorePatterns.begin();
			it != m_ignorePatterns.end(); ++it )
		if ( stricmp( (*it).c_str(), pattern ) == 0 )
			return;

	m_ignorePatterns.push_back( pattern );
}


/**
	Match an exclusive pattern.

	\param text The text to match an ignore pattern against.
	\return Returns true if the directory should be ignored, false otherwise.
**/
bool FileGlobBase::MatchExclusivePattern( const char* text )
{
	for ( StringList::iterator it = m_exclusiveFilePatterns.begin();
			it != m_exclusiveFilePatterns.end(); ++it )
	{
		if ( WildMatch( (*it).c_str(), text, false  ) )
			return true;
	}

	return false;
}


/**
	Do a case insensitive find for the pattern.

	\param text The text to match an ignore pattern against.
	\return Returns true if the directory should be ignored, false otherwise.
**/
bool FileGlobBase::MatchIgnorePattern( const char* text )
{
	for ( StringList::iterator it = m_ignorePatterns.begin();
			it != m_ignorePatterns.end(); ++it )
	{
		if ( WildMatch( (*it).c_str(), text, false  ) )
			return true;
	}

	return false;
}


/**
	\internal
	\author Jack Handy

	Borrowed from http://www.codeproject.com/string/wildcmp.asp.
	Modified by Joshua Jensen.
**/
bool WildMatch( const char* pattern, const char *string, bool caseSensitive )
{
	// Handle all the letters of the pattern and the string.
	while ( *string != 0  &&  *pattern != '*' )
	{
		if ( *pattern != '?' )
		{
			if ( caseSensitive )
			{
				if ( *pattern != *string )
					return false;
			}
			else
			{
				if ( toupper( *pattern ) != toupper( *string ) )
					return false;
			}
		}

		pattern++;
		string++;
	}

	const char* mp = NULL;
	const char* cp = NULL;
	while ( *string != 0 )
	{
		if (*pattern == '*')
		{
			// It's a match if the wildcard is at the end.
			if ( *++pattern == 0 )
			{
				return true;
			}

			mp = pattern;
			cp = string + 1;
		}
		else
		{
			if ( caseSensitive )
			{
				if ( *pattern == *string  ||  *pattern == '?' )
				{
					pattern++;
					string++;
				}
				else 
				{
					pattern = mp;
					string = cp++;
				}
			}
			else
			{
				if ( toupper( *pattern ) == toupper( *string )  ||  *pattern == '?' )
				{
					pattern++;
					string++;
				}
				else
				{
					pattern = mp;
					string = cp++;
				}
			}
		}
	}

	// Collapse remaining wildcards.
	while ( *pattern == '*' )
		pattern++;

	return !*pattern;
}


/**
	\internal Simple path splicing (assumes no '/' in either part)
	\author Matthias Wandel (MWandel@rim.net) http://http://www.sentex.net/~mwandel/
**/
static void CatPath(char * dest, const char * p1, const char * p2)
{
	size_t len = strlen( p1 );
	if ( len == 0 )
	{
		strcpy( dest, p2 );
	}
	else
	{
		if ( len + strlen( p2 ) > 200 )
		{
			// Path too long.
			return;
		}
		memcpy( dest, p1, len + 1 );
		if ( dest[ len - 1 ] != '/'  &&  dest[ len - 1 ] != ':' )
		{
			dest[ len++ ] = '/';
		}
		strcpy( dest + len, p2 );
	}
}


/**
	I have made the following extensions:

	-	Support for ignoring directories.
	-	Perforce-style (and DJGPP-style) ... for recursion, instead of **.
	-	Automatic conversion from ...Stuff to .../*Stuff.  Allows lookup of
		files by extension, too: '....h' translates to '.../*.h'.
	-	Ability to handle forward slashes and backslashes.
	-	A minimal C++ class design.
	-	Wildcard matching not based on FindFirstFile().  Should allow greater
		control in the future and patching in of the POSIX fnmatch() function
		on systems that support it.
**/
void FileGlobBase::GlobHelper( const char* inPattern )
{
	char patternBuf[ _MAX_PATH * 2 ];
	strcpy( patternBuf, inPattern );

DoRecursion:
	char basePath[ _MAX_PATH ];
	char* basePathEndPtr = basePath;
	char* recurseAtPtr = NULL;
	char* pattern = patternBuf;  // C++11 scope fix: declare before for-loop

	// Split the path into base path and pattern to match against.
	bool hasWildcard = false;

	for ( pattern = patternBuf; *pattern != '\0'; ++pattern )
	{
		char ch = *pattern;

		// Is it a '?' ?
		if ( ch == '?' )
			hasWildcard = true;

		// Is it a '*' ?
		else if ( ch == '*' )
		{
			hasWildcard = true;

			// Is there a '**'?
			if ( pattern[ 1 ] == '*' )
			{
				// If we're just starting the pattern or the characters immediately
				// preceding the pattern are a drive letter ':' or a directory path
				// '/', then set up the internals for later recursion.
				if ( pattern == patternBuf  ||  pattern[ -1 ] == '/'  ||
					pattern[ -1 ] == ':')
				{
					char ch2 = pattern[ 2 ];
					if ( ch2 == '/' )
					{
						recurseAtPtr = pattern;
						memcpy(pattern, pattern + 3, strlen( pattern ) - 2 );
					}
					else if ( ch2 == '\0' )
					{
						recurseAtPtr = pattern;
						*pattern = '\0';
					}
				}
			}
		}

		// Is there a '/' or ':' in the pattern at this location?
		if ( ch == '/'  ||  ch == ':' )
		{
			if ( hasWildcard )
				break;
			basePathEndPtr = &basePath[ pattern - patternBuf + 1 ];
		}
	}

	// If there is no wildcard this time, then just add the current file and
	// get out of here.
	if ( !hasWildcard )
	{
		// This should refer to a file.
		FoundMatch( patternBuf );
		return;
	}

	// Did we make it to the end of the pattern?  If so, we should match files,
	// since there were no slashes encountered.
	bool matchFiles = *pattern == '\0';

	// Copy the directory down.
	size_t basePathLen = basePathEndPtr - basePath;
	strncpy( basePath, patternBuf, basePathLen );

	// Copy the wildcard matching string.
	char matchPattern[ _MAX_PATH ];
	size_t matchLen = ( pattern - patternBuf ) - basePathLen;
	strncpy( matchPattern, patternBuf + basePathLen, matchLen + 1 );
	if ( matchPattern[ matchLen ] == '/' )
		matchPattern[ matchLen ] = 0;

	StringList fileList;

	// Do the file search with *.* in the directory specified in basePattern.
	strcpy( basePathEndPtr, "*.*" );

	// Start the find.
	WIN32_FIND_DATA fd;
	HANDLE handle = FindFirstFile( basePath, &fd );

	// Clear out the *.* so we can use the original basePattern string.
	*basePathEndPtr = 0;

	// Any files found?
	if ( handle != INVALID_HANDLE_VALUE )
	{
		for ( ;; )
		{
			// Is the file a directory?
			if ( ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )  &&  !matchFiles )
			{
				// Do a wildcard match.
				if ( WildMatch( matchPattern, fd.cFileName, false ) )
				{
					// It matched.  Let's see if the file should be ignored.
					bool ignore = false;

					// Knock out "." or ".." if they haven't already been.
					int len = strlen( fd.cFileName );
					fd.cFileName[ len ] = '/';
					fd.cFileName[ len + 1 ] = '\0';

					// See if this is a directory to ignore.
					ignore = MatchIgnorePattern( fd.cFileName );

					fd.cFileName[ len ] = 0;

					// Should this file be ignored?
					if ( !ignore )
					{
						// Nope.  Add it to the linked list.
						fileList.push_back( fd.cFileName );
					}
				}
			}
			else if ( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )  &&  matchFiles )
			{
				// Do a wildcard match.
				if ( WildMatch( matchPattern, fd.cFileName, false ) )
				{
					// It matched.  Let's see if the file should be ignored.
					bool ignore = MatchIgnorePattern( fd.cFileName );

					// Is this pattern exclusive?
					if ( !ignore  &&  m_exclusiveFilePatterns.begin() != m_exclusiveFilePatterns.end() )
					{
						ignore = !MatchExclusivePattern( fd.cFileName );
					}

					// Should this file be ignored?
					if ( !ignore )
					{
						// Nope.  Add it to the linked list.
						fileList.push_back( fd.cFileName );
					}
				}
			}

			// Look up the next file.
			if ( !FindNextFile( handle, &fd ) )
				break;
		}

		// Close down the file find handle.
		FindClose( handle );
	}

	// Sort the list.
	fileList.sort();

	// Iterate the file list and either recurse or add the file as a found
	// file.
	if ( !matchFiles )
	{
		for ( StringList::iterator it = fileList.begin(); it != fileList.end(); ++it )
		{
			char combinedName[ _MAX_PATH * 2 ];

			// Need more directories.
			CatPath( combinedName, basePath, (*it).c_str() );
			strcat( combinedName, pattern );
			GlobHelper( combinedName );
		}
	}
	else // if ( !matchFiles )
	{
		for ( StringList::iterator it = fileList.begin(); it != fileList.end(); ++it )
		{
			char combinedName[ _MAX_PATH * 2 ];
			CatPath( combinedName, basePath, (*it).c_str());
			FoundMatch( combinedName );
		}
	}

	// Clear out the file list, so the goto statement below can recurse
	// internally.
	fileList.clear();

	// Do we need to recurse?
	if ( !recurseAtPtr )
		return;

	// Copy in the new recursive pattern to match.
	strcpy( matchPattern, recurseAtPtr );
	strcpy( recurseAtPtr, "*/**/" );
	strcat( patternBuf, matchPattern );

	// As this function context is no longer needed, we can just go back
	// to the top of it to avoid adding another context on the stack.
	goto DoRecursion;
}



