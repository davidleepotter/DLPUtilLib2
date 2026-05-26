/*********************************************************************
*	File : FILETRANSFER.h
*	Desc : FILETRANSFER
*
*	Comment: 
*
*
*
*	Date: 1/12/2004
*********************************************************************/
#if !defined(AFX_FILETRANSFER_H__7B2D8F1F_6A06_4A83_A1AF_B2D8F8771A56__INCLUDED_)
#define AFX_FILETRANSFER_H__7B2D8F1F_6A06_4A83_A1AF_B2D8F8771A56__INCLUDED_

class CStr;
class CFileTransfer;
class DLP_CPtrArray;
class CFileChunk;
class CUIDManager;
class CUniqueID;
class CFileObj;
class CUserObj;
class CSocketNode2;
class CSocketPortal2;
class CGenNetMsg;

enum GLOB_FILE_TOKEN
{
	MSG_GET_FILE_LIST		= 200,
	MSG_GET_FILE_LIST_END	= 201,
	MSG_TRANS_FILE			= 202,
	MSG_TRANS_START			= 203,
	MSG_TRANS_END			= 204,
	MSG_TRANS_PACKET		= 205,
	MSG_BAD_PASS			= 206,
	MSG_HELLO				= 207,
	MSG_CHUNK_CACHE			= 208,
};

class CTransferObj
{
private:
	CUniqueID *poUID;
	CDebugInfo *poDebug;
	CTimerManObj *poTimerMan;
	DLP_CPtrList *poUserList;
	DLP_CPtrList *poFileList;
	int nMaxPPS;

	CSocketPortal2 *poPortal;
	unsigned long nCurPacket;
	int nLocalThreadShutdown;

	unsigned long nLastTransTime;
	long nLastPacketCnt;
	int nCurChunk;

public:
	friend CFileTransfer;

	CTransferObj(
		CDebugInfo *potDebug
		,CUIDManager *potUIDManager
		,CTimerManObj *potTimerMan
		,DLP_CPtrList *poUserList
		,CSocketPortal2	*poPortal
		,unsigned long ntOwnerid
		, unsigned long ntFileid);


	~CTransferObj();
	unsigned long nUid;
	unsigned long nTotalChunks;
	unsigned long nTotalPackets;

	unsigned long nOwnerID;
	unsigned long nFileID;
	float fPerct;
	long nBPS;
	void vSetMaxPPS(int nVal);
	CNetTimer *poTransferTimer;
	static void transferCallBack(unsigned long n_val,void *poVoidPointer1,void *poVoidPointer2,CNetTimer *o_tNetTimer);
	CSocketNode2 *poFindNetNode(void);
	
	CUserObj * poFindUser(unsigned long nUID);
	int nSendNextPacket(CSocketNode2 *poNode);
	void vStartTransfer(void);
	void vStartSendingPackets(void);
	void vEndTransfer(void);
	CFileObj * poFindFile(unsigned long nUID);

	int nStartTransfer(void);

};

class CUserObj
{

private:
	CUniqueID		*poUID;
	DLP_CPtrArray	*poFileArray;
	CDebugInfo		*poDebug;
	CTimerManObj	*poTimerMan;
	CUIDManager		*poUIDManager;

public:
	CUserObj(char *ip,CDebugInfo *potDebug,CTimerManObj *potTimerMan,CUIDManager *poUIDManager,unsigned long ntConID);
	~CUserObj();
	unsigned long nUid;
	CStr *poUserName;
	unsigned long nConID;
	CTransferObj *poCurTransfer;
	CStr			*poIP;

	CFileObj *poAddFile(char *filename);
	CFileObj *poFindFile(char *filename);
	CFileObj *poFindFile(unsigned long nFileID);
};


class CFileObj
{

private:
	CUniqueID *poUID;
	CDebugInfo *poDebug;
	CTimerManObj *poTimerMan;
	unsigned long nLastByteCnt;

public:
	CFileObj(CTimerManObj *potTimerMan,CDebugInfo *potDebug,CUIDManager *poUIDManager,char *name, char *date,char *version,char *password,char *guid);
	~CFileObj();
	int nClientParseMessages(CGenNetMsg *poMsg);

	CFileTransfer *poFileTransfer;
	CStr *poPassword;
	CStr *poFileName;
	CStr *poVersion;
	CStr *poGUID;
	unsigned long nUid;
	CStr *poFilePath;
	CStr *poDate;
	long nSize;
	float fPerctComplete;

	unsigned long nStartTime;
	unsigned long nEndTime;
	unsigned long nLastPacketTime;	
	unsigned long nCurPacketTime;
	long nLastPacketCnt;
	long nCurPacket;
};


class CPacketObject
{
private:
	CFileTransfer *m_poTransMan;
	CDebugInfo *m_poDebug;
	long m_nUID;
	char *m_PacketPtr;
	int m_PacketSize;
	CUniqueID *poUID;
	long nChunkUID;
	int nPointerSet;

public:
	friend CFileTransfer;
	friend CFileChunk;

