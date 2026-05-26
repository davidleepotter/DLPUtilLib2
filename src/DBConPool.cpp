/*********************************************************************
*	File : CDBConPoll.cpp 
*	Desc :  DataBase Connection Pool
*
*
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 
#include "utilheader.h"

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CDBConPool::CDBConPool(CDebugInfo *potDebug,int ntMaxPool,int ntType,char *database, char *table, char *username, char *password, char *ipaddress, int ntPort)
{
	poFreeList	= new DLP_CPtrList(potDebug);
	poInUseList	= new DLP_CPtrList(potDebug);
	poDebug = potDebug;


	nMaxPool	= ntMaxPool;
	nType		= ntType;
	nPort		= ntPort;

	poDatabase	= new CStr;
	poBaseTable	= new CStr;
	poUsername	= new CStr;
	poPassword	= new CStr;
	poIpAddress	= new CStr;

	poDatabase->Format("%s",database);
	poBaseTable->Format("%s",table);
	poUsername->Format("%s",username);
	poPassword->Format("%s",password);
	poIpAddress->Format("%s",ipaddress);


	poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Created a DB Connection Pool of %d Connections\n"
		,poFreeList->nGetCount());
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CDBConPool::~CDBConPool()
{
	DLP_CPtrListItem *plt = poFreeList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CDbase *poDBase = (CDbase *)plt->pGetPtr();
			
			delete poDBase;

			plt = nextPlt;
	    }
	
	poFreeList->vRemoveAll();

	delete poFreeList;

	plt = poInUseList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CDbase *poDBase = (CDbase *)plt->pGetPtr();
			
			delete poDBase;

			plt = nextPlt;
	    }
	
	poInUseList->vRemoveAll();

	delete poInUseList;

	delete poDatabase;
	delete poBaseTable;
	delete poUsername;
	delete poPassword;
	delete poIpAddress;
}

//////////////////////////////////////////////////////
//
//	Init The Connection Pool
//
//////////////////////////////////////////////////////
int CDBConPool::nInitConPool(void)
{
	//Remove all the Free
	DLP_CPtrListItem *plt = poFreeList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CDbase *poDBase = (CDbase *)plt->pGetPtr();
			
			delete poDBase;

			plt = nextPlt;
	    }
	
	poFreeList->vRemoveAll();


	//Remove all the in use
	plt = poInUseList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CDbase *poDBase = (CDbase *)plt->pGetPtr();
			
			delete poDBase;

			plt = nextPlt;
	    }
	
	poInUseList->vRemoveAll();

	//Init the DB Connection Pool
	for(int x=0; x<nMaxPool; x++)
	{
		CDbase *poDBase = 0;
		poDBase = poCreateDb();

		if(poDBase == 0)
		{
			//DB Error
			return 0;
		}
		poFreeList->vAddTail(poDBase);
	}

return 1;
}

//////////////////////////////////////////////////////
//
//	Constructor type float
//
//////////////////////////////////////////////////////
CDbase * CDBConPool::poGetNextConnection(void)
{
	//Go thru and clean up stale connections
	if(poFreeList->nGetCount() > this->nMaxPool)
	{
		int nOverCount = poFreeList->nGetCount() - this->nMaxPool;

		if(nOverCount > 10)
		{
			for(int x=0; x<nOverCount; x++)
			{
				//Check for timeout
				DLP_CPtrListItem *plt = poFreeList->poGetHeadItem();
				CDbase *poDBase = (CDbase *)plt->pGetPtr();

				if(poDBase->nInUse == 1)
				{		
					poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"DB Connection %d InUse in FreeList\n",poDBase->nUID);
					break;
				}

				poFreeList->nRemoveListItem(plt);
				delete poDBase;
				poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"DB Connection Expired %d\n",poDBase->nUID);
			}	
		}
	}

	//Make sure I have atleast 1 connection in pool
	if(poFreeList->nGetCount() <= 0)
	{
		CDbase *potDBase = 0;
		potDBase = poCreateDb();
		poFreeList->vAddTail(potDBase);
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"Out of Database connections adding one\n");
	}

	//Add to used list
	CDbase *poDBase = 0;
	DLP_CPtrListItem *plt1 = poFreeList->poGetHeadItem();
	poDBase = (CDbase *)plt1->pGetPtr();
	poFreeList->nRemoveListItem(plt1);
	poInUseList->vAddTail(poDBase);

poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Got DB Connection %d\n",poDBase->nUID);
poDBase->nInUse = 1;
return poDBase;
}
//////////////////////////////////////////////////////
//
//	Create DB
//
//////////////////////////////////////////////////////
int CDBConPool::nReleaseConnection(CDbase *poUsedDBase)
{

	DLP_CPtrListItem *plt = poInUseList->poGetHeadItem();
	while (plt)
	    {
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CDbase *poDBase = (CDbase *)plt->pGetPtr();
			

			if(poUsedDBase->nUID == poDBase->nUID)
			{
				poInUseList->nRemoveListItem(plt);
				poFreeList->vAddTail(poDBase);
				poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Released DB Connection %d\n",poDBase->nUID);
				poDBase->nInUse = 0;
				return 1;
			}

			plt = nextPlt;
	    }
	
poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_ERROR,"Could not Release DB Connection %d\n",poUsedDBase->nUID);
return 0;
}

//////////////////////////////////////////////////////
//
//	Create DB
//
//////////////////////////////////////////////////////
CDbase * CDBConPool::poCreateDb(void)
{
			CDbase *poDBase = 0;
			switch(nType)
			{
				case DB_TYPE_MYSQL:
					{
						CDBMySql *poMySql = 0;
						poMySql = new CDBMySql(poDebug);
						poDBase = (CDbase *)poMySql;
						poDBase->vSetType(nType);
					}
					break;

				case DB_TYPE_ORACLE:
						{
						}
						break;


				case DB_TYPE_ODBC:
					{
#ifndef NO_ODBC 
#ifdef WIN32
						CDBOdbc *poODBC = 0;
						poODBC = new CDBOdbc(poDebug);
						poDBase = (CDbase *)poODBC;
						poDBase->vSetType(nType);
#endif
#endif

					}
					break;

			}
		
		int nRetVal = poDBase->nConnectToDb(poDatabase->GetString()
									,poBaseTable->GetString()
									,poIpAddress->GetString()
									,nPort
									,poUsername->GetString()
									,poPassword->GetString());

	if(nRetVal == 0)
	{
		return 0;
	}

return poDBase;
}
