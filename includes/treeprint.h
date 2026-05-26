/*********************************************************************
*	File : TreePrint.h
*	Desc : Tree Print util
*
*	Comment:
*
*
*
*	Date: 12/12/2003
*********************************************************************/
#if !defined(AFX_TREEPRINT_H__1B6A20D8_6D13_4B1D_89C5_3714F948EC92__INCLUDED_)
#define AFX_TREEPRINT_H__1B6A20D8_6D13_4B1D_89C5_3714F948EC92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDebugInfo;
class CNetTimer;
class DLP_CPtrList;
class CTreeItem;
class CStr;

class CTreePrint  
{
private:
	CDebugInfo *poDebug;

public:
	friend CTreeItem;

	CTreePrint(CDebugInfo *poDebug);
	DLP_CPtrList *poTree;		
	virtual ~CTreePrint();
	void vPrintTree(void);
	void vAddToTree(CTreeItem *poItem);
};

class CTreeItem
{
private:
	CDebugInfo *poDebug;
	DLP_CPtrList *poChildern;			
	CTreeItem *poParent;
	CTreeItem *poTopParent;
	CStr *poName;
	int nCnt;
	int nNum;

public:

	friend CTreePrint;
	CTreeItem(CTreeItem *poTopParent,CDebugInfo *poDebug,CTreeItem *poParent,char *name,int nNum);
	virtual ~CTreeItem();
	int nLevel;

	void vAddChild(CTreeItem *poItem);
	void vPrintBranch(int nCnt,int nLastVal);
	void vAdvanceCurPos(int nPos);
};


#endif // !defined(AFX_TREEPRINT_H__1B6A20D8_6D13_4B1D_89C5_3714F948EC92__INCLUDED_)
