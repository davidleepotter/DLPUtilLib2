/*********************************************************************
*	File : CStr.cpp 
*	Desc : CString Class
*
*
*
*
*
*	Date: 8/01/2001
*********************************************************************/ 

#include <stdio.h>
#define TEXT(x) x

#ifndef WIN32
	#include <memory.h>
	#include <ctype.h>
	#include <sys/io.h>
	#include <string.h>
#endif


#include <fcntl.h>
#ifdef WIN32
#include <io.h>
#endif

#include <time.h>
#include "cstr.h"
#include "NewCol.h"
#include "netmutex.h"
#include "utildefines.h"
#include "dlprnd.h"
#include "timermanobj.h"
#include "FileInfo.h"


unsigned int CStr::nGlobalCnt = 0;


/*********************************************************************
* Round up to next value divisible by 4, minus one.  
* Usually compiles to or bx,3  
*********************************************************************/ 
void up_alloc(CPOS &value)
{
    value = CPOS(value | 3);
}

/*********************************************************************
* Proc:		CStr::NewFromString
* Purpose:	Core code for 'copy char* constructor' and '= operator'.
*			Assumes our 'data' field is garbage on entry.
*********************************************************************/ 
void CStr::NewFromString(const char *s, CPOS slen, CPOS prealloc)
{
    // Determine actual size of buffer that needs to be allocated
    if (slen > prealloc)
    {
        prealloc = slen;
    }
    
    // Empty string?
    if (slen == 0 && prealloc == 0)
    {
        Empty();
        return;
    }
    
    // Round up to blkalloc() granularity
    up_alloc(prealloc);
    
    if (data->m_Alloc < prealloc)
    {
        if (data->m_Text != 0)
        {
            delete data;
            data = new SDesc(prealloc + 1);
            data->m_Length = slen;
        }
    }
    else
    {
        memset(data->m_Text, 0, data->m_Alloc);
    }

	data->m_Length = slen;
    memcpy(data->m_Text, s, slen);
}

/*********************************************************************
* Proc:		CStr::operator = const char*
* Purpose:	Copies a string into our internal buffer, if it is big
*			enough and is used only by us; otherwise, blkfrees the
*			current instance and allocates a new one.
*********************************************************************/ 
void CStr:: operator = (const char *s)
{
    // Check for zero length string.
    UINT     slen = strlen(s);
    
    if (slen == 0)
    {
        delete data;
        data = new SDesc(1);
        return;
    }
    
    // Can we handle this without reallocations?  NOTE: we do
    //   not use Buffer() here because if the string needs to
    //   be expanded, the old one will be copied, and we don't
    //   care about it anyway.
    if (data->m_Alloc >= slen)
    {
        // Yes, copy bytes and get out
        memset(data->m_Text, 0, data->m_Alloc);
        memcpy(data->m_Text, s, slen);
        data->m_Length = (CPOS)slen;
        return;
    }
    NewFromString(s, (CPOS)slen, 0);
}

/*********************************************************************
* Proc:		CStr::operator = const char*
* Purpose:	Copies a string into our internal buffer, if it is big
*			enough and is used only by us; otherwise, blkfrees the
*			current instance and allocates a new one.
*********************************************************************/ 
void CStr:: operator = (char *s)
{
    // Check for zero length string.
    UINT     slen = strlen(s);
    
    if (slen == 0)
    {
        delete data;
        data = new SDesc(1);
        return;
    }
    
    // Can we handle this without reallocations?  NOTE: we do
    //   not use Buffer() here because if the string needs to
    //   be expanded, the old one will be copied, and we don't
    //   care about it anyway.
    if (data->m_Alloc >= slen)
    {
        // Yes, copy bytes and get out
        memset(data->m_Text, 0, data->m_Alloc);
        memcpy(data->m_Text, s, slen);
        data->m_Length = (CPOS)slen;
        return;
    }
    NewFromString(s, (CPOS)slen, 0);
}


/*********************************************************************
* Proc:		CStr::operator = CStr
* Purpose:	Copies a string into another string, destroying the
*			previous content.
*********************************************************************/ 
void CStr:: operator = (const CStr &source)
{
	const char *s = source.data->m_Text;

    // Check for zero length string.
    UINT     slen = strlen(s);
    
    if (slen == 0)
    {
        delete data;
        data = new SDesc(1);
        return;
    }
    
    // Can we handle this without reallocations?  NOTE: we do
    //   not use Buffer() here because if the string needs to
    //   be expanded, the old one will be copied, and we don't
    //   care about it anyway.
    if (data->m_Alloc >= slen)
    {
        // Yes, copy bytes and get out
        memset(data->m_Text, 0, data->m_Alloc);
        memcpy(data->m_Text, s, slen);
        data->m_Length = (CPOS)slen;
        return;
    }
    NewFromString(s, (CPOS)slen, 0);

}

/*********************************************************************
* Proc:		CStr::operator = CStr
* Purpose:	Copies a string into another string, destroying the
*			previous content.
*********************************************************************/ 
void CStr:: operator = (const CStr *source)
{
	const char *s = source->data->m_Text;

    // Check for zero length string.
    UINT     slen = strlen(s);
    
    if (slen == 0)
    {
        delete data;
        data = new SDesc(1);
        return;
    }
    
    // Can we handle this without reallocations?  NOTE: we do
    //   not use Buffer() here because if the string needs to
    //   be expanded, the old one will be copied, and we don't
    //   care about it anyway.
    if (data->m_Alloc >= slen)
    {
        // Yes, copy bytes and get out
        memset(data->m_Text, 0, data->m_Alloc);
        memcpy(data->m_Text, s, slen);
        data->m_Length = (CPOS)slen;
        return;
    }
    NewFromString(s, (CPOS)slen, 0);

}

/*********************************************************************
* Proc:		CStr::Buffer
* Purpose:	Helper.  Makes sure that our internal buffer has
*			the specified number of bytes available, and that
*			we can overwrite it (i.e. m_Usage is 1).  If this
*			isn't so, prepares a copy of our internal data.
*********************************************************************/ 
void  CStr::Buffer(CPOS newlength)
{
    up_alloc(newlength);
    
    if (data->m_Alloc >= newlength)
    {
        return;
    }
    
    SDesc     *prevdata = data;
    
    if (newlength < prevdata->m_Length)
    {
        newlength = prevdata->m_Length;
    }
    
    // Determine actual size of buffer that needs to be allocated
    if (prevdata->m_Length > newlength)
    {
        newlength = prevdata->m_Length;
    }
    
    // Empty string?
    if (prevdata->m_Length == 0 && newlength == 0)
    {
        data = new SDesc(1);
        delete prevdata;
        return;
    }
    
    // Round up to blkalloc() granularity
    up_alloc(newlength);
    
    if (data->m_Alloc < newlength)
    {
        if (data->m_Text != 0)
        {
            data = new SDesc(newlength + 1);
        }
    }
    else
    {
        memset(data->m_Text, 0, data->m_Alloc);
    }
    data->m_Length = prevdata->m_Length;
    memcpy(data->m_Text, prevdata->m_Text, prevdata->m_Length);
    delete prevdata;
}




