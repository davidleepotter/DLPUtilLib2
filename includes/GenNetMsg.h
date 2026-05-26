/*********************************************************************
*	File :	GenNetMsg.h
*	Desc :	Generic Network Message Object
*			
*
*
*
*
*	Date: 4/23/2003
*********************************************************************/
#if !defined(AFX_GENNETMSG_H__F507694B_EF04_4947_9DE3_EDB2A33E4CE7__INCLUDED_)
#define AFX_GENNETMSG_H__F507694B_EF04_4947_9DE3_EDB2A33E4CE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory.h>

#include "netmutex.h"

class CDebugInfo;
class DLP_CPtrList;
class CGenNetMsg;
class CStr;
class ARACrypt;

class CDataBlock
{

private:
	CDebugInfo *poDebug;
	unsigned char *poStartData;
	unsigned char *poCurData;
	long nCount;
	long nSize;


public:

	CDataBlock(CDebugInfo *potDebug,unsigned char *data);
	~CDataBlock();
	void vGetToken(unsigned char *pChar);
	void vGetByte(char *pChar);
	void vGetUByte(unsigned char *pChar);
	void vGetInt(int *pChar);
	void vGetUInt(unsigned int *pChar);
	void vGetLong(long *pChar);
	void vGetULong(unsigned long *pChar);
	void vGetFloat(float *pChar);
	void vGetString(char *pChar, int nLen);
	void vGetChunk(char *pChar, int nLen);
	void vGetIntArray(char *pChar,int nLen);
	void vGetNextItem(int nItemSize);
	void vGetNextItemAtOffset(long nOffset,int nItemSize);
	void vReset(void);

};

class CMsgHeader
{
private:

public:
	friend CGenNetMsg;

	CMsgHeader()
	{
		nEncrypt		= 0;
		nHeaderSize		= 0;
		nDataSize		= 0;
		nCrc			= 0;
		nNumElements	= 0;
		nIdent			= 0;

	}

	virtual ~CMsgHeader()
	{

	}

	char nEncrypt;
	long nIdent;
	int nHeaderSize;
	int nDataSize;
	long nCrc;
	char nNumElements;
	

	int nGetHeaderSize()
	{
		int nVal = 0;
		nVal += sizeof(long);					//Message Identifer
		nVal += sizeof(int);					//Size of data
		nVal += sizeof(char);					//Comprssed
		nVal += sizeof(char);					//Encrypted
		nVal += sizeof(long);					//Crc
		nVal += sizeof(char);					//Num Elements
		nVal += sizeof(char) * nNumElements;	//Ele Types

		nHeaderSize = nVal;
		return nVal;
	}

};


class CMsgElement
{
private:
	CDebugInfo *poDebug;
	char nType;
	long nUID;
	static unsigned long g_uID;

	bool			bInUse;
	unsigned char	nToken;
	char			nByte;
	unsigned char	nUByte;
	int				nInt;
	unsigned int	nUInt;
	float			fFloat;
	long			nLong;
	unsigned long	nULong;
	CStr			*poString;
	char			*pChunk;
	int				nChunkSize;
	int				*pIntArray;
	int				nIntArraySize;

	unsigned long ulGLOBALGetUid(void);

public:
	friend CGenNetMsg;

	CMsgElement(CDebugInfo *poDebug,char nType,char *data,int nDataSize);
	virtual ~CMsgElement();

	unsigned long nGetUID(void);
	int nGetSize(void);
	unsigned char * pGetData(void);
};


//This is the Main Message Data block class
class CGenNetMsg : public CNetMutex    
{
private:
	static unsigned long g_uID;	//Global uid array
	unsigned long u_msgID;		//local message uid
	long nMsgAge;
	char nCompressed;
	char nEncrypted;
	CStr *poKey;
	
	CDebugInfo *poDebug;
	unsigned char *pDataBuffer;
	DLP_CPtrList *poElementList;
	unsigned long ulGLOBALGetUid(void);
	int nDataSize;
	int nDestroy;
	unsigned long nSrcNodeId;

public:
  
	unsigned long nMagic;
	CGenNetMsg(CDebugInfo *poDebug);
	CGenNetMsg(CDebugInfo *poDebug,unsigned long nSrcNodId);
	virtual ~CGenNetMsg();
	unsigned long nGetUID(void);
	int nGetDataSize(void);
	void vSetDataSize(long nVal);
	void vSetDestroy(int nVal);
	int nGetDestroy(void);

