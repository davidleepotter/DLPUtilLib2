// dbase.h: Stub for CDbase (MySQL database connection base class)
// Original from netLib/mysqllinux - stubbed for cross-platform build

#ifndef DBASE_H
#define DBASE_H

class CDbase {
public:
    int nUID;
    int nInUse;
    
    CDbase() : nUID(0), nInUse(0) {}
    virtual ~CDbase() {}
    virtual void vSetType(int) {}
    virtual int nConnect(const char*, const char*, const char*, const char*, int) { return 0; }
    virtual int nConnectToDb(const char*, const char*, const char*, int, const char*, const char*) { return 0; }
    virtual int nDisconnect(void) { return 0; }
    virtual int nSelectDB(const char*) { return 0; }
    virtual int nQuery(const char*) { return 0; }
    virtual void* poGetResult(void) { return NULL; }
    virtual int nNumRows(void) { return 0; }
    virtual void* poFetchRow(void) { return NULL; }
    virtual const char* poFieldInfo(int) { return ""; }
    virtual int nAffectedRows(void) { return 0; }
    virtual long nInsertID(void) { return 0; }
    virtual int n_errno(void) { return 0; }
    virtual const char* po_error(void) { return ""; }
    virtual int n_fieldCount(void) { return 0; }
};

// CDBMySql stub - MySQL-specific database connection
class CDBMySql : public CDbase {
public:
    CDBMySql(void*) {}
    virtual ~CDBMySql() {}
};

#endif // DBASE_H
