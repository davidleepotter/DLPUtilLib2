/*********************************************************************
*	File : Global Defines 
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/
#if !defined(AFX_UTILDEFINE_H__2A48417C_01A9_46FD_92AE_3C33AF8C21BF__INCLUDED_)
#define AFX_UTILDEFINE_H__2A48417C_01A9_46FD_92AE_3C33AF8C21BF__INCLUDED_

//Byte Order
enum
{
	NETMAN_BYTE_ORDER = 1,
};

//Timer
enum
{
	TIMER_PAUSED,
	TIMER_RUNNING,
};

//Unique ID Type
enum
{
	UID_CHAR,
	UID_DCHAR,
	UID_ULONG,
};

//Mutex Debug Info
enum
{
	MUTEX_DEBUG,
	MUTEX_NO_DEBUG,
};

//OperTypes
enum
{
	OPER_EQUAL,
	OPER_NOT_EQUAL,	
	OPER_LESS,
	OPER_GREATER,
	OPER_LESS_EQUAL,
	OPER_GREATE_EQUAL,
};

//Requery
enum
{
	DB_NO_REQUERY,		//No Query if you know you are in the correct row query
	DB_REQUERY,			//Requery for each row query
	DB_BULK_QUERY,		//Bulk Row Col Query
	DB_FINISH_QUERY,	//Finish Query when you are finished with that row query
	DB_SINGLE_QUERY,
	DB_DISTINCT,		//Distinct multi query
	DB_NO_DISTINCT,		//Non Distinct multi query
	DB_SELECT_ALL,		//Select ALL

};

//Insert Dupe
enum
{
	DB_ALLOW_DUPE,
	DB_NO_DUPE,
};

//DB Types
enum
{
	DB_STRING,
	DB_INT,
	DB_FLOAT,
	DB_LONG,
	DB_BINARY,
	DB_MULT,
	DB_UNK,
};

//DB Functions
enum
{
	DB_INSERT,
	DB_MODIFY,
	DB_DELETE,
	DB_SELECT,
	DB_SELECT_DATA,
};

//MYSQL Define
enum
{
	DB_TABLE_EXISTS = 1050,
};

//DB Types
enum
{
	DB_TYPE_FLATFILE,
	DB_TYPE_MYSQL,
	DB_TYPE_ORACLE,
	DB_TYPE_ODBC,
};
//DB Misc
enum
{
	DB_NO_AUTO_INC = -1,
};

//Max Num of DB Data args
enum
{
	DB_ARG1,
	DB_ARG2,
	DB_ARG3,
	DB_ARG4,
	DB_ARG5,
	DB_ARG6,
	DB_ARG7,
	DB_ARG8,
	DB_ARG9,
	DB_ARG10,
	DB_ARG11,
	DB_ARG12,
	DB_ARG13,
	DB_ARG14,
	DB_ARG15,
	DB_ARG16,
	DB_ARG17,
	DB_ARG18,
	DB_ARG19,
	DB_ARG20,
	DB_ARG21,
	DB_ARG22,
	DB_ARG23,
	DB_ARG24,
	DB_ARG25,
	DB_ARG26,
	DB_ARG27,
	DB_ARG28,
	DB_ARG29,
	DB_ARG30,
	DB_ARG_CNT,
};

//Debug Levels
enum
{
	DEBUG_LEVEL1,
	DEBUG_LEVEL2,
	DEBUG_LEVEL3,
	DEBUG_LEVEL4,
	DEBUG_LEVEL5,
};

//Debug Types
enum
{
	DEBUG_INFO,
	DEBUG_ERROR,
	DEBUG_DATA,
	DEBUG_CLEAN,
};


//List Query Nums
enum
{
	LIST_QUERY1,
	LIST_QUERY2,
	LIST_QUERY3,
	LIST_QUERY4,
	LIST_QUERY5,
	LIST_QUERY6,
	LIST_QUERY7,
	LIST_QUERY8,
	LIST_QUERY9,
	LIST_QUERY10,
};

enum
{	
	MAX_MSG_BUFFER = 1024*13,
};

//Msg Elements
enum
{
	MSG_TOKEN,
	MSG_BYTE,
	MSG_UBYTE,
	MSG_INT,
	MSG_FLOAT,
	MSG_UINT,
	MSG_LONG,
	MSG_ULONG,
	MSG_STRING,
	MSG_CHUNK,
	MSG_INT_ARRAY,
};


#ifdef WIN32
#pragma warning( disable : 4355 4100 4127 4512 4702 4511 4514 4800 4710 4516)
#endif

#endif