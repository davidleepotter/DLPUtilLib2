/*********************************************************************
*	File : NTokLib.cpp 
*	Desc : Token Config file Clas
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 
#if !defined(AFX_NTOK1_H__C4725903_AFA5_4724_840E_B05736E246EA__INCLUDED_)
#define AFX_NTOK1_H__C4725903_AFA5_4724_840E_B05736E246EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
class CStr;
class DLP_CPtrList;
class CDebugInfo;
class DLP_CPtrArray;

class CDLPVector3
{
private:

public:
	float x;
	float y;
	float z;

	CDLPVector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	~CDLPVector3()
	{

	}

};

enum MAX_VARS
{
	MAX_VAR = 100000,

};

enum TOK_TYPES 
{
	TOK_WORD,
	TOK_INT,
	TOK_FLOAT,
	TOK_STRING,
	TOK_IP,
	TOK_FILE,
	TOK_STRING_ENCRYPT,
	TOK_STRING_ENCRYPTED,
	TOK_VECTOR3,
	TOK_GROUP,
	TOK_GROUP_ITEM,
	TOK_TOTAL,
};

enum GROUP_TYPES 
{
	A_TYPE__WORD,
	A_TYPE__INT,
	A_TYPE__FLOAT,
	A_TYPE__STRING,
	A_TYPE__IP,
	A_TYPE__FILE,
	A_TYPE__STRING_ENCRYPT,
	A_TYPE__STRING_ENCRYPTED,
	A_TYPE__VECTOR3,
	A_TYPE__TOTAL,
};

class CNTok;


class CFileItem
{

private:

public:	
	CStr *poFileName;
	CStr *poPathSrc;
	CStr *poPathDest;
	CStr *poDate;
	long nSize;	

	CFileItem(char *fileName, char *pathSrc,char *pathDest ,long size, char *date);
	~CFileItem();
};


class CNTokItem
{

private:
	int nUid;
	CDebugInfo *poDebug;

public:
	friend CNTok;

	CNTokItem(char nVal, char *tname,CDebugInfo *potDebug);
	CNTokItem(int nVal, char *tname,CDebugInfo *potDebug);
	CNTokItem(float nVal, char *tname,CDebugInfo *potDebug);
	CNTokItem(char *nVal, char *tname,CDebugInfo *potDebug);
	CNTokItem(char *nVal, char *tname,char *key,CDebugInfo *potDebug);
	CNTokItem(int ip1,int ip2, int ip3, int ip4, char *tname,CDebugInfo *potDebug);
	CNTokItem(char *filename,char *srcPath,char *destPath,long size,char *date, char *tname,CDebugInfo *potDebug);
	CNTokItem(float fx, float fy, float fz, char *tname,CDebugInfo *potDebug);
	
	//ARRAY
	CNTokItem(char *tnam,int nType,CDebugInfo *potDebug);
	CNTokItem * poGetGroupItem(int nIndex);
	CNTokItem *poAddGroupItem(void *data,char *name);
	void vRemoveGroupItem(char *name);
	CNTokItem * poGetGroupItem(char *name);
	void vSetDataString(void);
	void vSetDataString(char *data0,char *data1, char *data2);

	virtual ~CNTokItem();

	int				nSubType;
	int				nNumVars;
	int				type;
	int				ip[4];
	char			varWord;
	int				varInt;
	float			varFloat;
	CStr			*poGroupName;
	CStr			*varStr;
	CStr			*name;
	CStr			*poDataString[3];
	CDLPVector3		*poVector3;
	DLP_CPtrList	*poCommentList;
	CFileItem		*poFItem;
	DLP_CPtrArray	*poGroup;
	int nGroupType;

};

class CNTok  
{
private:
	FILE			*fp;
	CStr			*sFileName;
	int nType;
	CDebugInfo		*poDebug;
	CStr			*poGlobalKey;
	DLP_CPtrArray	*m_poMasterGroup;

	void vWriteComments(CNTokItem *tItem);

public:
	void vParseTok(void);
	void vUnLoadTokFile(void);
	void vRemoveItem(CNTokItem *tItem);
	void vAddItem(CNTokItem *tItem);
	void vAddItem(CNTokItem *tItem,CNTokItem *tVarGroup);
	int nLoadTokFile(void);
	int	nWriteTokFile(char *filename);
	int nFindAndRemoveByName(char *val);
	int nGetNumItems(void);
	void vAddItem(char nVal, char *tname);
	void vAddItem(int nVal, char *tname);
	void vAddItem(float nVal, char *tname);
	void vAddItem(char *nVal, char *tname);
	void vAddItem(int ip1,int ip2, int ip3, int ip4, char *tname);
	void vAddItem(float fx, float fy, float fz, char *tname);
	void vAddItem(char *tname,int nType);
	int nGetNumGroups(void);
	CNTokItem *poGetGroup(int nVal);
	
	void vWriteItem(CNTokItem *tItem,FILE *fp);

	static long nGUID;

	CNTokItem * poFindVarName(char *val);

	int nNumOfType(int nType);

	static const char * pItemTypes[];
	static const char * pGroupTypes[];
	int nFindGroupType(const char *type);

	DLP_CPtrList *varItems;
	CNTok(CDebugInfo *poDebug);
	CNTok(char * sFileName,CDebugInfo *potDebug);
	virtual ~CNTok();
};

#endif // !defined(AFX_NTOK1_H__C4725903_AFA5_4724_840E_B05736E246EA__INCLUDED_)