/*********************************************************************
* Proc:		CStr::Empty
* Purpose:	Sets the string to NULL.  However, the allocated buffer
*			is not released.
*********************************************************************/ 
void CStr::Empty()
{
    // Already empty, and with buffer zero?
    if (data->m_Length == 0)
    {
        return;
    }
    memset(data->m_Text, 0, data->m_Alloc);
    data->m_Length = 0;
}


/*********************************************************************
* Proc:		CStr::Reset
* Purpose:	Sets the string to NULL, deallocates buffer
*********************************************************************/ 
void CStr::Reset()
{
    delete data;
    data = new SDesc(1);
}


/*********************************************************************
* Proc:		CStr::AddChar
* Purpose:	Appends a single character to the end of the string
*********************************************************************/ 
void CStr::AddChar(char ch)
{
    UINT     cur_len = data->m_Length;
    
    Buffer(CPOS(cur_len + 1));
    
    // And append the character (no need for cache_arb because of Buffer)
    data->m_Text[cur_len] = ch;
    data->m_Text[cur_len + 1] = 0;
    data->m_Length = CPOS(cur_len + 1);
}


/*********************************************************************
* Proc:		CStr::AddInt
* Purpose:	Appends a decimal signed integer, possibly with - sign
*********************************************************************/ 
void CStr::AddInt(int value)
{
    char     buf[32];

#ifdef WIN32	
    itoa(value, buf, 10);
#endif

#ifndef WIN32
    sprintf(buf, "%d", value);
#endif	
    
    AddString(buf);
}


/*********************************************************************
* Proc:		CStr::AddDouble
* Purpose:	Appends a signed double value, uses specified # of digits
*********************************************************************/ 
void CStr::AddDouble(double value, UINT after_dot)
{
    if (after_dot > 48)
    {
        after_dot = 48;
    }
    
    char     fmt[16];
    
    sprintf(fmt, "%%.%df", (int)after_dot);
    
    char     buf[256];
    
    sprintf(buf, fmt, value);
    AddString(buf);
}


/*********************************************************************
* Proc:		CStr::CoreAddChars
* Purpose:	Core code for AddChars() and operators +=; assumes
*			howmany is bigger than 0
*********************************************************************/ 
int  CStr::InsertChar(const char *s, CPOS nPos)
{
   int nVal = (GetLength() - nPos);

   if(nVal <= 0)
   {
		return -1;
   }

   // Prepare big enough buffer
   Buffer(CPOS(data->m_Length + 1));
 
   char     *src = &data->m_Text[nPos];
   char     *dest = &data->m_Text[nPos+1];
   memcpy(dest,src,nVal);
   
   memcpy(dest, s, 1);
   data->m_Length = CPOS(data->m_Length + 1);

   return 1;
}

/*********************************************************************
* Proc:		CStr::CoreAddChars
* Purpose:	Core code for AddChars() and operators +=; assumes
*			howmany is bigger than 0
*********************************************************************/ 
int  CStr::InsertChars(const char *s, CPOS nPos, int nLen)
{
   //Insert Position
   int nVal = (GetLength() - nPos);
   if(nVal < 0)
   {
		return -1;
   }

   // Prepare big enough buffer
   Buffer(CPOS(data->m_Length + nLen));
 
   char     *src = &data->m_Text[nPos];
   char     *dest = &data->m_Text[nPos+nLen];
   
   if(nVal > 0)
   {
	   // memcpy will not work here, you must move chars from right to left
	   for(int i=nVal-1; i>=0; i--)
	   {
		   dest[i] = src[i];
	   }
   }
	
	memcpy(src, s, nLen);

   data->m_Length = CPOS(data->m_Length + nLen);	
   
   
   int nVal1 = (data->m_Alloc - data->m_Length);
   if(nVal1 > 0)
   {
	memset(&data->m_Text[GetLength()],0,nVal1);
   }

   return 1;
}

/*********************************************************************
* Proc:		CStr::CoreAddChars
* Purpose:	Core code for AddChars() and operators +=; assumes
*			howmany is bigger than 0
*********************************************************************/ 
int  CStr::EraseChar(CPOS nPos)
{
   int nVal = (GetLength() - nPos);

   if(nVal <= 0)
   {
		return -1;
   }

   char     *src = &data->m_Text[nPos+1];
   char     *dest = &data->m_Text[nPos];
   memmove(dest,src,nVal);  
   data->m_Length = CPOS(data->m_Length - 1);

   return 1;
}


/*********************************************************************
* Proc:		CStr::CoreAddChars
* Purpose:	Core code for AddChars() and operators +=; assumes
*			howmany is bigger than 0
*********************************************************************/ 
void  CStr::CoreAddChars(const char *s, CPOS howmany)
{
    // Prepare big enough buffer
    Buffer(CPOS(data->m_Length + howmany));
    
    // And copy the bytes
    char     *dest = data->m_Text + data->m_Length;
    
    memcpy(dest, s, howmany);
    dest[howmany] = 0;
    data->m_Length = CPOS(data->m_Length + howmany);
}


/*********************************************************************
* Proc:		CStr::operator += (both from const char* and from CStr)
* Purpose:	Append a string to what we already contain.
*********************************************************************/ 
void CStr:: operator += (char nChar)
{
	this->AddChar(nChar);
}


/*********************************************************************
* Proc:		CStr::operator += (both from const char* and from CStr)
* Purpose:	Append a string to what we already contain.
*********************************************************************/ 
void CStr:: operator += (char *nChar)
{
	UINT     slen = strlen(nChar);
    
    if (slen != 0)
    {
        CoreAddChars(nChar, (CPOS)slen);
    }
}

/*********************************************************************
* Proc:		CStr::operator += (both from const char* and from CStr)
* Purpose:	Append a string to what we already contain.
*********************************************************************/ 
void CStr:: operator += (const CStr &obj)
{
    if (obj.data->m_Length != 0)
    {
        CoreAddChars(obj.data->m_Text, obj.data->m_Length);
    }
}

/*********************************************************************
* Proc:		CStr::operator += (both from const char* and from CStr)
* Purpose:	Append a string to what we already contain.
*********************************************************************/ 
void CStr:: operator += (const char *s)
{
    UINT     slen = strlen(s);
    
    if (slen != 0)
    {
        CoreAddChars(s, (CPOS)slen);
    }
}


/*********************************************************************
* Proc:		CStr::AddChars
* Purpose:	Catenates a number of characters to our internal data.
*********************************************************************/ 
void CStr::AddChars(const char *s, CPOS startat, CPOS howmany)
{
    if (howmany != 0)
    {
        CoreAddChars(s + startat, howmany);
    }
}

/*********************************************************************
* Proc:		CStr::AddStringAtLeft
* Purpose:	Prepend a string before us
*********************************************************************/ 
void CStr::AddStringAtLeft(const char *s)
{
    UINT     slen = strlen(s);
    
    if (slen == 0)
    {
        return;
    }
    Buffer(CPOS(GetLength()+slen));
    
    // Move existing data -- do NOT use memcpy!!
    memmove(data->m_Text + slen, data->m_Text, GetLength()+1);
    
    // And copy bytes to be prepended
    memcpy(data->m_Text, s, slen);
    data->m_Length = CPOS(data->m_Length + slen);
}


