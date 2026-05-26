// dbquery.h: interface for the CDBQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBQUERY_H__3CBEA2EB_1C1C_43FF_9F75_E680BCBF386D__INCLUDED_)
#define AFX_DBQUERY_H__3CBEA2EB_1C1C_43FF_9F75_E680BCBF386D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDBMySql;
class CDBFlatFile;
class CStr;
class DLP_CPtrList;
class CDebugInfo;
class CDBOdbc;
class CNetManObj;
class DLP_CListAccess;
class CDbase;
class CDBMySql;
class CDBOdbc;
class CDBQuery  
{

private:
	DLP_CPtrList *poQueryList;		//List of all Queries
	DLP_CPtrList *poQueryBulkList;	//List of all Bulk Queries
	CStr *poStrQuery;				//Actual String Query
	int nType;
	CDebugInfo *poDebug;
	CStr *poOper;
	int nOperType;
	int nAutoInc;

public:
	friend CDbase;
	friend CDBMySql;
	friend CDBOdbc;

	CDBQuery(int nttype,CDebugInfo *poDebug);
	virtual ~CDBQuery();

	DLP_CListAccess *poQueryBulkListAccess;
	
	
	int nAddQuery(char *data);
	int nAddQuery(int nData);
	int nAddQuery(float uData);
	int nAddQuery(unsigned long ulData);
	int nAddQuery(long ulData);


	int nAddQuery(int nArg,char *data);
	int nAddQuery(int nArg,int nData);
	int nAddQuery(int nArg,float uData);
	int nAddQuery(int nArg,unsigned long ulData);
	int nAddQuery(int nArg,long ulData);
	
	void vClear(void);
	void vClearBulkList(void);
	void vSetOper(int nOType);
	void vSetAutoInc(int nArg);
	void vClearAutoInc(void);
	CStr *poGetOper(void);

};

#endif // !defined(AFX_DBQUERY_H__3CBEA2EB_1C1C_43FF_9F75_E680BCBF386D__INCLUDED_)
