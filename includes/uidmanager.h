/*********************************************************************
*	File : UIDManager.h
*	Desc : UIDManager
*
*	Comment: 
*
*
*
*	Date: 11/18/2002
*********************************************************************/
#if !defined(AFX_UIDMANAGER_H__7B2D8F1F_6A06_4A83_A1AF_B2D8F8771A56__INCLUDED_)
#define AFX_UIDMANAGER_H__7B2D8F1F_6A06_4A83_A1AF_B2D8F8771A56__INCLUDED_

class CDebugInfo;
class CNetTimer;
class DLP_CPtrList;
class CStr;
class CUIDManager;
class CUniqueID;


class CUIDGroup
{

private:
	CDebugInfo *poDebug;
	unsigned long uGlobUID;
	unsigned char bGlobUID;
	unsigned char nGlobUID[2];

	int nType;
	DLP_CPtrList *poUIDList;
	CStr *poUIDName;

public:
	friend CUIDManager;

	CUIDGroup(CDebugInfo *potDebug);
	virtual ~CUIDGroup();
	CUIDGroup * poCreateGroup(char *name,int ntType);
	void vResetUID(void);

	CUniqueID *poGetNextUID(void);

};

class CUIDManager  
{


private:
	CDebugInfo *poDebug;
	int nCheckForDupe(char *name);
	
	DLP_CPtrList *poUIDGroupList;
public:
	friend CUIDGroup;

	CUIDManager(CDebugInfo *poDebug);
	virtual ~CUIDManager();

	CUIDGroup * poGetGroup(char *name);
	CUIDGroup * poCreateGroup(char *name,int nType);
	CUniqueID *poGetNextUniqueID(char *name);
	void vRemoveGroup(char *name);
	void vAddToGroup(char *name,CUniqueID *poUID);
	void vResetGroup(char *name);

	unsigned long uCalcDChar(unsigned char UID0,unsigned char UID1);
	void vCalcDChar(unsigned long uVal,unsigned char *UID0,unsigned char *UID1);


};

#endif // !defined(AFX_UIDMANAGER_H__7B2D8F1F_6A06_4A83_A1AF_B2D8F8771A56__INCLUDED_)
