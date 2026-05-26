/*********************************************************************
*	File : ARACrypt.h
*	Desc : Basic Key Encryption
*
*	Comment: 
*
*
*
*	Date: 1/02/2002
*********************************************************************/
#if !defined(AFX_ARACRYPT_H__66035FB9_8319_11D2_85E6_004005FFF9AF__INCLUDED_)
#define AFX_ARACRYPT_H__66035FB9_8319_11D2_85E6_004005FFF9AF__INCLUDED_

// ARACrypt window
class CStr;
class CDebugInfo;

class ARACrypt
{
// Construction
public:
	ARACrypt();

// Attributes
public:

// Operations
public:
	virtual void TransformString(char * csKey, CStr *csTarget);
	virtual void TransformString(char * csKey, CStr *csTarget,int nlen);

	void vCreateKey(CDebugInfo *poDebug,CStr *poKey,int nVal);	
	void vCreateKey(CDebugInfo *poDebug,CStr *poInKey1,CStr *poInKey2,CStr *poOutKey);

	void vCreateHWKey(CDebugInfo *poDebug,CStr *poKey);
	int nVaidateHWKey(CDebugInfo *poDebug,CStr *poKey,int *pVal);


protected:
	virtual void SetKey(char * csKey);
	virtual void GetKey(CStr *csKey);
	virtual void TransformChar(unsigned char *csTarget);

// Implementation
public:
	virtual ~ARACrypt();

// Attributes
protected:

	CStr *m_csKey;

    unsigned long           m_LFSR_A;
    unsigned long           m_LFSR_B;
    unsigned long           m_LFSR_C;
    const unsigned long     m_Mask_A;
    const unsigned long     m_Mask_B;
    const unsigned long     m_Mask_C;
    const unsigned long     m_Rot0_A;
    const unsigned long     m_Rot0_B;
    const unsigned long     m_Rot0_C;
    const unsigned long     m_Rot1_A;
    const unsigned long     m_Rot1_B;
    const unsigned long     m_Rot1_C;

};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ARACRYPT_H__66035FB9_8319_11D2_85E6_004005FFF9AF__INCLUDED_)


