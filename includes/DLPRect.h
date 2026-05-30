// DLPRect.h: interface for the CDLPRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLPRECT_H__362B30C3_BB71_4120_A9EE_360545B09987__INCLUDED_)
#define AFX_DLPRECT_H__362B30C3_BB71_4120_A9EE_360545B09987__INCLUDED_

#include <windows.h>

class CDLPSize
{
private:

public:
	CDLPSize();
	CDLPSize(long width, long height);
	CDLPSize(CDLPSize& s);
	CDLPSize(CDLPSize* s);
#ifdef WIN32
	CDLPSize(SIZE &s);
	CDLPSize(SIZE *s);
#endif	
	virtual ~CDLPSize();
	
	long nGetWidth(void);
	long nGetHeight(void);
	void vSet(long width, long height);

	CDLPSize operator=(CDLPSize s);
	CDLPSize operator+(CDLPSize s);
	CDLPSize operator-(CDLPSize s);

	bool operator<(CDLPSize Size);
	bool operator>(CDLPSize Size);

	bool operator==(CDLPSize Size);
	bool operator!=(CDLPSize Size); 
	void operator+=(CDLPSize Size);
	void operator-=(CDLPSize Size);
	void Offset(int WidthOffset, int HeightOffset);
	void Offset(CDLPSize Size);

    long  width;
    long  height;

};

class CDLPPoint
{
private:

public:
	CDLPPoint();
	CDLPPoint(long x, long y);
	CDLPPoint(CDLPPoint& s);
	CDLPPoint(CDLPPoint* s);
#ifdef WIN32
	CDLPPoint(POINT &s);
	CDLPPoint(POINT *s);
#endif	
	virtual ~CDLPPoint();
	
	long nGetX(void);
	long nGetY(void);
	void vSet(long nX, long nY);

	CDLPPoint operator=(CDLPPoint s);
	CDLPPoint operator+(CDLPPoint s);
	CDLPPoint operator-(CDLPPoint s);

	bool operator==(CDLPPoint point);
	bool operator!=(CDLPPoint point); 
	void operator+=(CDLPPoint point);
	void operator-=(CDLPPoint point);
	void Offset(int xOffset, int yOffset);
	void Offset(CDLPPoint point);

    long  x;
    long  y;

};


class CDLPRect  
{
private:
 
public:
	CDLPRect();
	virtual ~CDLPRect();

	CDLPRect(long x, long y,long width, long height);
	CDLPRect(CDLPRect& s);
	CDLPRect(CDLPRect* s);
#ifdef WIN32
	CDLPRect(RECT &s);
	CDLPRect(RECT *s);
	int nIsPtInRect(POINT& s);
	int nIsPtInRect(POINT* s);
#endif
	long nGetX(void);
	long nGetY(void);
	long nGetWidth(void);
	long nGetHeight(void);

	void vSet(long nX, long nY,long nWidth, long nHeight);

	CDLPRect * operator=(CDLPRect& s);
	CDLPRect * operator=(CDLPRect* s);

	int operator==(CDLPRect& s);
	int operator==(CDLPRect* s);
    
	int nIsPtInRect(long x, long y);
	int nIsPtInRect(CDLPPoint& s);
	int nIsPtInRect(CDLPPoint* s);


	long   x;
    long   y;
    long   width;
    long   height;

};

/////////////////////////

class CDLPFloatSize
{
private:

public:
	CDLPFloatSize();
	CDLPFloatSize(float width, float height);
	CDLPFloatSize(CDLPFloatSize& s);
	CDLPFloatSize(CDLPFloatSize* s);

	virtual ~CDLPFloatSize();
	
	float nGetWidth(void);
	float nGetHeight(void);
	void vSet(float width, float height);

	CDLPFloatSize operator=(CDLPFloatSize s);
	CDLPFloatSize operator+(CDLPFloatSize s);
	CDLPFloatSize operator-(CDLPFloatSize s);

	bool operator<(CDLPFloatSize Size);
	bool operator>(CDLPFloatSize Size);

	bool operator==(CDLPFloatSize Size);
	bool operator!=(CDLPFloatSize Size); 
	void operator+=(CDLPFloatSize Size);
	void operator-=(CDLPFloatSize Size);
	void Offset(int WidthOffset, int HeightOffset);
	void Offset(CDLPFloatSize Size);

    float  width;
    float  height;

};

class CDLPFloatPoint
{
private:

public:
	CDLPFloatPoint();
	CDLPFloatPoint(float x, float y);
	CDLPFloatPoint(CDLPFloatPoint& s);
	CDLPFloatPoint(CDLPFloatPoint* s);

	virtual ~CDLPFloatPoint();
	
	float nGetX(void);
	float nGetY(void);
	void vSet(float nX, float nY);

	CDLPFloatPoint operator=(CDLPFloatPoint s);
	CDLPFloatPoint operator+(CDLPFloatPoint s);
	CDLPFloatPoint operator-(CDLPFloatPoint s);

	bool operator==(CDLPFloatPoint point);
	bool operator!=(CDLPFloatPoint point); 
	void operator+=(CDLPFloatPoint point);
	void operator-=(CDLPFloatPoint point);
	void Offset(int xOffset, int yOffset);
	void Offset(CDLPFloatPoint point);

    float  x;
    float  y;

};


class CDLPFloatRect  
{
private:
 
public:
	CDLPFloatRect();
	virtual ~CDLPFloatRect();

	CDLPFloatRect(float x, float y,float width, float height);
	CDLPFloatRect(CDLPFloatRect& s);
	CDLPFloatRect(CDLPFloatRect* s);

	float nGetX(void);
	float nGetY(void);
	float nGetWidth(void);
	float nGetHeight(void);

	void vSet(float nX, float nY,float nWidth, float nHeight);

	CDLPFloatRect * operator=(CDLPFloatRect& s);
	CDLPFloatRect * operator=(CDLPFloatRect* s);

	int operator==(CDLPFloatRect& s);
	int operator==(CDLPFloatRect* s);
    
	int nIsPtInRect(float x, float y);
	int nIsPtInRect(CDLPFloatPoint& s);
	int nIsPtInRect(CDLPFloatPoint* s);


	float   x;
    float   y;
    float   width;
    float   height;

};

#endif // !defined(AFX_DLPRECT_H__362B30C3_BB71_4120_A9EE_360545B09987__INCLUDED_)



