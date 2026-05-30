/*********************************************************************
*	File : NTokLib.cpp 
*	Desc : Token Config file Class
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 

#include "ntoklib.h"
#include "cstr.h"
#include <stdio.h>
#include "NewCol.h"
#include "utildefines.h"
#include "DebugInfo.h"
#include "aracrypt.h"

long CNTok::nGUID = 0;

CFileItem::CFileItem(char *fileName, char *pathSrc,char *pathDest, long size, char *date)
{

	poFileName	= new CStr(fileName);
	poPathSrc	= new CStr(pathSrc);
	poPathDest	= new CStr(pathDest);
	poDate		= new CStr(date);
	nSize		= size;

}

CFileItem::~CFileItem()
{
	delete poFileName;
	delete poPathDest;
	delete poDate;
	delete poPathSrc;
}


//////////////////////////////////////////////////////
//
//	Constructor type word
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(char nVal, char *tname,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	type			= TOK_WORD;	
	varWord			= nVal;
	varInt			= -1;
	varFloat		= -1.0f;
	varStr			= 0;
	name			= new CStr(tname);
	poGroupName		= 0;
	poCommentList	= new DLP_CPtrList(poDebug);
	poFItem			= 0;
	poVector3		= 0;
	poGroup			= 0;
	nGroupType		= -1;
	nSubType		= -1;

	nNumVars		= 1;
	poDataString[0]	= new CStr;
	poDataString[1]	= 0;
	poDataString[2]	= 0;

	vSetDataString();
	
	nUid = CNTok::nGUID;
	CNTok::nGUID++;
}


//////////////////////////////////////////////////////
//
//	Constructor type Group
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(char *tname,int nType,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	varInt			= -1;	
	varWord			= -1;
	varFloat		= -1.0f;
	varStr			= 0;
	name			= new CStr(tname);
	poGroupName		= 0;
	poCommentList	= new DLP_CPtrList(potDebug);
	poFItem			= 0;
	poVector3		= 0;
	ip[0]			= 0;
	ip[1]			= 0;
	ip[2]			= 0;
	ip[3]			= 0;
	nSubType		= -1;

	nNumVars		= 0;
	poDataString[0]	= 0;
	poDataString[1]	= 0;
	poDataString[2]	= 0;

	type = TOK_GROUP;
	poGroup = new DLP_CPtrArray;
	nGroupType = nType;

	nUid = CNTok::nGUID;
	CNTok::nGUID++;
}
//////////////////////////////////////////////////////
//
//	poGetGroupItem
//
//////////////////////////////////////////////////////
CNTokItem * CNTokItem::poGetGroupItem(int nIndex)
{
	if(poGroup != 0)
	{
		CNTokItem *poTokItem = 0;
		poTokItem = (CNTokItem *)poGroup->pGetAt(nIndex);
		return poTokItem;
	}

return 0;
}

//////////////////////////////////////////////////////
//
//	poGetGroupItem
//
//////////////////////////////////////////////////////
CNTokItem * CNTokItem::poGetGroupItem(char *name)
{
	if(poGroup != 0)
	{
		for(int i=0; i<poGroup->nGetSize(); i++)
		{
			CNTokItem *poTokItem = 0;
			poTokItem = (CNTokItem *)poGroup->pGetAt(i);

			if(poTokItem->name->CompareNoCase(name) == 0)
			{
				return poTokItem;	
			}
		}
	}

return 0;
}

//////////////////////////////////////////////////////
//
//	vRemoveGroupItem
//
//////////////////////////////////////////////////////
void CNTokItem::vRemoveGroupItem(char *name)
{
	if(poGroup != 0)
	{
		for(int i=0; i<poGroup->nGetSize(); i++)
		{
			CNTokItem *poTokItem = 0;
			poTokItem = (CNTokItem *)poGroup->pGetAt(i);

			if(poTokItem->name->CompareNoCase(name) == 0)
			{
				poGroup->vRemoveAt(i);
				delete poTokItem;
				return;	
			}
		}
	}

return;
}

//////////////////////////////////////////////////////
//
//	poAddGroupItem
//
//////////////////////////////////////////////////////
CNTokItem * CNTokItem::poAddGroupItem(void *data,char *tname)
{
	if(poGroup != 0)
	{
		CNTokItem *poTokItem = 0;
		
		switch(nGroupType)
		{
			case TOK_WORD:
				{
					char cWord = 0;
					memcpy(&cWord,(char *)data,sizeof(char));
					poTokItem = new CNTokItem(cWord,tname,poDebug);
				}
				break;

			case TOK_INT:
				{
					int nInt = 0;
					memcpy(&varInt,(char *)data,sizeof(int));
					poTokItem = new CNTokItem(nInt,tname,poDebug);
				}
				break;
			
			case TOK_FLOAT:
				{
					float fFloat = 0.0f;
					memcpy(&varFloat,(char *)data,sizeof(float));
					poTokItem = new CNTokItem(fFloat,tname,poDebug);
				}
				break;

			case TOK_STRING:
				{
					poTokItem = new CNTokItem((char *)data,tname,poDebug);
				}
				break;

			case TOK_IP:
				{
					int nIP[4];
					memcpy(&nIP[0],data,4*sizeof(int));
					poTokItem = new CNTokItem(nIP[0],nIP[1],nIP[2],nIP[3],tname,poDebug);
				}
				break;

			case TOK_VECTOR3:
				{
					CDLPVector3 oVector;
					memcpy(&oVector,data,sizeof(CDLPVector3));
					poTokItem = new CNTokItem(oVector.x,oVector.y,oVector.z,tname,poDebug);
				}
				break;
		}

		if(poTokItem->poGroupName == 0)
		{
			poTokItem->poGroupName = new CStr;
			poTokItem->poGroupName->Format("%s",this->name->GetString());
		}

		poTokItem->name->Format("Item%d",poGroup->nGetSize());
		poTokItem->nSubType = 1;
		poGroup->nAdd(poTokItem);
		return poTokItem;
	}

return 0;
}

//////////////////////////////////////////////////////
//
//	Constructor type int
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(int nVal, char *tname,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	type			= TOK_INT;
	varInt			= nVal;	
	varWord			= -1;
	varFloat		= -1.0f;
	varStr			= 0;
	name			= new CStr(tname);
	poGroupName		= 0;
	poCommentList	= new DLP_CPtrList(potDebug);
	poFItem			= 0;
	poVector3		= 0;
	poGroup			= 0;
	nGroupType		= -1;
	nSubType		= -1;

	nNumVars		= 1;
	poDataString[0]	= new CStr;
	poDataString[1]	= 0;
	poDataString[2]	= 0;

	vSetDataString();

	nUid = CNTok::nGUID;
	CNTok::nGUID++;
}

//////////////////////////////////////////////////////
//
//	Constructor type float
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(float nVal, char *tname,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	type			= TOK_FLOAT;
	varFloat		= nVal;
	varInt			= -1;	
	varWord			= -1;
	varStr			= 0;
	name			= new CStr(tname);
	poGroupName		= 0;
	poCommentList	= new DLP_CPtrList(potDebug);
	poFItem			= 0;	
	poVector3		= 0;
	poGroup			= 0;
	nGroupType		= -1;
	nSubType		= -1;

	nNumVars		= 1;
	poDataString[0]	= new CStr;
	poDataString[1]	= 0;
	poDataString[2]	= 0;

	vSetDataString();

	nUid = CNTok::nGUID;
	CNTok::nGUID++;
}

//////////////////////////////////////////////////////
//
//	Constructor type string
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(char *nVal, char *tname,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	type			= TOK_STRING;
	varFloat		= -1;
	varInt			= -1;	
	varWord			= -1;
	varStr			= new CStr(nVal);
	name			= new CStr(tname);
	poGroupName		= 0;
	poCommentList	= new DLP_CPtrList(potDebug);
	poFItem			= 0;	
	poGroup			= 0;
	nGroupType		= -1;
	poVector3		= 0;
	nSubType		= -1;

	nNumVars		= 1;
	poDataString[0]	= new CStr;
	poDataString[1]	= 0;
	poDataString[2]	= 0;

	vSetDataString();

//Linux Kludge
#ifndef WIN32
			varStr->RemoveRight(1);
#endif

	nUid = CNTok::nGUID;
	CNTok::nGUID++;
}

//////////////////////////////////////////////////////
//
//	Constructor type string encrypted
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(char *nVal, char *tname,char *key,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	type			= TOK_STRING_ENCRYPTED;
	varFloat		= -1;
	varInt			= -1;	
	varWord			= -1;
	varStr			= new CStr(nVal);
	name			= new CStr(tname);
	poGroupName		= 0;
	poCommentList	= new DLP_CPtrList(potDebug);
	poFItem			= 0;
	poVector3		= 0;
	poGroup			= 0;
	nGroupType		= -1;
	nSubType		= -1;

	//Check to see if any part of the string is non alpha
	for(unsigned int x=0; x<varStr->GetLength(); x++)
	{
		if(!varStr->nIsAlphaNum(x))
		{
			ARACrypt oCrypt;
			oCrypt.TransformString(key,varStr,varStr->GetLength());
			break;
		}
	}
	
//Linux Kludge
#ifndef WIN32
			varStr->RemoveRight(1);
#endif

	nNumVars		= 1;
	poDataString[0]	= new CStr;
	poDataString[1]	= 0;
	poDataString[2]	= 0;
			
	vSetDataString();
	
	nUid = CNTok::nGUID;
	CNTok::nGUID++;

}
//////////////////////////////////////////////////////
//
//	Constructor type ip address
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(int ip1,int ip2, int ip3, int ip4, char *tname,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	type			= TOK_IP;
	ip[0]			= ip1;
	ip[1]			= ip2;
	ip[2]			= ip3;
	ip[3]			= ip4;
	name			= new CStr(tname);
	poGroupName		= 0;
	poCommentList	= new DLP_CPtrList(potDebug);
	poFItem			= 0;
	poVector3		= 0;
	poGroup			= 0;
	nGroupType		= -1;
	nSubType		= -1;

	nNumVars		= 1;
	poDataString[0]	= new CStr;
	poDataString[1]	= 0;
	poDataString[2]	= 0;

	vSetDataString();
	
	nUid = CNTok::nGUID;
	CNTok::nGUID++;
}

//////////////////////////////////////////////////////
//
//	Constructor type Vector3
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(float fx, float fy, float fz, char *tname,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	type			= TOK_VECTOR3;
	name			= new CStr(tname);
	poGroupName		= 0;
	poCommentList	= new DLP_CPtrList(potDebug);
	poFItem			= 0;
	poVector3		= new CDLPVector3;
	poVector3->x = fx;
	poVector3->y = fy;
	poVector3->z = fz;
	poGroup			= 0;
	nGroupType		= -1;
	nSubType		= -1;

	nNumVars		= 3;
	poDataString[0]	= new CStr;
	poDataString[1]	= new CStr;
	poDataString[2]	= new CStr;

	vSetDataString();
	
	nUid = CNTok::nGUID;
	CNTok::nGUID++;
}

//////////////////////////////////////////////////////
//
//	Constructor type file
//
//////////////////////////////////////////////////////
CNTokItem::CNTokItem(char *filename,char *srcPath,char *destPath,long size,char *date, char *tname,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	type			= TOK_FILE;
	varFloat		= -1;
	varInt			= -1;	
	varWord			= -1;
	varStr			= 0;
	name			= new CStr(tname);
	poGroupName		= 0;
	poFItem			= new CFileItem(filename,srcPath,destPath,size,date);
	poCommentList	= new DLP_CPtrList(potDebug);

	nUid = CNTok::nGUID;
	CNTok::nGUID++;

	poVector3		= 0;
	poGroup			= 0;
	nSubType		= -1;

	nNumVars		= 1;
	poDataString[0]	= new CStr;
	poDataString[1]	= 0;
	poDataString[2]	= 0;

	vSetDataString();
	nGroupType		= -1;
}
 
//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CNTokItem::~CNTokItem()
{
	if(poGroupName != 0)
	{
		delete 	poGroupName;
	}
	if(poGroup != 0)
	{
		for(int i = 0; i<poGroup->nGetSize(); i++)
		{
			CNTokItem *poTokItem = 0;
			poTokItem = (CNTokItem *)poGroup->pGetAt(i);
			delete poTokItem;
		}
		poGroup->vRemoveAll();
		delete poGroup;
		poGroup = 0;
	}


	if(poFItem != 0)
	{
		delete poFItem;
	}

	delete name;

	for(int i = 0; i<3; i++)
	{
		if(poDataString[i] != 0)
		{
			delete poDataString[i];
			poDataString[i] = 0;
		}
	}

	if(type == TOK_VECTOR3)
	{
		delete poVector3;
	}

	if(type == TOK_STRING)
	{
		delete varStr;
	}

	//Delete Comments
	DLP_CPtrListItem *plt = poCommentList->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CStr *pStr = (CStr *)plt->pGetPtr();
		delete pStr;
	    plt = nextPlt;
	    }

	poCommentList->vRemoveAll();
	delete poCommentList;

}

//////////////////////////////////////////////////////
//
//	vSetDataString 
//
//////////////////////////////////////////////////////
void CNTokItem::vSetDataString(void)
{
	switch(this->type)
		{
			case TOK_WORD:
				{
					poDataString[0]->Format("%d",this->varWord);
				}
				break;

			case TOK_INT:
				{
					poDataString[0]->Format("%d",this->varInt);
				}
				break;
			
			case TOK_FLOAT:
				{
					poDataString[0]->Format("%f",this->varFloat);
				}
				break;

			case TOK_STRING:
				{
					poDataString[0]->Format("%s",varStr->GetString());
				}
				break;

			case TOK_IP:
				{
					poDataString[0]->Format("%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
				}
				break;

			case TOK_VECTOR3:
				{
					poDataString[0]->Format("%f",poVector3->x);
					poDataString[1]->Format("%f",poVector3->y);
					poDataString[2]->Format("%f",poVector3->z);
				}
				break;
		}

}

//////////////////////////////////////////////////////
//
//	vSetDataString 
//
//////////////////////////////////////////////////////
void CNTokItem::vSetDataString(char *data0,char *data1, char *data2)
{
	switch(this->type)
		{
			case TOK_WORD:
				{
					varWord = atoi(data0);
					poDataString[0]->Format("%d",this->varWord);
				}
				break;

			case TOK_INT:
				{
					varInt = atoi(data0);
					poDataString[0]->Format("%d",this->varInt);
				}
				break;
			
			case TOK_FLOAT:
				{
					varFloat = (float)atof(data0);
					poDataString[0]->Format("%f",this->varFloat);
				}
				break;

			case TOK_STRING:
				{
					varStr->Format("%s",data0);
					poDataString[0]->Format("%s",varStr->GetString());
				}
				break;

			case TOK_IP:
				{
					CStr oStr;
					oStr.Format("%s",data0);
					oStr.nStringToIP(&ip[0]);
					poDataString[0]->Format("%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
				}
				break;

			case TOK_VECTOR3:
				{
					if(data0 != 0)
					{
						poVector3->x = (float)atof(data0);
						poDataString[0]->Format("%f",poVector3->x);
					}

					if(data1 != 0)
					{
						poVector3->y = (float)atof(data1);
						poDataString[1]->Format("%f",poVector3->y);
					}
					if(data2 != 0)
					{
						poVector3->z = (float)atof(data2);
						poDataString[2]->Format("%f",poVector3->z);
					}
				}
				break;
		}

}


//////////////////////////////////////////////////////
//
//	Constructor 
//
//////////////////////////////////////////////////////
CNTok::CNTok(char *sTFileName,CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	fp				= 0;
	sFileName		= 0;
	sFileName		= new CStr(sTFileName);
	varItems		= new DLP_CPtrList(potDebug);
	poGlobalKey		= new CStr("NetMan999");
	m_poMasterGroup = new DLP_CPtrArray;
	nLoadTokFile();
}

//////////////////////////////////////////////////////
//
//	Constructor 
//
//////////////////////////////////////////////////////
CNTok::CNTok(CDebugInfo *potDebug)
{
	poDebug			= potDebug;
	fp				= 0;
	sFileName		= 0;
	sFileName		= 0;
	varItems		= new DLP_CPtrList(potDebug);
	poGlobalKey		= new CStr("NetMan999");
	m_poMasterGroup = new DLP_CPtrArray;
}

//////////////////////////////////////////////////////
//
//	Destructor 
//
//////////////////////////////////////////////////////
CNTok::~CNTok()
{
	vUnLoadTokFile();
	
	m_poMasterGroup->vRemoveAll();
	delete m_poMasterGroup;
	m_poMasterGroup = 0;


	DLP_CPtrListItem *plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
		varItems->nRemoveListItem(plt);
		delete poTItem;


	    plt = nextPlt;
	    }

	varItems->vRemoveAll();
	delete varItems;
	delete poGlobalKey;

}

//////////////////////////////////////////////////////
//
//	AddItem
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(CNTokItem *tItem)
{
	DLP_CPtrListItem *plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
		if(poTItem->name->CompareNoCase(tItem->name->GetString()) == 0)
			{
				this->vRemoveItem(poTItem);
			}

	    plt = nextPlt;
	    }

	varItems->vAddTail(tItem);
	if(poDebug != 0)
	{
		poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO,"Added Item [%s]\n",tItem->name->GetString());
	}
}

//////////////////////////////////////////////////////
//
//	AddItem
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(CNTokItem *tItem,CNTokItem *tGroupItem)
{
	tGroupItem->poGroup->nAdd(tItem);
}

//////////////////////////////////////////////////////
//
//	Remove Item
//
//////////////////////////////////////////////////////
void CNTok::vRemoveItem(CNTokItem *tItem)
{ 
	DLP_CPtrListItem *plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
		if(poTItem->nUid == tItem->nUid)
			{
				varItems->nRemoveListItem(plt);
				delete poTItem;
				return;
			}	

	    plt = nextPlt;
	    }

}

//////////////////////////////////////////////////////
//
//	Find Var Name 
//
//////////////////////////////////////////////////////
CNTokItem * CNTok::poFindVarName(char *val)
{
	DLP_CPtrListItem *plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
			if(poTItem->name->CompareNoCase(val) == 0)
			{
				return poTItem;
			}

	    plt = nextPlt;
	    }

	return 0;
}

//////////////////////////////////////////////////////
//
//	Find Var Name 
//
//////////////////////////////////////////////////////
int CNTok::nFindAndRemoveByName(char *val)
{
	DLP_CPtrListItem *plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
			if(poTItem->name->CompareNoCase(val) == 0)
			{
				varItems->nRemoveListItem(plt);
				delete poTItem;
				return 1;
			}	

	    plt = nextPlt;
	    }
	
	return 0;
}

//////////////////////////////////////////////////////
//
//	Load File 
//
//////////////////////////////////////////////////////
int CNTok::nLoadTokFile()
{
	fp = fopen(sFileName->GetString(),"r");

	if(fp == 0)
	{
		poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_ERROR
			,"Could Not find NDT File [%s]\n"
			,sFileName->GetString());
		return 0;
	}

	vParseTok();

	if(fp != 0)
	{
		fclose(fp);
		fp = 0;
	}

return 1;
}

//////////////////////////////////////////////////////
//
//	Load File 
//
//////////////////////////////////////////////////////
int CNTok::nWriteTokFile(char *filename)
{
	if(filename == 0)
	{
		filename = this->sFileName->GetString();
	}

	if(fp !=0)
	{
		fclose(fp);
	}
	
	//int nVal = 0;
	//nVal = DeleteFile(filename);
	fp = fopen(filename,"w+");

	if(fp == 0)
	{
		return 0;
	}
	
	fwrite("startConfig\n",sizeof(char),strlen("startConfig\n"),fp);

	CStr tStr;
	DLP_CPtrListItem *plt = varItems->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();

		//Write Comments
		vWriteComments(poTItem);
	    
			switch(poTItem->type)
			{
				case TOK_GROUP:
					{
						
						tStr.Format("varGroupStart,%s\n",poTItem->name->GetString());
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);

						int nVal = poTItem->poGroup->nGetSize();

						for(int i=0; i<nVal; i++)
						{
							CNTokItem *poTItem1 = (CNTokItem *)poTItem->poGroup->pGetAt(i);
							vWriteItem(poTItem1,fp);
						}

						tStr.Format("varGroupEnd,%s\n",poTItem->name->GetString());
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);

					}
					break;
			}

			fflush(fp);
	    plt = nextPlt;
	    }	
	
/*
	//Word
	DLP_CPtrListItem *plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
			switch(poTItem->type)
			{
				case TOK_WORD:
					{
						//Write Comments
						vWriteComments(poTItem);
						CStr tStr;
						tStr.Format("varWord,%s,%d\n",poTItem->name->GetString(),poTItem->varWord);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;
			};

	    plt = nextPlt;
	    }


	//Int
	plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
			switch(poTItem->type)
			{
				case TOK_INT:
					{
						//Write Comments
						vWriteComments(poTItem);
						CStr tStr;
						tStr.Format("varInt,%s,%d\n",poTItem->name->GetString(),poTItem->varInt);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;
			};

	    plt = nextPlt;
	    }

	//Float
	plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
			switch(poTItem->type)
			{
			
					case TOK_FLOAT:
					{
						//Write Comments
						vWriteComments(poTItem);
						CStr tStr;
						tStr.Format("varWord,%s,%f\n",poTItem->name->GetString(),poTItem->varFloat);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;
			};
	    plt = nextPlt;
	    }

	//String
	plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
			switch(poTItem->type)
			{
			
				case TOK_STRING:
					{
						//Write Comments
						vWriteComments(poTItem);
						CStr tStr;
						tStr.Format("varString,%s,%s\n",poTItem->name->GetString(),poTItem->varStr->GetString());
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;
			};
	    plt = nextPlt;
	    }

	//IP
	plt = varItems->poGetHeadItem();
	while (plt)
	    {
	    DLP_CPtrListItem *nextPlt = plt->pGetNext();
	    CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
	    
			switch(poTItem->type)
			{
				case TOK_IP:
					{
						//Write Comments
						vWriteComments(poTItem);
						CStr tStr;
						tStr.Format("varIP,%s,%d.%d.%d.%d\n",poTItem->name->GetString(),poTItem->ip[0],poTItem->ip[1],poTItem->ip[2],poTItem->ip[3]);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;
			};
	    plt = nextPlt;
	    }
*/
	fwrite("endConfig\n",sizeof(char),strlen("endConfig\n"),fp);

	if(fp !=0)
	{
		fclose(fp);
	}