/*********************************************************************
* Proc:		CStr::GetLastChar
*********************************************************************/ 
char CStr::GetLastChar() const
{
    UINT     l = GetLength();
    
    if (l < 1)
    {
        ThrowBadIndex(l, GetLength());
    }
    return data->m_Text[l - 1];
}

/*********************************************************************
* Proc:		CStr::GetLeft
*********************************************************************/
void CStr::GetMiddleStartEnd(CPOS start, CPOS end, CStr *result)
{
	int nLen = GetLength();

	int chars = end - start;

    result->Empty();

    // Nothing to return?
    CPOS     l = GetLength();

    if (l == 0 || ((start) + chars) == 0)
    {
        return;
    }

    // Do not return data beyond the end of the string
    if (start >= l)
    {
        return;
    }

    if (((start) + chars) >= l)
    {
        chars = CPOS(l - start);
    }

	if(chars > 0)
	{

		// Copy bytes
		result->CoreAddChars(GetString()+start, chars);
	}
}


/*********************************************************************
* Proc:		CStr::GetLeft
*********************************************************************/ 
void CStr::GetMiddle(CPOS start, CPOS chars, CStr *result)
{
    result->Empty();
    
    // Nothing to return?
    CPOS     l = GetLength();
    
    if (l == 0 || ((start) + chars) == 0)
    {
        return;
    }
    
    // Do not return data beyond the end of the string
    if (start >= l)
    {
        return;
    }
    
    if (((start) + chars) >= l)
    {
        chars = CPOS(l - start);
    }
    
    // Copy bytes
    result->CoreAddChars(GetString()+start, chars);
}


CStr CStr::Mid(CPOS start, CPOS length) const
{
    CStr result;
    result.Empty();
    CPOS l = GetLength();
    if (l == 0 || ((start) + length) == 0)
        return result;
    if (start >= l)
        return result;
    if ((start + length) > l)
        length = CPOS(l - start);
    result.CoreAddChars(GetString()+start, length);
    return result;
}


void CStr::GetLeft(CPOS chars, CStr *result)
{
    GetMiddle(0, chars, result);
}


/*********************************************************************
* Proc:		get right
*********************************************************************/ 
void CStr::GetRight(CPOS chars, CStr *result)
{
    if (chars >= GetLength())
    {
        result = this;
        return;
    }
    GetMiddle(CPOS(GetLength()-chars), chars, result);
}


/*********************************************************************
* Proc:		CStr::TruncateAt
* Purpose:	Cuts off the string at the character with the specified
*			index.  The allocated buffer remains the same.
*********************************************************************/ 
void CStr::TruncateAt(CPOS idx)
{
    if (idx >= GetLength())
    {
        return;
    }
    
    // Spawn a copy if necessary
    Buffer(data->m_Alloc);    // Preserve buffer size
    
    // And do the truncation
    data->m_Text[idx] = 0;
    data->m_Length = idx;
}


/*********************************************************************
* Proc:		find
*********************************************************************/ 
int CStr::Find(char *lpszSub, int nStart) const
{
    int     nLength = GetLength();
    
    if (nStart > nLength)
    {
        return-1;
    }
    
    // find first matching substring
    char     *lpsz = strstr(data->m_Text + nStart, lpszSub);
    
    // return -1 for not found, distance from beginning otherwise
    return(lpsz == NULL) ? -1 : (int)(lpsz - data->m_Text);
}


/*********************************************************************
* Proc:		CStr::Find and ReverseFind
* Purpose:	Scan the string for a particular character (must not
*			be 0); return the index where the character is found
*			first, or -1 if cannot be met
*********************************************************************/ 
int CStr::Replace(char ch, CPOS startat /*= 0*/ ) const
{
    if (startat < 0 || startat >= GetLength())
    {
        ThrowBadIndex(startat, GetLength());
        return 0;
    }
    data->m_Text[startat] = ch;
    return 1;
}


/*********************************************************************
* Proc:		CStr::Find and ReverseFind
* Purpose:	Scan the string for a particular character (must not
*			be 0); return the index where the character is found
*			first, or -1 if cannot be met
*********************************************************************/ 
int CStr::Find(char ch, CPOS startat /*= 0*/ ) const
{
    // Start from middle of string?
    if (startat > 0)
    {
        if (startat >= GetLength())
        {
            ThrowBadIndex(startat, GetLength());
            return-1;
        }
    }
    
    char     *scan = strchr(data->m_Text + startat, ch);
    
    if (scan == NULL)
    {
        return-1;
    }
    else
    {
        return scan - data->m_Text;
    }
}


/*********************************************************************
* Proc:		CStr::reverse find
*********************************************************************/ 
int CStr::ReverseFind(char ch, CPOS startat /*= -1*/ ) const
{
    if (startat == (UINT) - 1)
    {
        // Scan entire string
        char     *scan = strrchr(data->m_Text, ch);
        
        if (scan)
        {
            return scan - data->m_Text;
        }
    }
    else
    {
        // Make sure the index is OK
        if (startat >= GetLength())
        {
            ThrowBadIndex(startat, GetLength());
            return-1;
        }
        
        for (int findex = (int)startat - 1; findex >= 0; findex--)
        {
            if (data->m_Text[findex] == ch)
            {
                return findex;
            }
        }
    }
    return-1;
}


/*********************************************************************
* Proc:		CStr::Compare and CompareNoCase
*********************************************************************/ 
int CStr::Compare(const char *match) const
{
    int     i = strcmp(data->m_Text, match);
    
    if (i == 0)
    {
        return 0;
    }
    else if (i < 0)
    {
        return-1;
    }
    else
    {
        return 1;
    }
}


/*********************************************************************
* Proc:		CStr::compare no case
*********************************************************************/ 
int CStr::CompareNoCase(const char *match) const
{
	if(match == 0)
	{
		return -1;
	}

	if(strlen(match) == 0 && this->GetLength() > 0)
	{
		return -1;
	}


#ifdef WIN32
    int     i = stricmp(match, data->m_Text);
#endif

#ifndef WIN32
    int     i = strcasecmp(match, data->m_Text);
#endif
    
    int     len = strlen(data->m_Text);
    int     len1 = strlen(match);
    
    if (len != len1)
    {
        return-1;
    }
    
    if (i == 0)
    {
        return 0;
    }
    else if (i < 0)
    {
        return-1;
    }
    else
    {
        return 1;
    }
}


/*********************************************************************
* Proc:		CStr::GrowTo
* Purpose:	If the buffer is smaller than the amount of characters
*			specified, reallocates the buffer.  This function cannot
*			reallocate to a buffer smaller than the existing one.
*********************************************************************/ 
void CStr::GrowTo(CPOS size)
{
    Buffer(size);
}

