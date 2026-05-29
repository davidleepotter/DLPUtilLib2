/*********************************************************************
*	File : DBQuery.cpp 
*	Desc :  DB Query Class
*
*
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 
#include "UtilHeader.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBQuery::CDBQuery(int nttType,CDebugInfo *potDebug)
{
	nType = nttType;
	nAutoInc = DB_NO_AUTO_INC;
	poStrQuery = new CStr;
	poQueryList = new DLP_CPtrList(potDebug);
	poQueryBulkList	= new DLP_CPtrList(potDebug);
	poQueryBulkListAccess = new DLP_CListAccess(poQueryBulkList,potDebug);
	poDebug = potDebug;
	poOper = new CStr;
	vSetOper(OPER_EQUAL);
}

CDBQuery::~CDBQuery()
{
	delete poStrQuery;

	DLP_CPtrListItem *plt = poQueryList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CQuery *poQuery = (CQuery *)plt->pGetPtr();
			
			delete poQuery;

			plt = nextPlt;
	    }
	
	poQueryList->vRemoveAll();
	delete poQueryList;

	vClearBulkList();
	poQueryBulkList->vRemoveAll();
	delete poQueryBulkListAccess;
	delete poQueryBulkList;
	
	if(poOper != 0)
	{
		delete poOper;
	}	

}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
void CDBQuery::vClearBulkList(void)
{
	if(poQueryBulkList == 0)
	{
		return;
	}

	if(poQueryBulkList->nGetCount() <= 0)
	{
		return;
	}

	DLP_CPtrListItem *plt = poQueryBulkList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CQuery *poQuery = (CQuery *)plt->pGetPtr();
			
			delete poQuery;

			plt = nextPlt;
	    }
	
	poQueryBulkList->vRemoveAll();
}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(char *data)
{	
	CQuery *poQuery = new CQuery(DB_STRING,data);

	//Path Kludge
	if(nType == DB_TYPE_MYSQL)
	{
		poQuery->poString->nReplaceString("\\","\\\\");
	}

	poQueryList->vAddTail(poQuery);	
return 1;
}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(int nData)
{
	CQuery *poQuery = new CQuery(DB_INT,nData);
	poQueryList->vAddTail(poQuery);
return 1;
}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(float uData)
{
	CQuery *poQuery = new CQuery(DB_FLOAT,uData);
	poQueryList->vAddTail(poQuery);
return 1;
}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(unsigned long ulData)
{
	CQuery *poQuery = new CQuery(DB_LONG,ulData);
	poQueryList->vAddTail(poQuery);
return 1;
}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(long ulData)
{
	CQuery *poQuery = new CQuery(DB_LONG,(unsigned long)ulData);
	poQueryList->vAddTail(poQuery);
return 1;
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(int nArg,char *data)
{	
	CQuery *poQuery = new CQuery((char)nArg,DB_STRING,data);

	//Path Kludge
	if(nType == DB_TYPE_MYSQL)
	{
		poQuery->poString->nReplaceString("\\","\\\\");
	}

	poQueryList->vAddTail(poQuery);	
return 1;
}
//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(int nArg,int nData)
{
	CQuery *poQuery = new CQuery((char)nArg,DB_INT,nData);
	poQueryList->vAddTail(poQuery);
return 1;
}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(int nArg,float uData)
{
	CQuery *poQuery = new CQuery((char)nArg,DB_FLOAT,uData);
	poQueryList->vAddTail(poQuery);
return 1;
}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(int nArg,unsigned long ulData)
{
	CQuery *poQuery = new CQuery((char)nArg,DB_LONG,ulData);
	poQueryList->vAddTail(poQuery);
return 1;
}

//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
int CDBQuery::nAddQuery(int nArg,long ulData)
{
	CQuery *poQuery = new CQuery((char)nArg,DB_LONG,(unsigned long)ulData);
	poQueryList->vAddTail(poQuery);
return 1;
}
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////
//
//	
//
//////////////////////////////////////////////////////
void CDBQuery::vClear(void)
{
	poStrQuery->Empty();
	vSetOper(OPER_EQUAL);

	DLP_CPtrListItem *plt = poQueryList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CQuery *poQuery = (CQuery *)plt->pGetPtr();
			
			delete poQuery;

			plt = nextPlt;
	    }
	
	poQueryList->vRemoveAll();
	poQueryBulkList->vRemoveAll();

}

/////////////////////////////////////////////////////
//
//	Set Oper
//
//////////////////////////////////////////////////////
void CDBQuery::vSetOper(int nOType)
{
	switch(nOType)
	{

	case OPER_EQUAL:
		{
			poOper->Format("=");
			nOperType = nOType;
		}
		break;

	case OPER_NOT_EQUAL:
		{
			poOper->Format("!=");
			nOperType = nOType;
		}
		break;

	case OPER_LESS:
		{
			poOper->Format("<");
			nOperType = nOType;
		}
		break;
	
	case OPER_GREATER:
		{
			poOper->Format(">");
			nOperType = nOType;
		}
		break;
	
	case OPER_LESS_EQUAL:
		{
			poOper->Format("<=");
			nOperType = nOType;
		}
		break;

	case OPER_GREATE_EQUAL:
		{
			poOper->Format(">=");
			nOperType = nOType;
		}
		break;
	};
}

//////////////////////////////////////////////////////
//
//	Get Oper
//
//////////////////////////////////////////////////////
CStr * CDBQuery::poGetOper(void)
{
	return poOper;
}


//////////////////////////////////////////////////////
//
// Clear Auto Inc
//
//////////////////////////////////////////////////////
void CDBQuery::vClearAutoInc(void)
{
	nAutoInc = DB_NO_AUTO_INC;
}

//////////////////////////////////////////////////////
//
//	Set Auto Inc
//
//////////////////////////////////////////////////////
void CDBQuery::vSetAutoInc(int nArg)
{
	nAutoInc = nArg;
}