return 1;
}

//////////////////////////////////////////////////////
//
//	vWriteItem 
//
//////////////////////////////////////////////////////
void CNTok::vWriteItem(CNTokItem *poTItem,FILE *fp)
{
		CStr tStr;
		//Write Comments
		vWriteComments(poTItem);
	    
			switch(poTItem->type)
			{
				case TOK_WORD:
					{
						tStr.Format("varWord,%s,%d\n",poTItem->name->GetString(),poTItem->varWord);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;

				case TOK_INT:
					{
						tStr.Format("varInt,%s,%d\n",poTItem->name->GetString(),poTItem->varInt);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;
				
				case TOK_FLOAT:
					{
						tStr.Format("varFloat,%s,%.02f\n",poTItem->name->GetString(),poTItem->varFloat);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;

				case TOK_STRING:
					{
						tStr.Format("varString,%s,%s\n",poTItem->name->GetString(),poTItem->varStr->GetString());
						int nVal = fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;

				case TOK_STRING_ENCRYPTED:
					{
						//tStr.Format("varEncryptedString,%s,%s\n",poTItem->name->GetString(),poTItem->varStr->GetString());
						//fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;

				case TOK_IP:
					{
						tStr.Format("varIP,%s,%d.%d.%d.%d\n",poTItem->name->GetString(),poTItem->ip[0],poTItem->ip[1],poTItem->ip[2],poTItem->ip[3]);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;

				case TOK_VECTOR3:
					{
						tStr.Format("varVector3,%s,%.02f,%.02f,%.02f\n",poTItem->name->GetString(),poTItem->poVector3->x,poTItem->poVector3->y,poTItem->poVector3->z);
						fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
					}
					break;
			}

			fflush(fp);
}

//////////////////////////////////////////////////////
//
//	Unloadfile 
//
//////////////////////////////////////////////////////
void CNTok::vUnLoadTokFile()
{
	if(fp != 0)
	{
		fclose(fp);

	}
	
	if(sFileName !=0)
	{
		delete sFileName;
	}
}

//////////////////////////////////////////////////////
//
//	Parase
//
//////////////////////////////////////////////////////
void CNTok::vParseTok()
{
CStr tBuffer;
char buffer[1024];
CNTokItem *poCurGroup = 0;
int loc = 0;

DLP_CPtrList *poTempCommentList = 0;

	for(int x=0; x<MAX_VAR; x++)
	{
		if (fgets(buffer,1024,fp) == 0) 
			{
				break;
			}
		else
			{
			tBuffer = buffer;
			}	

		
			CStr tStr;
			tBuffer.GetMiddle(0,2,&tStr);
			
			//Found Comment
			if(tStr.CompareNoCase("//") == 0)
			{

				if(poTempCommentList == 0)
				{
					poTempCommentList = new DLP_CPtrList(poDebug);
				}

				tBuffer.RemoveLeft(2);
				CStr *tComment = new CStr;
				tComment->Format("%s",tBuffer.GetString());
				poTempCommentList->vAddTail(tComment);
				continue;
			}


//////////////////////////
			loc = tBuffer.Find("varGroupStart,",0);
			if (loc >= 0) 
				{
					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					
					//Remove the CR
					int len3 = strlen(tBuf2);
					tBuf2[len3] = 0;

					if(poCurGroup != 0)
					{
						CStr oeStr;
						oeStr.Format("Missing GroupEnd %s",poCurGroup->name->GetString());
						MessageBox(NULL,oeStr.GetString(),"Config Error",MB_OK);
						exit(0);
					}

					CNTokItem *tGroupItem  = poFindVarName(tBuf1);
					if(tGroupItem != 0)
					{
						CStr oeStr;
						oeStr.Format("Duplicate GroupStart %s",tBuf1);
						MessageBox(NULL,oeStr.GetString(),"Config Error",MB_OK);
						exit(0);
					}

					int nGroupType = 0;
					CNTokItem *tItem = new CNTokItem(tBuf1,TOK_GROUP,poDebug);
					poCurGroup = tItem;

					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}

					m_poMasterGroup->nAdd(tItem);
					vAddItem(tItem);
				}

//////////////////////////////////////
			loc = tBuffer.Find("varGroupEnd,",0);
			if (loc >= 0) 
				{
					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					
					//Remove the CR
					int len3 = strlen(tBuf2);
					tBuf2[len3] = 0;

					CNTokItem *tGroupItem  = poFindVarName(tBuf1);
					if(tGroupItem == 0)
					{
						CStr oeStr;
						oeStr.Format("GroupStart Not Found for %s",tBuf1);
						MessageBox(NULL,oeStr.GetString(),"Config Error",MB_OK);
						exit(0);
					}

					poCurGroup = 0;
				}

//////////////////////////////////////

			loc = tBuffer.Find("varWord,",0);
			if (loc >= 0) 
				{
					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					int loc3 = tBuffer.Find(",",loc2+1);
					tBuffer.Replace('\0',loc3);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					char *tBuf3 = tBuffer.GetAtPos(loc3+1);
					
					//Remove the CR
					int len3 = strlen(tBuf3);
					tBuf3[len3] = 0;
					
					CNTokItem *tItem = new CNTokItem((char)atoi(tBuf2),tBuf1,poDebug);

					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}

					vAddItem(tItem,poCurGroup);
				}


			loc = tBuffer.Find("varInt,",0);
			if (loc >= 0) 
				{

					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					
					CNTokItem *tItem = new CNTokItem((int)atoi(tBuf2),tBuf1,poDebug);

					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}

						vAddItem(tItem,poCurGroup);
				}


			loc = tBuffer.Find("varFloat,",0);
			if (loc >= 0) 
				{

					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					
					CNTokItem *tItem = new CNTokItem((float)atof(tBuf2),tBuf1,poDebug);
					
					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}
					
						vAddItem(tItem,poCurGroup);
				}

/*
			loc = tBuffer.Find("varEncryptedString,",0);
			if (loc >= 0) 
				{

					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					
					//Remove the CR
					int len3 = strlen(tBuf2);
					tBuf2[len3] = 0;

					CNTokItem *tItem = new CNTokItem(tBuf2,tBuf1,poGlobalKey->GetString(),poDebug);

					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}

					AddItem(tItem,poCurGroup);
				}

*/
			loc = tBuffer.Find("varString,",0);
			if(loc == -1)
			{
				loc = tBuffer.Find("varStr,",0);
			}
			if (loc >= 0) 
				{

					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					
					//Remove the CR
					int len3 = strlen(tBuf2);
					tBuf2[len3] = 0;

					CNTokItem *tItem = new CNTokItem(tBuf2,tBuf1,poDebug);

					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}

					vAddItem(tItem,poCurGroup);
				}

//////////////////////////////////////////////////

			loc = tBuffer.Find("varIP,",0);
			if (loc >= 0) 
				{

					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					

					CStr temp1(tBuf2);
					int iploc1 = temp1.Find(".",0);
					temp1.Replace('\0',iploc1);

					int iploc2 = temp1.Find(".",iploc1+1);
					temp1.Replace('\0',iploc2);
					
					int iploc3 = temp1.Find(".",iploc2+1);
					temp1.Replace('\0',iploc3);

					char *tBufa1 = temp1.GetAtPos(0);
					char *tBufa2 = temp1.GetAtPos(iploc1+1);
					char *tBufa3 = temp1.GetAtPos(iploc2+1);
					char *tBufa4 = temp1.GetAtPos(iploc3+1);

					CNTokItem *tItem = new CNTokItem(atoi(tBufa1),atoi(tBufa2),atoi(tBufa3),atoi(tBufa4),tBuf1,poDebug);
					
					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}
					
					vAddItem(tItem,poCurGroup);
				}
////////////////////////////////////////
			loc = tBuffer.Find("varVector3,",0);
			if (loc >= 0) 
				{
					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					

					CStr temp1(tBuf2);
					int iploc1 = temp1.Find(",",0);
					temp1.Replace('\0',iploc1);

					int iploc2 = temp1.Find(",",iploc1+1);
					temp1.Replace('\0',iploc2);
					
					int iploc3 = temp1.Find(",",iploc2+1);
					temp1.Replace('\0',iploc3);

					char *tBufa1 = temp1.GetAtPos(0);
					char *tBufa2 = temp1.GetAtPos(iploc1+1);
					char *tBufa3 = temp1.GetAtPos(iploc2+1);
		
					CNTokItem *tItem = new CNTokItem((float)atof(tBufa1),(float)atof(tBufa2),(float)atof(tBufa3),tBuf1,poDebug);
					
					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}
					
					vAddItem(tItem,poCurGroup);
				}


//////////////////////////
			loc = tBuffer.Find("varGroup,",0);
			if (loc >= 0) 
				{
					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					
					//Remove the CR
					int len3 = strlen(tBuf2);
					tBuf2[len3] = 0;

					int nGroupType = 0;
					nGroupType = nFindGroupType(tBuf2);
					CNTokItem *tItem = new CNTokItem(tBuf1,nGroupType,poDebug);

					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}

					vAddItem(tItem);
				}

