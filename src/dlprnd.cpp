
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dlprnd.h"
#include "timermanobj.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifndef WIN32
#include  <unistd.h>
#include  <sys/types.h>
#include  <errno.h>
#include  <sys/ioctl.h>
#endif

static unsigned long IL_R250RandomIndex1 = 0;
static unsigned long IL_R250RandomIndex2 = 103;   
static unsigned long IL_R250Table[250] = { 
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
static unsigned char IL_R250IncrementTable[] =
    {
      1,  2,  3,  4,  5,  6,  7,  8,  9, 10  ,
     11, 12, 13, 14, 15, 16, 17, 18, 19, 20  ,
     21, 22, 23, 24, 25, 26, 27, 28, 29, 30  ,
     31, 32, 33, 34, 35, 36, 37, 38, 39, 40  ,
     41, 42, 43, 44, 45, 46, 47, 48, 49, 50  ,
     51, 52, 53, 54, 55, 56, 57, 58, 59, 60  ,
     61, 62, 63, 64, 65, 66, 67, 68, 69, 70  ,
     71, 72, 73, 74, 75, 76, 77, 78, 79, 80  ,
     81, 82, 83, 84, 85, 86, 87, 88, 89, 90  ,
     91, 92, 93, 94, 95, 96, 97, 98, 99,100  ,
    101,102,103,104,105,106,107,108,109,110  ,
    111,112,113,114,115,116,117,118,119,120  ,
    121,122,123,124,125,126,127,128,129,130  ,
    131,132,133,134,135,136,137,138,139,140  ,
    141,142,143,144,145,146,147,148,149,150  ,
    151,152,153,154,155,156,157,158,159,160  ,
    161,162,163,164,165,166,167,168,169,170  ,
    171,172,173,174,175,176,177,178,179,180  ,
    181,182,183,184,185,186,187,188,189,190  ,
    191,192,193,194,195,196,197,198,199,200  ,
    201,202,203,204,205,206,207,208,209,210  ,
    211,212,213,214,215,216,217,218,219,220  ,
    221,222,223,224,225,226,227,228,229,230  ,
    231,232,233,234,235,236,237,238,239,240,
    241,242,243,244,245,246,247,248,249,  0
    };


static void randomSeed(void)
    {
    int n;
    unsigned long msk, bit;

    IL_R250RandomIndex1 = 0;
    IL_R250RandomIndex2 = 103;
    for (n = 250 - 1; n >= 0; n--)
	{
	// rand() returns a number from 0 to 0x7FFF
	unsigned int lowWord = rand();
	unsigned int highWord = rand();
	unsigned int xWord = rand();
        IL_R250Table[n] = (highWord<<17) | (lowWord<<2) | (xWord & 3);
	}
    for (n = 3, msk = 0xffffffff, bit = 0x80000000; bit; n += 7) 
	{
        IL_R250Table[n] = (IL_R250Table[n] & msk) | bit;
        msk >>= 1;
        bit >>= 1;
	}
    }


int CDLPRandom::initialized = 0;

CDLPRandom::CDLPRandom(void)
    {
    if (!initialized)
	{
	initialized = 1;
	srand( (unsigned)time( 0 ) );
	randomSeed();
	}
    }

void CDLPRandom::setSeed(unsigned long seed)
    {
    srand(seed);
    randomSeed();
    return;
    }

int CDLPRandom::randomInt(int low, int high)
    {
    // Return a random number between low and high inclusive.
	// Fix code so order of 'low' and 'high' does not matter

	unsigned int i = 0;
    if (low == high)
	{
		return low;
	}

    i = (unsigned int) rand();
	
		if (low < high)
		{
			i %= high - low + 1;
			int nVal =  low + (int)i;
			return nVal;
		}
		else
		{
			i %= low - high + 1;
			int nVal = high + (int)i;
			return nVal;
		}
    }

static CDLPRandom R;

int CDLPRandom::random32bit(void)
{
    int i1 = 0;
	int i2 = 0;

    IL_R250RandomIndex1 = IL_R250IncrementTable[i1 = IL_R250RandomIndex1];
    IL_R250RandomIndex2 = IL_R250IncrementTable[i2 = IL_R250RandomIndex2];
    return (IL_R250Table[i1] ^= IL_R250Table[i2]);
}