/*********************************************************************
* Proc:		CStr::operator == (basic forms, the rest are )
*********************************************************************/ 
BOOL operator == ( const CStr &s1, const CStr &s2)
{
    UINT     slen = s2.GetLength();
    
    if (s1.GetLength() != slen)
    {
        return FALSE;
    }
    return memcmp(s1.GetString(), s2.GetString(), slen) == 0;
}

/*********************************************************************
* Proc:		CStr::operator == (basic forms, the rest are )
*********************************************************************/ 
BOOL operator == ( const CStr &s1, LPCTSTR s2)
{
    UINT     slen = strlen(s2);
    
    if (s1.GetLength() != slen)
    {
        return FALSE;
    }
    return memcmp(s1.GetString(), s2, slen) == 0;
}


/*********************************************************************
* Proc:		CStr::RemoveLeft
*********************************************************************/ 
void CStr::RemoveLeft(CPOS count)
{
    if (GetLength() <= count)
    {
        Empty();
        return;
    }
    
    if (count == 0)
    {
        return;
    }
    Buffer(data->m_Alloc);    // Preserve buffer size
    memmove(data->m_Text, data->m_Text + count, GetLength()-count + 1);
    data->m_Length = CPOS(data->m_Length - count);
}


/*********************************************************************
* Proc:		remove middle
*********************************************************************/ 
void CStr::RemoveMiddle(CPOS start, CPOS count)
{
    if (GetLength() <= start)
    {
        Empty();
        return;
    }
    Buffer(data->m_Alloc);    // Preserve buffer size

    char     *pstart =  &data->m_Text[start];
    char     *pstart1 = &data->m_Text[start+count];
    
    if (GetLength() <= (start + count))
    { 
        pstart[0] = 0;
        data->m_Length = start;
        return;
    }



	int destLen = (GetLength()-(start + count) + 1);
    memmove(pstart,pstart1,destLen);
    data->m_Length = CPOS(data->m_Length - count);
}


/*********************************************************************
* Proc:		Remove Right
*********************************************************************/ 
void CStr::RemoveRight(CPOS count)
{
    if (GetLength() <= count)
    {
        Empty();
    }
    else
    {
        TruncateAt(CPOS(GetLength()-count));
    }
}


/*********************************************************************
* Proc:		CStr::FmtOneValue
* Purpose:	Helper for CStr::Format, formats one %??? item
* In:		x - ptr to the '%' sign in the specification; on exit,
*				will point to the first char after the spec.
* Out:		True if OK, False if should end formatting (but also copy
*			the remaining characters at x)
*********************************************************************/ 
BOOL CStr::FmtOneValue(const char *&x, va_list &marker)
{
    // Start copying format specifier to a local buffer
    char     fsbuf[64];
    
    fsbuf[0] = '%';
    
    int     fsp = 1;

GetMoreSpecifiers:
    
    // Get one character
    if (fsp >= sizeof(fsbuf))
    {
        CStr::ThrowPgmError();
        return FALSE;
    }
    
    char     ch = x[0];
    
    if (ch == 0)
        return FALSE;    // unexpected end of format string
    x++;
    
    // Chars that may exist in the format prefix
    const char     fprefix[] = "-+0 #*.123456789hlL";
    
    if (strchr(fprefix, ch) != NULL)
    {
        fsbuf[fsp] = ch;
        fsp++;
        goto GetMoreSpecifiers;
    }
    
    // 's' is the most important parameter specifier type
    if (ch == 's')
    {
        // Find out how many characters should we actually print.
        //   To do this, get the string length, but also try to
        //   detect a .precision field in the format specifier prefix.
        const char     *value = va_arg(marker, const char *);
        UINT           slen = strlen(value);
        
        fsbuf[fsp] = 0;
        
        char     *precis = strchr(fsbuf, '.');
        
        if (precis != NULL && precis[1] != 0)
        {
            // Convert value after dot, put within 0 and slen
            char     *endptr;
            int      result = (int)strtol(precis + 1, &endptr, 10);
            
            if (result >= 0 && result < int(slen))
                slen = (UINT)result;
        }
        
        // Copy the appropriate number of characters
        if (slen > 0)
            CoreAddChars(value, (CPOS)slen);
        return TRUE;
    }
    
    // '!' is our private extension, allows direct passing of CStr*
    if (ch == '!')
    {
        // No precision characters taken into account here.
        const CStr     *value = va_arg(marker, const CStr *);
        
        *this += *value;
        return TRUE;
    }
    
    // Chars that format an integer value
    const char     intletters[] = "cCdiouxX";
    
    if (strchr(intletters, ch) != NULL)
    {
        fsbuf[fsp] = ch;
        fsbuf[fsp + 1] = 0;
        
        char     valbuf[64];
        int      value = va_arg(marker, int);
        
        sprintf(valbuf, fsbuf, value);
        *this += valbuf;
        return TRUE;
    };
    
    // Chars that format a double value
    const char     dblletters[] = "eEfgG";
    
    if (strchr(dblletters, ch) != NULL)
    {
        fsbuf[fsp] = ch;
        fsbuf[fsp + 1] = 0;
        
        char       valbuf[256];
        double     value = va_arg(marker, double);
        
        sprintf(valbuf, fsbuf, value);
        *this += valbuf;
        return TRUE;
    }
    
    // 'Print pointer' is supported
    if (ch == 'p')
    {
        fsbuf[fsp] = ch;
        fsbuf[fsp + 1] = 0;
        
        char     valbuf[64];
        void     *value = va_arg(marker, void *);
        
        sprintf(valbuf, fsbuf, value);
        *this += valbuf;
        return TRUE;
    };
    
    // 'store # written so far' is obscure and unsupported
    if (ch == 'n')
    {
        ThrowPgmError();
        return FALSE;
    }
    
    // 'Print unicode string' is not supported also
    if (ch == 'S')
    {
        ThrowNoUnicode();
        return FALSE;
    }
    
    // If we fall here, the character does not represent an item
    AddChar(ch);
    return TRUE;
}


/*********************************************************************
* Proc:		CStr::Format
* Purpose:	sprintf-like method
*********************************************************************/ 
void CStr::FormatCore(const char *x, va_list &marker)
{
    for (;;)
    {
        // Locate next % sign, copy chunk, and exit if no more
        LPCSTR     next_p = strchr(x, '%');
        
        if (next_p == NULL)
		{
            break;
		}
        if (next_p > x)
        {
            CoreAddChars(x, CPOS(next_p - x));
        }
        x = next_p + 1;
        
        // We're at a parameter
        if (!FmtOneValue(x, marker))
        {
            break;    // Copy rest of string and return
        }
    }
    
    if (x[0] != 0)
	{
        *this += x;
	}
}


/*********************************************************************
* Proc:		CSTR::format
*********************************************************************/ 
void CStr::Format(const char *fmt, ...)
{
	poMutex->vSetMutex();

    Empty();
    
    // Walk the string
    va_list     marker;
    
    va_start(marker, fmt);
    FormatCore(fmt, marker);
    va_end(marker);

	poMutex->vClearMutex();
}

/*********************************************************************
* Proc:		operator + (CStr and CStr, CStr and LPCSTR)
*********************************************************************/ 
CStr operator + ( const CStr &s1, const CStr &s2)
{
    CStr     out(CPOS(s1.GetLength() + s2.GetLength()));
    
    out = s1;
    out += s2;
    return out;
}