//////////////////////////////////////

			loc = tBuffer.Find("varFile,",0);
			if (loc >= 0) 
				{

					int nFind = tBuffer.Find("\n",0);
					if(nFind > 0)
					{
						tBuffer.Replace('\0',nFind);
					}

					int loc1 = tBuffer.Find(",",0);
					tBuffer.Replace('\0',loc1);

					int loc2 = tBuffer.Find(",",loc1+1);
					tBuffer.Replace('\0',loc2);

					char *tBuf1 = tBuffer.GetAtPos(loc1+1);
					char *tBuf2 = tBuffer.GetAtPos(loc2+1);
					

					CStr temp1(tBuf2);
					int iploc1 = temp1.Find(",",0);
					temp1.Replace('\0',iploc1);

					int iploc2 = temp1.Find(",",iploc1+1);
					temp1.Replace('\0',iploc2);
					
					int iploc3 = temp1.Find(",",iploc2+1);
					temp1.Replace('\0',iploc3);

					int iploc4= temp1.Find(",",iploc3+1);
					temp1.Replace('\0',iploc4);


					char *tBufa1 = temp1.GetAtPos(0);
					char *tBufa2 = temp1.GetAtPos(iploc1+1);
					char *tBufa3 = temp1.GetAtPos(iploc2+1);
					char *tBufa4 = temp1.GetAtPos(iploc3+1);
					char *tBufa5 = temp1.GetAtPos(iploc4+1);

					CNTokItem *tItem = new CNTokItem(tBufa1,tBufa2,tBufa3,atoi(tBufa4),tBufa5,tBuf1,poDebug);
					
					if(poTempCommentList != 0)
					{
						//Move Comments to Item
						DLP_CPtrListItem *plt = poTempCommentList->poGetHeadItem();
						while (plt)
							{
								DLP_CPtrListItem *nextPlt = plt->pGetNext();
								CStr *pStr = (CStr *)plt->pGetPtr();
								tItem->poCommentList->vAddTail(pStr);
								plt = nextPlt;
							}

						poTempCommentList->vRemoveAll();
						delete poTempCommentList;					
						poTempCommentList = 0;
					}
					
					vAddItem(tItem,poCurGroup);
				}
	}

}

