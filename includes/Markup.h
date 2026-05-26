#if !defined(AFX_MARKUP_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_)
#define AFX_MARKUP_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MARKUP_SETDEBUGSTATE

class CStr;
class DLP_CPtrArray;

class CMarkup  
{
public:
	CMarkup();
	CMarkup( char * szDoc );
	CMarkup( const CMarkup& markup );
	void operator=( const CMarkup& markup );
	virtual ~CMarkup();

	// Create
	CStr * GetDoc() const { return m_csDoc; };
	bool AddElem( char * szName, char * szData=NULL ) { return x_AddElem(szName,szData,false,false); };
	bool AddChildElem( char * szName, char * szData=NULL ) { return x_AddElem(szName,szData,false,true); };
	bool AddAttrib( char * szAttrib, char * szValue ) { return x_SetAttrib(m_iPos,szAttrib,szValue); };
	bool AddChildAttrib( char * szAttrib, char * szValue ) { return x_SetAttrib(m_iPosChild,szAttrib,szValue); };
	bool SetAttrib( char * szAttrib, char * szValue ) { return x_SetAttrib(m_iPos,szAttrib,szValue); };
	bool SetChildAttrib( char * szAttrib, char * szValue ) { return x_SetAttrib(m_iPosChild,szAttrib,szValue); };

	// Navigate
	bool SetDoc( char * szDoc );
	bool IsWellFormed();
	bool FindElem( char * szName=NULL );
	bool FindChildElem( char * szName=NULL );
	bool IntoElem();
	bool OutOfElem();
	void ResetChildPos() { x_SetPos(m_iPosParent,m_iPos,0); };
	void ResetMainPos() { x_SetPos(m_iPosParent,0,0); };
	void ResetPos() { x_SetPos(0,0,0); };
	CStr * GetTagName() const;
	CStr * GetChildTagName() const { return x_GetTagName(m_iPosChild); };
	CStr * GetData() const { return x_GetData(m_iPos); };
	CStr * GetChildData() const { return x_GetData(m_iPosChild); };
	CStr * GetAttrib( char * szAttrib ) const { return x_GetAttrib(m_iPos,szAttrib); };
	CStr * GetChildAttrib( char * szAttrib ) const { return x_GetAttrib(m_iPosChild,szAttrib); };
	CStr * GetError() const { return m_csError; };

	enum MarkupNodeType
	{
		MNT_ELEMENT					= 1,  // 0x01
		MNT_TEXT					= 2,  // 0x02
		MNT_WHITESPACE				= 4,  // 0x04
		MNT_CDATA_SECTION			= 8,  // 0x08
		MNT_PROCESSING_INSTRUCTION	= 16, // 0x10
		MNT_COMMENT					= 32, // 0x20
		MNT_DOCUMENT_TYPE			= 64, // 0x40
		MNT_EXCLUDE_WHITESPACE		= 123,// 0x7b
	};

protected:

#ifdef _DEBUG
	char * m_pMainDS;
	char * m_pChildDS;
#endif

	CStr *m_csDoc;
	CStr *m_csError;

	struct ElemPos
	{
		ElemPos() { Clear(); };
		ElemPos( const ElemPos& pos ) { *this = pos; };
		bool IsEmptyElement() const { return (nStartR == nEndL + 1); };
		void Clear()
		{
			nStartL=0; nStartR=0; nEndL=0; nEndR=0; nReserved=0;
			iElemParent=0; iElemChild=0; iElemNext=0;
		};
		void AdjustStart( int n ) { nStartL+=n; nStartR+=n; };
		void AdjustEnd( int n ) { nEndL+=n; nEndR+=n; };
		int nStartL;
		int nStartR;
		int nEndL;
		int nEndR;
		int nReserved;
		int iElemParent;
		int iElemChild;
		int iElemNext;
	};

	//CArray< ElemPos, ElemPos& > m_aPos;
	DLP_CPtrArray *m_aPos;
	int m_iPosParent;
	int m_iPos;
	int m_iPosChild;
	int m_iPosFree;
	int m_nNodeType;

	struct TokenPos
	{
		TokenPos( char * sz ) { Clear(); szDoc = sz; };
		bool IsValid() const { return (nL <= nR); };
		void Clear() { nL=0; nR=-1; nNext=0; bIsString=false; };
		bool Match( char * szName )
		{
			int nLen = nR - nL + 1;
			return ( strncmp( &szDoc[nL], szName, nLen ) == 0)
				&& ( szName[nLen] == '\0' || strchr(" =/[",szName[nLen]));
		};
		int nL;
		int nR;
		int nNext;
		char * szDoc;
		bool bIsString;
	};

	void x_SetPos( int iPosParent, int iPos, int iPosChild )
	{
		m_iPosParent = iPosParent;
		m_iPos = iPos;
		m_iPosChild = iPosChild;
		m_nNodeType = iPos?MNT_ELEMENT:0;
		MARKUP_SETDEBUGSTATE;
	};

	int x_GetFreePos();
	int x_ReleasePos();
	int x_ParseElem( int iPos );
	int x_ParseError( char * szError, char * szName = NULL );
	static bool x_FindChar( char * szDoc, int& nChar, char c );
	static bool x_FindAny( char * szDoc, int& nChar );
	static bool x_FindToken( TokenPos& token );
	CStr * x_GetToken( const TokenPos& token ) const;
	int x_FindElem( int iPosParent, int iPos, char * szPath );
	CStr * x_GetTagName( int iPos ) const;
	CStr * x_GetData( int iPos ) const;
	CStr * x_GetAttrib( int iPos, char * szAttrib ) const;
	bool x_AddElem( char * szName, char * szValue, bool bInsert, bool bAddChild );
	bool x_FindAttrib( TokenPos& token, char * szAttrib=NULL ) const;
	bool x_SetAttrib( int iPos, char * szAttrib, char * szValue );
	void x_LocateNew( int iPosParent, int& iPosRel, int& nOffset, int nLength, int nFlags );
	int x_ParseNode( TokenPos& token );
	void x_DocChange( int nLeft, int nReplace, const CStr& csInsert );
	void x_Adjust( int iPos, int nShift, bool bAfterPos = false );
	CStr * x_TextToDoc( char * szText, bool bAttrib = false ) const;
	CStr * x_TextFromDoc( int nLeft, int nRight ) const;
};

#endif // !defined(AFX_MARKUP_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_)
