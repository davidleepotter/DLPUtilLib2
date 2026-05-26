/*********************************************************************
*	File : NewCol.cpp 
*	Desc : Collection Class - Array, List, Hash Table
*
*	Comment:
*
*
*
*	Date: 11/01/2001
*********************************************************************/ 
#ifndef _DLP_NEWCOL_H
#define _DLP_NEWCOL_H

class CNetMutex;
class CStr;
class CDebugInfo;

//////////////////////////////////////////////////////////////////////////////////////
//	Ptr List Item
//////////////////////////////////////////////////////////////////////////////////////
class DLP_CPtrListItem
{
private:     
    void				*pPtr;
    DLP_CPtrListItem    *pNext;
    DLP_CPtrListItem    *pPrev;
     
    // Only the friend class can delete these objects.
    virtual ~DLP_CPtrListItem(void);
    
    //	DLP_CPtrListItem(const &DLP_CPtrListItem);	// Disallow copy constructor.
public:
    friend class DLP_CPtrList;

    static int nObjectCount;
    
    DLP_CPtrListItem(void);
    DLP_CPtrListItem *pGetNext(void);
    DLP_CPtrListItem *pGetPrev(void); 
    void *pGetPtr(void); 
    void vSetNext(DLP_CPtrListItem *item);
    void vSetPrev(DLP_CPtrListItem *item);
    void vSetPtr(void *p);
};

// abstract iteration position
typedef DLP_CPtrListItem *pPtrPosition;

//////////////////////////////////////////////////////////////////////////////////////
//	Ptr List
//////////////////////////////////////////////////////////////////////////////////////
class DLP_CPtrList
{
private:	

    CNetMutex				*poMutex;
    int					nCount;
    DLP_CPtrListItem		*poHead;
    DLP_CPtrListItem		*poTail;
 	static DLP_CPtrListItem	*pFreeItems;
   
	void vAddItemHead(DLP_CPtrListItem *item);
    void vAddItemTail(DLP_CPtrListItem *item);
    int nRemoveInternalItem(DLP_CPtrListItem *item);
  
    //	DLP_CPtrList(const &DLP_CPtrList);    // Disallow copy constructor...
    DLP_CPtrList & operator = ( const DLP_CPtrList &);

public:
    
    static int nObjectCount;
   	int	nLocked;
   
    DLP_CPtrList(CDebugInfo *poDebug);
    ~DLP_CPtrList(void);
    
    DLP_CPtrListItem *poGetHeadItem(void);
    DLP_CPtrListItem *poGetTailItem(void);
    void *pGetHead(void);
    void *pGetTail(void);
  
    void vAddHead(void *p);
    void vAddTail(void *p);
    void vRemoveAll(void);
    void *pRemoveHead(void);
    void *pRemoveTail(void);
    int nIsEmpty(void) const;
    int nGetCount(void) const;
	void *pFindIdentical(void *p);

    int nRemoveListItem(DLP_CPtrListItem *item);
	void vRemoveItem(void *p);
  
};

/////////////////////////////////////////////////////////////////////////////////////
//	Ptr Array
//////////////////////////////////////////////////////////////////////////////////////
class DLP_CPtrArray
{
private:
    static int nObjectCount;
	CNetMutex *poMutex;
	
public:   

    // Construction
    DLP_CPtrArray();
    ~DLP_CPtrArray();

    // Attributes
    int nGetSize() const;

    int nGetUpperBound() const;
	void vRandomize(void);

    // Operations
    // Clean up
    void vFreeExtra();
    void vRemoveAll();
    
    // Accessing elements
    void *pGetAt(int nIndex) const;
    void vSetAt(int nIndex, void *newElement);
    void *&pElementAt(int nIndex);
    void vInsertAt(int nIndex, void *newElement, int nCount = 1);
	void vSetSize(int nNewSize, int nGrowBy = - 1);

    // Direct Access to the element data (may return NULL)
    void **pGetData();
	void *pGetDataAddress();
    
    // Potentially growing the array
    void vSetAtGrow(int nIndex, void *newElement);
    int nAdd(void *newElement);
    int nAppend(const DLP_CPtrArray &src);
    void vCopy(const DLP_CPtrArray &src);
    
    // overloaded operator helpers
    void * operator []( int nIndex) const;
    void *& operator []( int nIndex);
    
    // Operations that move elements around
    void vRemoveAt(int nIndex, int nCount = 1);
    
    // Implementation
protected:
    
    void     **m_pData;      // the actual array of data
    int      m_nSize;        // # of elements (upperBound - 1)
    int      m_nMaxSize;     // max allocated
    int      m_nGrowBy;      // grow amount
    
    //void AssertValid() const;
protected:
    
    // local typedefs for class templates
    typedef void *BASE_TYPE;
    typedef void *BASE_ARG_TYPE;
};

//////////////////////////////////////////////////////////////////////////////////////
//	Hash Item
//////////////////////////////////////////////////////////////////////////////////////
class DLP_CHashItem
{
private:

public:
		DLP_CHashItem();
		~DLP_CHashItem();

		long key;
		void	*data;
		CStr	*poString;
		int		nIsString;

		int compare(DLP_CPtrListItem *item) const;
};

//////////////////////////////////////////////////////////////////////////////////////
//	Hash Table
//////////////////////////////////////////////////////////////////////////////////////
class DLP_CHashTable
{
private:
   	CNetMutex *poMutex;
	int		nColCnt;
	int		size;
    DLP_CPtrArray	*poBuckets[3];     // Array of lists containing DLP_CHashItems.
     
    DLP_CHashItem * poGetHashItem(unsigned long key) const;
    DLP_CHashItem * poGetHashItem(char *str) const;  
    int nHashFunc(unsigned long key) const;
    int nHashFunc(char *str) const; 
    static int	nObjectCount;

public:
    DLP_CHashTable(unsigned long _size);
    ~DLP_CHashTable(void);

    void *vHashLookup(unsigned long key) const;
	void *vHashLookup(char *str) const;
    void *poGetHashItemByIdx(int nArrayNum,int nIdx) const;  

    int nHashInsert(unsigned long key, void *data);
    int nHashInsert(char *str, void *data);
	
	void *poHashDelete(char *str);
	void *poHashDelete(unsigned long key);
    
	void *poHashDeleteIdx(int nIdx);
    void vHashDeleteAll(void);
    
	int nGetCount(int nArrayNum);
 	int nGetHashCount(int nArrayNum);
	int nGetCollisionCount(void);
};

//////////////////////////////////////////////////////////////////////////////////////
//	List Access
//////////////////////////////////////////////////////////////////////////////////////
class DLP_CListAccess
{
private:
	
	CNetMutex *poMutex;
	DLP_CPtrListItem *GlobPlt[10];
	DLP_CPtrListItem *GlobNextPlt[10];
	DLP_CPtrList *poList;
	CDebugInfo *poDebug;


public:
	DLP_CListAccess(DLP_CPtrList *potList,CDebugInfo *potDebug);
	~DLP_CListAccess();

	void * poGetFirstItem(int nQueryNum);
	void * poGetNextItem(int nQueryNum);
	void * poGetLastItem(int nQueryNum);
	int nGetCount(void);
};
#endif