//////////////////////////////////////////////////////
//
//	nGetNumItems
//
//////////////////////////////////////////////////////
int CNTok::nGetNumItems(void)
{
	return varItems->nGetCount();
}

//////////////////////////////////////////////////////
//
//	Write Comments
//
//////////////////////////////////////////////////////
void CNTok::vWriteComments(CNTokItem *poTItem)
{
	//Write Comments from Item
	DLP_CPtrListItem *plt1 = poTItem->poCommentList->poGetHeadItem();
	while (plt1)
		{
			DLP_CPtrListItem *nextPlt1 = plt1->pGetNext();
			CStr *pStr = (CStr *)plt1->pGetPtr();
			
			CStr tStr;
			tStr.Format("//%s",pStr->GetString());
			fwrite(tStr.GetString(),sizeof(char),tStr.GetLength(),fp);
			plt1 = nextPlt1;
		}

}

//////////////////////////////////////////////////////
//
//	Add Group
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(char *tname,int ntype)
{
	CNTokItem *poTokItem = new CNTokItem(tname,ntype,poDebug);
	this->vAddItem(poTokItem);
}

//////////////////////////////////////////////////////
//
//	Add Item Char
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(char nVal, char *tname)
{
	CNTokItem *poTokItem = new CNTokItem(nVal,tname,poDebug);
	this->vAddItem(poTokItem);
}

