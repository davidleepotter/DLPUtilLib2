/*********************************************************************
*	File : DLP_Color.h 
*	Desc : Color Object
*
*	Comment: 
*
*
*
*	Date: 5/6/2003
*********************************************************************/
#if !defined(AFX_DLP_COLOR_H__DE98C51C_805A_46EC_BA61_B88ED353439D__INCLUDED_)
#define AFX_DLP_COLOR_H__DE98C51C_805A_46EC_BA61_B88ED353439D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDLP_Color  
{
private:
	float r;
	float g;
	float b;

public:
	CDLP_Color(float r,float g,float b);
	virtual ~CDLP_Color();

	void vSet(CDLP_Color *poColor);
	void vSet(float r,float g,float b);
	float fGetRed(void);
	float fGetGreen(void);
	float fGetBlue(void);



};

#endif // !defined(AFX_DLP_COLOR_H__DE98C51C_805A_46EC_BA61_B88ED353439D__INCLUDED_)
