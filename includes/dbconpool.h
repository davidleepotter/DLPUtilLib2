// dbconpool.h: Stub for CDBConPool (Database Connection Pool)
// Original from netLib/mysqllinux - stubbed for cross-platform build

#ifndef DBCONPOOL_H
#define DBCONPOOL_H

#include "DLPFile.h"

class CDebugInfo;
class CStr;
class CDbase;
class DLP_CPtrList;
class DLP_CPtrListItem;

class CDBConPool {
private:
    DLP_CPtrList *poFreeList;
    DLP_CPtrList *poInUseList;
    CDebugInfo *poDebug;
    int nMaxPool;
    int nType;
    int nPort;
    CStr *poDatabase;
    CStr *poBaseTable;
    CStr *poUsername;
    CStr *poPassword;
    CStr *poIpAddress;

public:
    CDBConPool(CDebugInfo *potDebug, int ntMaxPool, int ntType, char *database, 
               char *table, char *username, char *password, char *ipaddress, int ntPort);
    virtual ~CDBConPool();
    int nInitConPool(void);
    CDbase *poGetNextConnection(void);
    int nReleaseConnection(CDbase *poUsedDBase);
    CDbase *poCreateDb(void);
};

#endif // DBCONPOOL_H