/*********************************************************************
* Proc:		operator + (CStr and CStr, CStr and LPCSTR)
*********************************************************************/ 
CStr operator + ( const CStr &s, const char *lpsz)
{
    UINT     slen = strlen(lpsz);
    CStr     out(CPOS(s.GetLength() + slen));
    
    out = s;
    out += lpsz;
    return out;
}


/*********************************************************************
* Proc:		CStr::throw on bad index
*********************************************************************/ 
void CStr::ThrowBadIndex(int nStart, int nLen) const
{
    printf("Error Bad Index in CStr [%d] [%d]\n", nStart, nLen);
    fflush(stdout);
}


/*********************************************************************
* Proc:		CStr::throw on program error
*********************************************************************/ 
void CStr::ThrowPgmError(void)
{
    printf("ERROR Program CStr\n");
    fflush(stdout);
}


/*********************************************************************
* Proc:		CStr::throw if no unicode
*********************************************************************/ 
void CStr::ThrowNoUnicode(void)
{
    printf("ERROR No Unicode in CStr\n");
    fflush(stdout);
}


/*********************************************************************
* Proc:		CStr::throw if to large
*********************************************************************/ 
void CStr::ThrowTooLarge(void)
{
    printf("ERROR Too large in CStr\n");
    fflush(stdout);
}


/*********************************************************************
* Proc:		CStr::Throw Error if null
*********************************************************************/ 
void CStr::ThrowIfNull(void *)
{
    printf("ERROR Null in CStr\n");
    fflush(stdout);
}


/*********************************************************************
* Proc:		CStr::CStr()
* Purpose:	Constructs an empty instance
*********************************************************************/ 
CStr::CStr()
{
	poMutex = new CNetMutex(0,MUTEX_NO_DEBUG,"CStr");
	data = new SDesc(1);
	nUID = nGlobalCnt;
	nGlobalCnt++;

}


/*********************************************************************
* Proc:		CStr::copy constructor
*********************************************************************/ 
CStr::CStr( const CStr &source)
{
 	poMutex = new CNetMutex(0,MUTEX_NO_DEBUG,"CStr");
    data = source.data;

	nUID = nGlobalCnt;
	nGlobalCnt++;

}

/*********************************************************************
* Proc:		CStr::CStr (UINT prealloc)
* Purpose:	Instantiates an empty string, but with the specified
*			number of bytes in the preallocated buffer.
* In:		prealloc - number of bytes to reserve
*********************************************************************/ 
CStr::CStr( CPOS prealloc)
{
 	poMutex = new CNetMutex(0,MUTEX_NO_DEBUG,"CStr");
    up_alloc(prealloc);
    data = new SDesc(prealloc + 1);
	nUID = nGlobalCnt;
	nGlobalCnt++;
}


/*********************************************************************
* Proc:		CStr::CStr(const char*)
* Purpose:	Construct an instance that exactly copies the specified
*			string.  An optional second parameter specifies the buffer
*			size (will be ignored if it is less than what's needed)
*********************************************************************/ 
CStr::CStr( const char *s, CPOS prealloc /*= 0*/ )
{
    UINT     slen = strlen(s);
    
  	poMutex = new CNetMutex(0,MUTEX_NO_DEBUG,"CStr");
    data = new SDesc(slen);
    NewFromString(s, (CPOS)slen, prealloc);
	nUID = nGlobalCnt;
	nGlobalCnt++;
}


/*********************************************************************
* Proc:		CStr::destructor
*********************************************************************/ 
CStr::~CStr()
{
	delete data;
	data = 0;

	delete poMutex;
	poMutex = 0;

	nGlobalCnt--;

}

/*********************************************************************
* Proc:		CStr::GetFirstChar
*********************************************************************/ 
char CStr::GetFirstChar() const
{
    return data->m_Text[0];
}


/*********************************************************************
* Proc:		CStr::IsEmpty and GetLength
*********************************************************************/ 
BOOL CStr::IsEmpty(void) const
{
    return data->m_Length == 0;
}


/*********************************************************************
* Proc:		CStr::Get Length
*********************************************************************/ 
CPOS CStr::GetLength(void) const
{
    return data->m_Length;
}

/*********************************************************************
* Proc:		CStr::operator []
*********************************************************************/ 
char CStr:: operator [](CPOS idx) const
{
    if (idx >= GetLength() || idx < 0)
    {
        ThrowBadIndex(idx, GetLength());
        return 0;
    }
    return data->m_Text[idx];
}


/*********************************************************************
* Proc:		CStr::operator []
*********************************************************************/ 
char * CStr::GetAtPos(CPOS idx) const
{
    if (idx >= GetLength() || idx < 0)
    {
        ThrowBadIndex(idx, GetLength());
        return 0;
    }
    return &data->m_Text[idx];
}


/*********************************************************************
* Proc:		CStr::operator []
*********************************************************************/ 
char CStr::GetAt(CPOS idx) const
{
    if (idx >= GetLength() || idx < 0)
    {
        ThrowBadIndex(idx, GetLength());
        return 0;
    }
    return data->m_Text[idx];
}



/*********************************************************************
* Proc:		CStr::[operator ==] and [operator !=] d forms
*********************************************************************/ 
BOOL operator == ( LPCTSTR s1, const CStr &s2)
{
    return(s2 == s1);
}
BOOL operator != ( const CStr &s1, const CStr &s2)
{
    return !(s1 == s2);
}
BOOL operator != ( const CStr &s1, LPCTSTR s2)
{
    return !(s1 == s2);
}
BOOL operator != ( LPCTSTR s1, const CStr &s2)
{
    return !(s2 == s1);
}


/*********************************************************************
* Proc:		CStr::AddString - synonyms for operators +=
*********************************************************************/ 
void CStr::AddString(const CStr &obj)
{
    *this += obj;
}


void CStr::AddString(const char *s)
{
    *this += s;
}


void CStr::AddStringAtLeft(const CStr &obj)
{
    AddStringAtLeft(obj.GetString());
}

/*********************************************************************
* Proc:		operator + for CStr
*********************************************************************/ 
CStr operator + ( const char *lpsz, const CStr &s)
{
    return s + lpsz;
}

#include <errno.h>
//////////////////////////////////////////////////////
//
//	Get File Size
//
//////////////////////////////////////////////////////
long CStr::nGetFileSize(char *fileName)
{
	short fh = 0;
	long nSize = 0;


//Get File Size
#ifdef WIN32
	if(( fh = _open(fileName, _O_RDONLY ) ) <=  0 ) 
	{	
		int nVal = errno;
		switch(errno)
		{
			case EACCES:
				{
					int x=0;
				}
				break;
		};
		return -1;
	}
#endif

#ifndef WIN32
	if(( fh = open(fileName, O_RDONLY ) ) <=  0 ) 
	{
		return -1;
	}
#endif

#ifdef WIN32
	nSize = _lseek( fh, 0L , 2 );
	//size = (( unsigned short )tell(fh));
	_close( fh );
#endif


#ifndef WIN32
	nSize = lseek( fh, 0L , 2 );
	//size = (( unsigned short )tell(fh));
	close( fh );
#endif

	if(nSize <= 0)
	{
			return -1;
	}

return nSize;
}

