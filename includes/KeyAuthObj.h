/*********************************************************************
*	File :	KeyAuthObj.h
*	Desc :	Auth Key Header
*			
*
*
*
*
*	Date: 9/13/2004
*********************************************************************/
#if !defined(AFX_KEYAUTHOBJ_H__74326F8E_774D_4BA6_A061_5A23B7152CCE__INCLUDED_)
#define AFX_KEYAUTHOBJ_H__74326F8E_774D_4BA6_A061_5A23B7152CCE__INCLUDED_

class CDebugInfo;
class CStr;

class CKeyAuthObj  
{
private:

	CDebugInfo *poDebug;

	void vCreateKey(void);
	void vCreateKey(CDebugInfo *poDebug,CStr *poKey,int nVal);	
	void vCreateKey(CDebugInfo *poDebug,CStr *poInKey1,CStr *poInKey2,CStr *poOutKey);

	void vCreateHWKey(CDebugInfo *poDebug,CStr *poKey);
	int nVaidateHWKey(CDebugInfo *poDebug,CStr *poKey,int *pVal);
		

public:
	CKeyAuthObj(CDebugInfo *poDebug);
	virtual ~CKeyAuthObj();

};

#endif // !defined(AFX_KEYAUTHOBJ_H__74326F8E_774D_4BA6_A061_5A23B7152CCE__INCLUDED_)