	int nAddToken(unsigned char token);
	int nAddByte(char byte);
	int nAddUByte(unsigned char byte);
	int nAddInt(int n_Val);
	int nAddFloat(float n_Val);
	int nAddUInt(unsigned int n_Val);
	int nAddLong(long n_Val);
	int nAddULong(unsigned long n_Val);
	int nAddString(char *o_string);
	int nAddChunk(char *o_chunk,int nSize);
	int nAddIntArray(int *pArray,int nSize);


	int nParseToken(unsigned char *token);
	int nParseByte(char *byte);
	int nParseUByte(unsigned char *ubyte);
	int nParseInt(int *nint);
	int nParseFloat(float *nfloat);
	int nParseUInt(unsigned int *uint);
	int nParseLong(long *nlong);
	int nParseULong(unsigned long *nulong);
	int nParseString(CStr *poString);
	int nParseIntArray(DLP_CPtrArray *poArray,int *nSize);

	int nIsShutdown(unsigned char ntToken);
	char * nParseChunk(int *nSize);

	int nCreateMessage(void);
	int nCreateMessage(int nCompressed,char *key);
	unsigned char *poGetMessageData(void);
	void vSetMessageData(char *data);
	unsigned char *poGetDataBuffer(void);

	int nParseMsgData(unsigned char *data);
	int nParseMsgData(unsigned char *data, char *key);
	long nGetMsgAge(void);
	
	void vSetSrcNodeId(unsigned long nId);
	unsigned long nGetSrcNodeId(void);
	void vDumpMessage(void);
	
	void vEncryptData(char *data, int nSize);
	void vDeCryptData(char *data, int nSize);


};

class CMessageManager
{
private:
	CDebugInfo *poDebug;
	DLP_CPtrArray *poRecvDataMsgQueue;
	DLP_CPtrArray *poRecvControlMsgQueue;

	DLP_CPtrArray *poSendDataMsgQueue;
	DLP_CPtrArray *poSendControlMsgQueue;

	CNetMutex *poMutex;
	CNetMutex *poReadMutex;
	CNetMutex *poSendMutex;

public:
	CMessageManager(CDebugInfo *poDebug);
	virtual ~CMessageManager();

	int nGetRecvAllMsgCount(void);

	CGenNetMsg *poGetFirstRecvDataMessage(void);
	CGenNetMsg *poGetLastRecvDataMessage(void);
	int nRemoveFirstRecvDataMessage(void);
	int nRemoveLastRecvDataMessage(void);
	int nGetRecvDataMsgCount(void);

	void vAddRecvDataMessage(CGenNetMsg *poMsg);
	int nRemoveRecvDataMessage(CGenNetMsg *poMsg);
	int nRemoveRecvDataMessage(long nUID);

	CGenNetMsg *poGetFirstRecvControlMessage(void);
	CGenNetMsg *poGetLastRecvControlMessage(void);
	int nRemoveFirstRecvControlMessage(void);
	int nRemoveLastRecvControlMessage(void);
	int nGetRecvControlMsgCount(void);

	void vAddRecvControlMessage(CGenNetMsg *poMsg);
	int nRemoveRecvControlMessage(CGenNetMsg *poMsg);
	int nRemoveRecvControlMessage(long nUID);


	int nGetSendAllMsgCount(void);

	CGenNetMsg *poGetFirstSendDataMessage(void);
	CGenNetMsg *poGetLastSendDataMessage(void);
	int nRemoveFirstSendDataMessage(void);
	int nRemoveLastSendDataMessage(void);
	int nGetSendDataMsgCount(void);

	void vAddSendDataMessage(CGenNetMsg *poMsg);
	int nRemoveSendDataMessage(CGenNetMsg *poMsg);
	int nRemoveSendDataMessage(long nUID);

	CGenNetMsg *poGetFirstSendControlMessage(void);
	CGenNetMsg *poGetLastSendControlMessage(void);
	int nRemoveFirstSendControlMessage(void);
	int nRemoveLastSendControlMessage(void);
	int nGetSendControlMsgCount(void);

	void vAddSendControlMessage(CGenNetMsg *poMsg);
	int nRemoveSendControlMessage(CGenNetMsg *poMsg);
	int nRemoveSendControlMessage(long nUID);
};

#endif // !defined(AFX_NETMSG_H__F507694B_EF04_4947_9DE3_EDB2A33E4CE7__INCLUDED_)