/*********************************************************************
* Proc:		CStr::nLoadFromFile
* Purpose:	
*			
*********************************************************************/ 
int CStr::nLoadFromFile(char *filename)
{
  
	unsigned int nSize = nGetFileSize(filename);

	if(nSize == -1)
	{
		return -1;
	}

	FILE *fp = 0;

	fp = fopen(filename,"r");
	if(fp == 0)
	{
		return -1;
	}

   // Round up to blkalloc() granularity
    up_alloc(nSize);
    
    if (data->m_Alloc < nSize)
    {
        if (data->m_Text != 0)
        {
            delete data;
            data = new SDesc(nSize + 1);
            data->m_Length = nSize;
        }
    }
    else
    {
        memset(data->m_Text, 0, data->m_Alloc);
    }

	data->m_Length = nSize;

	int nRead = fread(data->m_Text,nSize,1,fp);

	fclose(fp);
return nRead;
};


/*********************************************************************
* Proc:		CStr::nLoadFromFile
* Purpose:	
*			
*********************************************************************/ 
int CStr::nWriteToFile(char *filename)
{
	FILE *fp = 0;

	fp = fopen(filename,"wt");
	if(fp == 0)
	{
		return -1;
	}

	int nWrite = fwrite(data->m_Text,GetLength(),1,fp);

	fclose(fp);
return nWrite;
};

/*********************************************************************
* Proc:		CStr::vToLower
* Purpose:	
*			
*********************************************************************/ 
void CStr::vToLower(void)
{
	for(unsigned int x=0; x<data->m_Length; x++)
	{
		char tChar = (char)tolower((int)data->m_Text[x]);
		data->m_Text[x] = tChar;
	}
}


/*********************************************************************
* Proc:		CStr::vToUpper
* Purpose:	
*			
*********************************************************************/ 
void CStr::vToUpper(void)
{
	for(unsigned int x=0; x<data->m_Length; x++)
	{
		char tChar = (char)toupper((int)data->m_Text[x]);
		data->m_Text[x] = tChar;
	}
}

/*********************************************************************
* Proc:		CStr::Replace String
* Purpose:	
*			
*********************************************************************/ 
int CStr::nReplaceString(char *src, char *dest)
{
	int nSrcLen = strlen(src);
	int nDestLen = strlen(dest);

	int nVal = Find(src,0);

	//nVal -= 1;

	if(nVal < 0)
	{
		return -1;
	}

	RemoveMiddle(nVal,nSrcLen);
	int nRetVal = InsertChars((const char *)dest,nVal,nDestLen);
return nRetVal;
}


//*********************************************************************
//* Proc:		CStr::Global Count
//* Purpose:	
//*			
//*********************************************************************/ 
unsigned int CStr::nGetGlobalCount(void)
{
	return this->nGlobalCnt;
}

//*********************************************************************
//* Proc:		CStr::Get Uid
//* Purpose:	
//*			
//*********************************************************************/ 
unsigned int CStr::nGetUniqueID(void)
{
	return this->nUID;
}


//*********************************************************************
//* Proc:		CStr::StringIP to IP
//* Purpose:	
//*			
//*********************************************************************/ 
int CStr::nStringToIP(unsigned char * p)
{

	int val = GetLength();
	char *a_address = new char[val+1];
	memset(a_address,0,val+1);
	memcpy(a_address,data->m_Text,val);
	

	a_address[val] = 0;
	char *pt = 0;

	for(int x=0; x<4; x++)
	{
				p[x] = 0;
	}

	p[0] = a_address[0];
	pt = strchr(a_address,'.');
	*pt = '\0';
	p[0] = atoi(a_address);


	if(pt == 0)
	{
		return -1;
	}

	pt++;
	p[1] = atoi(pt);
	pt = strchr(pt,'.');

	if(pt == 0)
	{
		return -1;
	}
	
	
	*pt = '\0';
	pt++;
	p[2] = atoi(pt);
	pt = strchr(pt,'.');

	if(pt == 0)
	{
		return -1;
	}
	
	*pt = '\0';
	pt++;
	p[3] = atoi(pt);
	pt = strchr(pt,'.');


	delete [] a_address;

return 1;
}

//*********************************************************************
//* Proc:		CStr::StringIP to IP
//* Purpose:	
//*			
//*********************************************************************/ 
int CStr::nStringToIP(int * p)
{

	int val = GetLength();
	char *a_address = new char[val+1];
	memset(a_address,0,val+1);
	memcpy(a_address,data->m_Text,val);
	

	a_address[val] = 0;
	char *pt = 0;

	for(int x=0; x<4; x++)
	{
				p[x] = 0;
	}

	p[0] = a_address[0];
	pt = strchr(a_address,'.');
	*pt = '\0';
	p[0] = atoi(a_address);


	if(pt == 0)
	{
		return -1;
	}

	pt++;
	p[1] = atoi(pt);
	pt = strchr(pt,'.');

	if(pt == 0)
	{
		return -1;
	}
	
	
	*pt = '\0';
	pt++;
	p[2] = atoi(pt);
	pt = strchr(pt,'.');

	if(pt == 0)
	{
		return -1;
	}
	
	*pt = '\0';
	pt++;
	p[3] = atoi(pt);
	pt = strchr(pt,'.');


	delete [] a_address;

return 1;
}

//*********************************************************************
//* Proc:		Is Ascii
//* Purpose:	
//*			
//*********************************************************************/ 
int CStr::nIsAlphaNum(CPOS nPos)
{
	if(isalpha(data->m_Text[nPos]) || isalnum(data->m_Text[nPos]))
	{
		return 1;
	}

return 0;
}

//*********************************************************************
//* Proc:		CStr::GetDateTime
//* Purpose:	
//*			
//*********************************************************************/ 
char * CStr::poGetDateTime(FILETIME *poTime)
{
	struct tm *newtime;
    char am_pm[] = "AM";
	t64 long_time;

	CDLP_TIME oTime;
	oTime.FileTimeToT64(poTime,&long_time);
    newtime = localtime((time_t *)&long_time ); /* Convert to local time. */

     if( newtime->tm_hour > 12 )        /* Set up extension. */
	 {
        strcpy( am_pm, "PM" );
	 }
	 
	 if( newtime->tm_hour > 12 )        /* Convert from 24-hour */
     {
		 newtime->tm_hour -= 12;		/*   to 12-hour clock.  */
	 } 
	 
	 if( newtime->tm_hour == 0 )        /*Set hour to 12 if midnight. */
     {
		 newtime->tm_hour = 12;
	 }

	 this->Format("%.19s %s", asctime( newtime ), am_pm);
 
return this->GetString();
}