//////////////////////////////////////////////////////
//
//	Add Item Int
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(int nVal, char *tname)
{
	CNTokItem *poTokItem = new CNTokItem(nVal,tname,poDebug);
	this->vAddItem(poTokItem);
}

//////////////////////////////////////////////////////
//
//	Add Item Float
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(float fVal, char *tname)
{
	CNTokItem *poTokItem = new CNTokItem(fVal,tname,poDebug);
	this->vAddItem(poTokItem);
}

//////////////////////////////////////////////////////
//
//	Add Item String
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(char *pStr, char *tname)
{
	CNTokItem *poTokItem = new CNTokItem(pStr,tname,poDebug);
	this->vAddItem(poTokItem);
}


//////////////////////////////////////////////////////
//
//	Add Itemp IP
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(int ip1,int ip2, int ip3, int ip4, char *tname)
{
	CNTokItem *poTokItem = new CNTokItem(ip1,ip2,ip3,ip4,tname,poDebug);
	this->vAddItem(poTokItem);
}

//////////////////////////////////////////////////////
//
//	Add Vector3
//
//////////////////////////////////////////////////////
void CNTok::vAddItem(float fx,float fy,float fz, char *tname)
{
	CNTokItem *poTokItem = new CNTokItem(fx,fy,fz,tname,poDebug);
	this->vAddItem(poTokItem);
}