	CPacketObject(long nChunkUID,CDebugInfo *poDebug,CFileTransfer *poTransMan,int nSize);
	~CPacketObject();

	long nGetChunkID(void);
	int nGetPacketSize();
	char *pGetPtr(void);
	void vDumpStatus(int nLevel);
	void vSetPtr(char *ptr);
};

class CFileChunk
{
private:
	long m_nUID;	
	long m_ChunkSize;
	CDebugInfo *m_poDebug;
	DLP_CPtrArray *m_poPacketArray;
	FILE *m_fp;
	CUniqueID *poUID;
	CFileTransfer *m_poTransMan;
	void vFreeChunkData(void);
	void vCalcTotalPackets(void);
	void vCalcPackets(void);
	void vDestroyPackets(void);

	long m_nStartPacket;
	long m_nEndPacket;

	long m_nMaxPacketSize;
	long m_nMaxBufferSize;
	long m_nCurFilePos;
	long m_nCurPacketPos;
	int m_nFileBufferStatus;
	char *m_pFileBuffer;
	long m_nCurFileBufferSize;

	int m_nCurPacket;
	long m_nTotalPackets;
	long m_nFileSize;
	int nInWrite;

public:
	enum FILE_TRANSFER_BUFFER_STATUS
	{
		BUFFER_NONE,
		BUFFER_NEXT,
		BUFFER_LAST,
	};

	friend CPacketObject;
	friend CFileTransfer;
	
	CPacketObject *poGetFirstPacket(void);
	CPacketObject *poGetNextPacket(void);
	CPacketObject *poGetPacket(int nIndex);
	int nLoadFileChunk(FILE *fp);
	int nWriteFileChunk(FILE *fp);
	int nCreateEmptyChunk(void);
	int nGetIsInWrite(void);

	bool bIsChunkFull(void);
	int nGetSize(void);
	void vDumpStatus(int nLevel);
	long nGetUID();

	int nOverRide;
	int nChunkWritten;
	CFileChunk(CDebugInfo *poDebug
		,CFileTransfer *poTransMan
		,long BuffernSize
		,long nPacketSize
		,int nFileSize);

	~CFileChunk();
};


class CFileTransfer
{
private:
	unsigned long nRemoteFileID;
	CSocketNode2 *poNode;
	CUIDManager *poUIDMan;
	CDebugInfo *m_poDebug;
	long m_nUID;
	long m_nFileSize;
	long m_nTotalChunks;
	long m_nTotalPackets;
	long m_nMaxPacketSize;
	long m_nMaxBufferSize;
	CTimerManObj *poTimerMan;

	CUniqueID *poUID;

	FILE *m_fp;
	CStr *m_poFileName;
	CStr *m_poCacheFileName;


	void vCalcTotalChunks(void);
	void vCleanUp(void);

public:
	friend CPacketObject;
	friend CFileChunk;
	friend CFileObj;
	
	DLP_CPtrArray *m_poChunkArray;
	unsigned long nBytesRecv;
	int nWriteFile(char *filename);
	int nOpenFile(char *filename);
	void vCloseFile(void);
	long nGetTotalPackets(void);
	long nGetTotalChunks(void);
	long nGetMaxPacketSize(void);
	long nGetMaxBufferSize(void);
	long nGetFileSize(void);
	int nIsFileComplete(void);
	int nIsInWrite(void);
	int nIsCached(void);
	void vSendCacheInfo(void);
	float fGetPerct(void);
	int nNumberCachedChunks;

	//Client Perct
	float fPerct;
	unsigned long nCurOffset;
	unsigned long nStartOffset;


	CFileChunk * poGetChunk(long nUID);
	CPacketObject *poGetPacket(long nIndex);
	int nFindPacket(long nIndex);
	long nConvertIndex(long nIndex,CFileChunk *poChunk);
	void vRemoveCacheFile(void);
	int nInWriteThread;
	int nFinished;
	char *poGetFileName();

	CFileTransfer(
		CSocketNode2 *potNode
		,CTimerManObj *poTimerMan
		,CUIDManager *potUIDManger
		,CDebugInfo *poDebug
		,char *tempfilename
		,int chunkwrite
		,long m_tnMaxBufferSize=5000000
		,long m_tnMaxPacketSize=1024);


	CFileTransfer(
		unsigned long nRemoteFileID
		,CSocketNode2 *potNode
		,CTimerManObj *poTimerMan
		,CUIDManager *potUIDManger
		,CDebugInfo *poDebug
		,char *name
		,char *path
		,char *filename
		,char *GUID
		,long nTotalChunks
		,long nTotalpackets
		,long nFileSize
		,long m_tnMaxBufferSize=5000000
		,long m_tnMaxPacketSize=1024);
	
	long nAddPacket(long nPackNumber,char *data);
	void vDumpStatus(int nLevel);
	void CFileTransfer::vCreateCacheFile(char *filename);

	static void * pChunkWriteThread( void * pParam );
	int nStartChunkWriteThread(CFileChunk *poChunk);

	virtual ~CFileTransfer(void);
};

#endif
