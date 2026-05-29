// stringpool.h: interface for the CStringPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGPOOL_H__8D8CCDA4_D65A_4564_BB4D_B974F692A806__INCLUDED_)
#define AFX_STRINGPOOL_H__8D8CCDA4_D65A_4564_BB4D_B974F692A806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PoolObj.h"

class CDebugInfo;

class CStringPool : public CPoolObj  
{

private:
	virtual void * poAddObject(void);
	CDebugInfo *poDebug;

public:
	virtual int nReleaseObj(void *poObj);
	virtual void vInitPool(int nSize);
	CStringPool(CDebugInfo *poDebug);
	virtual ~CStringPool();
	void vTest(void);
};

#endif // !defined(AFX_STRINGPOOL_H__8D8CCDA4_D65A_4564_BB4D_B974F692A806__INCLUDED_)