//////////////////////////////////////////////////////
//
//	nNumOfType
//
//////////////////////////////////////////////////////
int CNTok::nNumOfType(int nType)
{
	int nVal = 0;
	DLP_CPtrListItem *plt = varItems->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CNTokItem *poTItem = (CNTokItem *)plt->pGetPtr();
		    
			if(poTItem->type == nType)
			{
				nVal++;
			}

			plt = nextPlt;
	    }

return nVal;
}

//////////////////////////////////////////////////////
//
//	Find Group Type
//
//////////////////////////////////////////////////////
int CNTok::nFindGroupType(const char *type)
{
	CStr oStr;
	for(int i=0; i<A_TYPE__TOTAL; i++)
	{
		oStr.Format("%s",pGroupTypes[i]);
		if(oStr.CompareNoCase(type) == 0)
		{
			return i;
		}
	}

return -1;
}

//////////////////////////////////////////////////////
//
//	nGetNumGroups
//
//////////////////////////////////////////////////////
int CNTok::nGetNumGroups(void)
{
	return m_poMasterGroup->nGetSize();
}

//////////////////////////////////////////////////////
//
//	poGetGroup
//
//////////////////////////////////////////////////////
CNTokItem *CNTok::poGetGroup(int nVal)
{
	CNTokItem *poTok = 0;
	poTok = (CNTokItem *)m_poMasterGroup->pGetAt(nVal);

return poTok;
}