//*********************************************************************
//* Proc:		CStr::GetDateTime
//* Purpose:	
//*			
//*********************************************************************/ 
char * CStr::poGetDateTime(SYSTEMTIME *poTime)
{
   struct tm *newtime;
    char am_pm[] = "AM";
    t64 long_time;

	CDLP_TIME oTime;
	oTime.SystemTimeToT64(poTime,&long_time);
    newtime = localtime((time_t *)&long_time ); /* Convert to local time. */

     if( newtime->tm_hour > 12 )        /* Set up extension. */
	 {
        strcpy( am_pm, "PM" );
	 }
	 
	 if( newtime->tm_hour > 12 )        /* Convert from 24-hour */
     {
		 newtime->tm_hour -= 12;		/*   to 12-hour clock.  */
	 } 
	 
	 if( newtime->tm_hour == 0 )        /*Set hour to 12 if midnight. */
     {
		 newtime->tm_hour = 12;
	 }

	 this->Format("%.19s %s", asctime( newtime ), am_pm);
 
return this->GetString();
}


//*********************************************************************
//* Proc:		CStr::GetDateTime
//* Purpose:	
//*			
//*********************************************************************/ 
char * CStr::poGetDateTime(void)
{
    struct tm *newtime;
    char am_pm[] = "AM";
    time_t long_time;

    time( &long_time );                /* Get time as long integer. */
    newtime = localtime( &long_time ); /* Convert to local time. */

     if( newtime->tm_hour > 12 )        /* Set up extension. */
	 {
        strcpy( am_pm, "PM" );
	 }
	 
	 if( newtime->tm_hour > 12 )        /* Convert from 24-hour */
     {
		 newtime->tm_hour -= 12;		/*   to 12-hour clock.  */
	 } 
	 
	 if( newtime->tm_hour == 0 )        /*Set hour to 12 if midnight. */
     {
		 newtime->tm_hour = 12;
	 }

	 this->Format("%.19s %s", asctime( newtime ), am_pm);
 
return this->GetString();
}

//*********************************************************************
//* Proc:		CStr::GetDateTimeByTick
//* Purpose:	
//*			
//*********************************************************************/ 
char * CStr::poGetDateTimeByTick(long nTick)
{
	
	long nSec	= nTick / 1000;
	long nMin	= 0;
	long nHour	= 0;
	long nDay   = 0;
	
	
	if(nSec >= 60)
	{
		nMin	= nSec / 60;
		nSec	= (nSec - (nMin * 60));
	}

	if(nMin >= 60)
	{
		nHour	= nMin / 60;
		nMin	= (nMin- (nHour * 60));
	}

	if(nHour >= 24)
	{
		nDay   = nHour / 24;
	}




	if(nDay > 0)
	{
		this->Format("%dd:",nDay);
	}

	if(nHour > 0)
	{
		CStr oStr;
		oStr.Format("%dh:",nHour);
		this->AddString(oStr.GetString());
	}

	if(nMin > 0)
	{
		CStr oStr;
		oStr.Format("%dn:",nMin);
		this->AddString(oStr.GetString());
	}

	CStr oStr;
	oStr.Format("%ds",nSec);
	this->AddString(oStr.GetString());

return this->GetString();
}

 
//*********************************************************************
//* Proc:		CStr::Replace All
//* Purpose:	
//*			
//*********************************************************************/ 
int CStr::nReplaceAllInstances(char *src, char *dest)
{
	int nTotalSize = 100000;
	int nTotalCnt = 0;

	int nStart = 0;
	int nPos = Find(src,nStart);

	if(nPos == -1)
	{
		return 0;
	}

	nTotalCnt++;

	for(int x=0; x<nTotalSize; x++)
	{
		int nNewPos = nPos+1;
		nPos = Find(src,nNewPos);

		if(nPos == -1)
		{
			break;
		}
		else
		{
			nTotalCnt++;
		}
	}


	int nSrcLen = strlen(src);
	int nDestLen = strlen(dest);


	nStart = 0;

	for(int x1 = 0; x1<nTotalSize; x1++)
	{
		nPos = Find(src,nStart);

		if(nPos >= 0)
		{
			RemoveMiddle(nPos,nSrcLen);
			InsertChars(dest,nPos,nDestLen);
			nStart = nPos+nDestLen;
		}
		else
		{
			break;
		}
	}

nTotalCnt;
return nTotalCnt;
}

//////////////////////////////////////////////////////
//
//	vHexToDec
//
//////////////////////////////////////////////////////
void CStr::vHexToDec(void)
{
	this->vToUpper();
	char *str = GetString();

	int i,d;
	long off,value;

	off=1;value=0;
	for (i=strlen(str)-1;i>=0;i--)
	{
		if (str[i]>='A') d=str[i]-'A'+10;else d=str[i]-'0';
		value=value+off*d;
		off=off*16;
	}
	
	Format("%d",value);
}

//////////////////////////////////////////////////////
//
//	vDecToHex
//
//////////////////////////////////////////////////////
void CStr::vDecToHex(void)
{
	CStr oStr;
	oStr.Format("%x",atoi(GetString()));
	Format("%s",oStr.GetString());
	vToUpper();
}

//////////////////////////////////////////////////////
//
//	vGetPathAndFile
//
//////////////////////////////////////////////////////
void CStr::vGetPathAndFile(char *name,CStr *poPath,CStr *poFileName)
{
	int ntVal = 0;

	CStr oStr;
	oStr.Format("%s",name);

	//Find the last path
#ifdef WIN32
	oStr.nReplaceAllInstances("/","\\");

	ntVal = oStr.ReverseFind('\\');
#endif

#ifndef WIN32
	ntVal = oStr.ReverseFind('/');
#endif

	//See if we have a path
	if(ntVal > 0)
	{
		int nCnt = oStr.GetLength();
		int nRVal = nCnt - (ntVal+1);

		
		CStr oStr1;
		oStr.GetRight(nRVal,&oStr1);
		poFileName->Format("%s",oStr1.GetString());

		CStr oStr2;
		oStr.GetLeft(ntVal,&oStr2);

		poPath->Format("%s",oStr2.GetString());

	}
	//If not then just a filename
	else
	{
		poFileName->Format("%s",name);
	}
}

//////////////////////////////////////////////////////
//
//	vGetPathAndFile
//
//////////////////////////////////////////////////////
void CStr::vGetPathAndFile(CStr *poPath,CStr *poFileName)
{
	int ntVal = 0;

	CStr oStr;
	oStr.Format("%s",GetString());

	//Find the last path
#ifdef WIN32

	oStr.nReplaceAllInstances("/","\\");

	ntVal = oStr.ReverseFind('\\');

	if(ntVal == -1)
	{
		ntVal = oStr.ReverseFind('/');
	}

#endif

#ifndef WIN32
	ntVal = oStr.ReverseFind('/');
#endif

	//See if we have a path
	if(ntVal > 0)
	{
		int nCnt = oStr.GetLength();
		int nRVal = nCnt - (ntVal+1);

		
		CStr oStr1;
		oStr.GetRight(nRVal,&oStr1);
		poFileName->Format("%s",oStr1.GetString());

		CStr oStr2;
		oStr.GetLeft(ntVal,&oStr2);

		poPath->Format("%s",oStr2.GetString());

	}
	//If not then just a filename
	else
	{
		poFileName->Format("%s",GetString());
	}
}

