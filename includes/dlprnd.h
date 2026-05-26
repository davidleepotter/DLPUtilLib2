/*********************************************************************
*	File : dlprnd.cpp 
*	Desc :  Random Class
*
*
*
*
*
*	Date: 5/18/2002
*********************************************************************/ 

#if !defined(_DLP_RND_H)
#define _DLP_RND_H

class CDLPRandom
    {
    public:
	static int initialized;

	CDLPRandom(void);
	virtual ~CDLPRandom(void) { }

	static int random32bit(void);
	static int randomInt(int low, int high);
	static void setSeed(unsigned long seed);

    };


#endif