//////////////////////////////////////////////////////
//
//	Item Types
//
//////////////////////////////////////////////////////
const char * CNTok::pItemTypes[] =
{
	"Word",
	"Int",
	"Float",
	"String",
	"Ip",
	"File",
	"Encrypt",
	"Ecrypted",
	"Vector3",
	"Group",
	"Group Item",
	"Group",
};

//////////////////////////////////////////////////////
//
//	Group Types
//
//////////////////////////////////////////////////////
const char *CNTok::pGroupTypes[] =
{
	"TypeWord",
	"TypeInt",
	"TypeFloat",
	"TypeString",
	"TypeIp",
	"TypeFile",
	"TypeEncrypt",
	"TypeEcrypted",
	"TypeVector3",
};


///////////////////////////////////////////////////////////////////////////////
#if 0
/*
//Example .Cfg
startConfig
varGroupStart,Server
varIP,IP,127.0.0.1
varInt,PORT,2828
varGroupEnd,Server
varGroupStart,Graphics
varString,Name,Test123
varInt,Number,999
varGroupEnd,Graphics
varGroupStart,TestGroup1
varString,NameTest,Test1234
varFloat,FloatTest,123.01
varGroupEnd,TestGroup1
endConfig
*/
//////////////////////////////////////////////////////
//
//	vReadTest
//
//////////////////////////////////////////////////////
void vReadTest()
{
	//Open Tok File
	CNTok oTok("test.cfg",poEngine->poDebug);
	CNTokItem *tTokItem = 0;

	//Find Group Name
	tTokItem = oTok.poFindVarName("Graphics");
	if(tTokItem != 0)
	{
		CNTokItem *pItemTok = 0;
	
		//Find String Item in Group
		pItemTok = tTokItem->poGetGroupItem("Name");
		if(pItemTok != 0)
		{
			CStr oStr;
			oStr.Format("%s",pItemTok->varStr->GetString());
		}

		//Find Int Item in group
		pItemTok = tTokItem->poGetGroupItem("Number");
		if(pItemTok != 0)
		{
			int nVal = 0;
			nVal = pItemTok->varInt;
		}
	}
}