typedef BOOL (WINAPI *PGETDISKFREESPACEEX)(LPCSTR,
   PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);
//////////////////////////////////////////////////////
//
//	vGetDiskSpace
//
//////////////////////////////////////////////////////
void CStr::vGetDiskSpace(char *pszDrive)
{
	PGETDISKFREESPACEEX pGetDiskFreeSpaceEx;

   __int64 i64FreeBytesToCaller = 0, i64TotalBytes = 0, i64FreeBytes = 0;
   BOOL fResult = 0;

   HMODULE hMod;
   hMod = GetModuleHandle("kernel32.dll");
   pGetDiskFreeSpaceEx = (PGETDISKFREESPACEEX)GetProcAddress(hMod,
                          "GetDiskFreeSpaceExA");

   if (pGetDiskFreeSpaceEx)
   {
      fResult = pGetDiskFreeSpaceEx (pszDrive,
                 (PULARGE_INTEGER)&i64FreeBytesToCaller,
                 (PULARGE_INTEGER)&i64TotalBytes,
                 (PULARGE_INTEGER)&i64FreeBytes);

	  long nVal = 0;
	  if(i64FreeBytes > 0)
	  {
		nVal = (long)(i64FreeBytes / 1000);
	  }
	  this->Format("%d",nVal);
   // Process GetDiskFreeSpaceEx results.
      //printf("Total free bytes = %I64d\n", i64FreeBytes);
   }
}


//*********************************************************************
//* Proc:		CStr::ReadLine
//* Purpose:	
//*			
//*********************************************************************/ 
int CStr::nReadLine(int nInclEmpty,int nMaxVal,FILE *fp)
{
	Empty();
	char fdata = 0;
	
	int nVal = 0;
	
	//Read Line to MaxVal
	for(int i=0; i<nMaxVal; i++)
	{
		//Read 1 byte
		int ntVal = fread(&fdata,1,1,fp);
	
		//EOF
		if(ntVal == 0)
		{
			return -1;
		}

		//Do I want to no include CR / LF empty lines
		if(nInclEmpty == 0)
		{
			if((fdata == 10 || fdata == 13) && i == 0)
			{
				return 0;
			}
		}

		if(ntVal == 1)
			{
				nVal++;
			}

		if(fdata == 10 || fdata == 13)
		{
			break;
		}

		AddChar(fdata);
	}

return nVal;
}

//*********************************************************************
//* Proc:		CStr::nTokenize
//* Purpose:	
//*			
//*********************************************************************/ 
int CStr::nTokenize(DLP_CPtrArray *poArray,char delimiter)
{
  int index=0; 
    int count=0; 
    CStr str1=""; 
    CStr str;
	str.Format("%s",GetString());
	for (int i=0;i<(int)str.GetLength();i++) 
    { 
        if(str.GetAt(i)==delimiter) 
        { 
            CStr *poStr = new CStr;
			poStr->Format("%s",str1.GetString());
			str1.Empty();
			poArray->nAdd(poStr);
            str1=""; 
        } 
        else 
        { 
            str1.AddChar(str.GetAt(i)); 
        } 
    } 
    
	CStr *poStr = new CStr;
	poStr->Format("%s",str1.GetString());
	poArray->nAdd(poStr); 
    count=poArray->nGetSize(); 


return count;
}


//*********************************************************************
//* Proc:		CStr::nTokenize
//* Purpose:	
//*			
//*********************************************************************/ 
int CStr::nTokenize(DLP_CPtrArray *poArray,char delimiter,int nAllowDupe)
{
	int index=0; 
    int count=0; 
    CStr str1=""; 
    CStr str;
	str.Format("%s",GetString());
	for (int i=0;i<(int)str.GetLength();i++) 
    { 
        if(str.GetAt(i)==delimiter) 
        { 
            CStr *poStr = new CStr;
			poStr->Format("%s",str1.GetString());
			str1.Empty();


			if(nAllowDupe == 0)
			{
				int nFound = 0;
				for(int j=0; j<poArray->nGetSize(); j++)
				{
					CStr *potStr = (CStr *)poArray->pGetAt(j);
					
					if(potStr->CompareNoCase(poStr->GetString()) == 0)
					{
						//Set Found
						nFound = 1;
					}
				}

				//Not Found
				if(nFound == 0)
				{
					poArray->nAdd(poStr);
				}

			}
			else
			{
				poArray->nAdd(poStr);
            }
				
			str1=""; 
        } 
        else 
        { 
            str1.AddChar(str.GetAt(i)); 
        } 
    } 
    
	CStr *poStr = new CStr;
	poStr->Format("%s",str1.GetString());
	poArray->nAdd(poStr); 
    count=poArray->nGetSize(); 


return count;
}

//*********************************************************************
//* Proc:		CStr::nCompareFiles
//* Purpose:	
//*			
//*********************************************************************/ 
int CStr::nCompareFiles(char *file1, char *file2)
{ 
	long nSize1 = 0;
	long nSize2 = 0;

	nSize1 = nGetFileSize(file1);
	nSize2 = nGetFileSize(file2);

	//Error in file
	if(nSize1 == -1 || nSize2 == -1)
	{
		return -1;
	}

	//Size's not equal
	if(nSize1 != nSize2)
	{
		return 0;
	}

	//File Date
	CStr oVersion1;
	HANDLE myHandle1;
	WIN32_FIND_DATA myFileData1;
	SYSTEMTIME filetime1;
	myHandle1 = FindFirstFile( (LPCTSTR)file1, &myFileData1 );
	FileTimeToSystemTime(((FILETIME *)&myFileData1.ftLastWriteTime ), &filetime1);
	oVersion1.Format(TEXT("%d/%d/%d/%d/%d/%d/%d")
		,filetime1.wMonth
		,filetime1.wDay
		,filetime1.wYear
		,filetime1.wHour
		,filetime1.wMinute
		,filetime1.wSecond
		,filetime1.wMilliseconds);
	
	CStr oVersion2;
	HANDLE myHandle2;
	WIN32_FIND_DATA myFileData2;
	SYSTEMTIME filetime2;
	myHandle2 = FindFirstFile( (LPCTSTR)file2, &myFileData2);
	FileTimeToSystemTime(((FILETIME *)&myFileData2.ftLastWriteTime ), &filetime2);
	oVersion2.Format(TEXT("%d/%d/%d/%d/%d/%d/%d")
		,filetime2.wMonth
		,filetime2.wDay
		,filetime2.wYear
		,filetime2.wHour
		,filetime2.wMinute
		,filetime2.wSecond
		,filetime2.wMilliseconds);	

	//Dates not the same
	if(oVersion1.Compare(oVersion2.GetString()) != 0)
	{
		return 0;
	}

//The Same
return 1;
}


/*********************************************************************
* Proc:		Get String
*********************************************************************/ 
const char * CStr::GetString(void) const
{
    return data->m_Text;
}

/*********************************************************************
* Proc:		Get String
*********************************************************************/ 
char * CStr::GetString(void)
{
    return (char *)data->m_Text;
}


