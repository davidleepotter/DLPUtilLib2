#ifndef TCHAR_H
#define TCHAR_H

#include <string.h>
#include <wchar.h>

// TCHAR stubs for Linux
typedef char TCHAR;
typedef char _TCHAR;
#define _tcslen strlen
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcsstr strstr
#define _tcschr strchr
#define _tcsrchr strrchr
#define _tcsicmp strcasecmp
#define _tcsnicmp strncasecmp
#define _tcsupr strupr
#define _tcsdown strlwr
#define _T(x) x
#define _TEXT(x) x
#define _TCHAR_DEFINED

#endif // TCHAR_H