//////////////////////////////////////////////////////
//
//	vWriteTest
//
//////////////////////////////////////////////////////
void vWriteTest()
{
	//Open Tok
	CNTok oTok("test.cfg",poEngine->poDebug);
	CNTokItem *tTokItem = 0;

	//Find Group
	tTokItem = oTok.poFindVarName("Graphics");
	if(tTokItem != 0)
	{
		CNTokItem *pItemTok = 0;
	
		//Get String Group Item
		pItemTok = tTokItem->poGetGroupItem("Name");
		if(pItemTok != 0)
		{
			CStr oStr;
			pItemTok->varStr->Format("WriteTest");
		}

		//Get Int Group Item
		pItemTok = tTokItem->poGetGroupItem("Number");
		if(pItemTok != 0)
		{
			
			pItemTok->varInt = 999;
		}
	}

	//Write Tok
	oTok.nWriteTokFile("test1.cfg");
}

//////////////////////////////////////////////////////
//
//	vModWriteTest
//
//////////////////////////////////////////////////////
void vModWriteTest()
{
	//Open Tok
	CNTok oTok("test.cfg",poEngine->poDebug);
	
	//Create new Group
	CNTokItem *poGroupItem = new CNTokItem("TestGroup1",TOK_GROUP,poEngine->poDebug);
	
	//Add Group to Tok
	oTok.vAddItem(poGroupItem);

	//Create string new Item
	CNTokItem *poItem = new CNTokItem((char *)"Test1234","NameTest",poEngine->poDebug);

	//Add it to group
	oTok.vAddItem(poItem,poGroupItem);

	//Create float new Item
	poItem = new CNTokItem((float)123.01f,"FloatTest",poEngine->poDebug);

	//Add it to group
	oTok.vAddItem(poItem,poGroupItem);

	//Write Tok
	oTok.nWriteTokFile("test1.cfg");
}
#endif