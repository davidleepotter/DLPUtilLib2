/*********************************************************************
*	File : TreePrint.cpp
*	Desc : Tree Print util
*
*	Comment:
*
*
*
*	Date: 12/12/2003
*********************************************************************/
#include "utilheader.h"


//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CTreeItem::CTreeItem(CTreeItem *potTopParent,CDebugInfo *potDebug,CTreeItem *potParent,char *name,int ntNum)
{
	poDebug = potDebug;
	poTopParent = potTopParent;
	poParent = potParent;
	poName = new CStr(name);
	poChildern = new DLP_CPtrList(poDebug);
	nCnt = 0;
	nNum = ntNum;
	nLevel = 0;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CTreeItem::~CTreeItem()
{
	DLP_CPtrListItem *plt = poChildern->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CTreeItem *poItem = (CTreeItem *)plt->pGetPtr();
			delete poItem;
			plt = nextPlt;
		}

	poChildern->vRemoveAll();
	delete poChildern;
	poChildern = 0;

	delete poName;
}


//////////////////////////////////////////////////////
//
//	Add Child
//
//////////////////////////////////////////////////////
void CTreeItem::vAddChild(CTreeItem *poItem)
{
	poChildern->vAddTail(poItem);
}



//[Test]-|
//		 Item1-|


//////////////////////////////////////////////////////
//
//	Adv Cur Pos
//
//////////////////////////////////////////////////////
void CTreeItem::vAdvanceCurPos(int nPos)
{
	//CurPos
	for(int x1=0; x1<nPos; x1++)
	{
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_CLEAN," ");
	}

}


int nBCnt = 0;
//////////////////////////////////////////////////////
//
//	Print Brance
//
//////////////////////////////////////////////////////
void CTreeItem::vPrintBranch(int ntCnt,int nLastVal)
{



	int ntCnt1 = 0;


	//I am top level
	if(poParent == 0)
	{
		poName->Format("SceneGraph");
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_CLEAN,"[%s][%d]\n",poName->GetString(),nNum);
		
	}
	else
	{
		//CurPos
		vAdvanceCurPos((nLevel+1)*5);
		poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_CLEAN,"[%s][%d]\n",poName->GetString(),nNum);

	}



	DLP_CPtrListItem *plt = poChildern->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CTreeItem *poItem = (CTreeItem *)plt->pGetPtr();
			
			poItem->vPrintBranch(ntCnt1,0);

			plt = nextPlt;
		}



}

//////////////////////////////////////////////////////////////////////
// Tree Print
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CTreePrint::CTreePrint(CDebugInfo *potDebug)
{
	poDebug = potDebug;
	poTree = new DLP_CPtrList(poDebug);
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CTreePrint::~CTreePrint()
{
	DLP_CPtrListItem *plt = poTree->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CTreeItem *poItem = (CTreeItem *)plt->pGetPtr();
			delete poItem;
			plt = nextPlt;
		}

	poTree->vRemoveAll();
	delete poTree;
	poTree = 0;
}


//////////////////////////////////////////////////////
//
//	Add Item to Tree
//
//////////////////////////////////////////////////////
void CTreePrint::vAddToTree(CTreeItem *poItem)
{
	poTree->vAddTail(poItem);
}



//////////////////////////////////////////////////////
//
//	Print Tree
//
//////////////////////////////////////////////////////
void CTreePrint::vPrintTree(void)
{
	
	int nCnt = 0;
	DLP_CPtrListItem *plt = poTree->poGetHeadItem();
	while (plt)
		{
			DLP_CPtrListItem *nextPlt = plt->pGetNext();
			CTreeItem *poItem = (CTreeItem *)plt->pGetPtr();
		
			poItem->vPrintBranch(nCnt,0);
			plt = nextPlt;
			nCnt++;
		}


}