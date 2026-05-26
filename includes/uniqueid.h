// uniqueid.h: interface for the CUniqueID class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIQUEID_H__75DC6089_6DDD_4A68_876D_C069E6E3C6B3__INCLUDED_)
#define AFX_UNIQUEID_H__75DC6089_6DDD_4A68_876D_C069E6E3C6B3__INCLUDED_

class CUIDManager;

class CDebugInfo;

class CUniqueID  
{
private:
	int nType;
	CDebugInfo *poDebug;

	unsigned long uUID;
	unsigned char bUID;
	unsigned char nUID[2];

public:

	friend CUIDManager;

	CUniqueID(int ntType,CDebugInfo *potDebug,unsigned long uUID);
	CUniqueID(int ntType,CDebugInfo *potDebug,unsigned char bUID);
	CUniqueID(int ntType,CDebugInfo *potDebug,unsigned char uUID0,unsigned char uUID1);

	unsigned long uGetUniqueID(void);

	virtual ~CUniqueID();

	unsigned char nGetByteVal(int nByteType);
};

#endif // !defined(AFX_UNIQUEID_H__75DC6089_6DDD_4A68_876D_C069E6E3C6B3__INCLUDED_)
