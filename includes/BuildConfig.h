#ifndef BUILDCONFIG_H
#define BUILDCONFIG_H
//*****************************************************************************|
//*  FILE:    BuildConfig.h
//*
//*  AUTHOR:  Andrew Hall
//*           Copyright (c) 2003
//*           Permission is granted to use this code without restriction
//*           provided this copyright notice appears in all source files.
//*
//*  CREATED: February 24, 2003
//*
//*  SUMMARY:
//* 
//*    Declarations for the BuildConfig library, which makes source code
//*    configuration information available at runtime.
//*
//*****************************************************************************|
//       1         2         3         4         5         6         7         8
//345678901234567890123456789012345678901234567890123456789012345678901234567890
#include <iosfwd>
#include <set>

class CStr;

class VersionStamp
{
  public: 
    VersionStamp(const char* file,
                 const char* date,
                 const char* time,
                 const char* ver);

	virtual ~VersionStamp();

    const char* FileName() const;
    char *Version() const;
    const char* BuildDate() const;
    const char* BuildTime() const;

    bool operator<(const VersionStamp& rhs) const;

  private:
	CStr *m_poVersion;
    const char* mFileName;
    const char* mBuildDate;
    const char* mBuildTime;
    const char* mVersion;
};


struct VersionStampPtrLess
{
  bool operator()(const VersionStamp* lhs, const VersionStamp* rhs) const;
};


class BuildConfiguration
{
  private:
    typedef std::set<const VersionStamp*, VersionStampPtrLess> VersionInfo;
  
  public:
    typedef VersionInfo::const_iterator const_iterator;  

  public:
    static BuildConfiguration& Instance();

    void Output(std::ostream& out) const;
    void AddFile(const VersionStamp* fileStamp);

    const_iterator begin() const;
    const_iterator end() const;

  private:
    // Private for Singleton
    BuildConfiguration();

    // Unimplemented
    BuildConfiguration(const BuildConfiguration&);
    BuildConfiguration& operator=(const BuildConfiguration&);

  private:
   VersionInfo mConfig;
};


#define FILEVERHELP(VER, NAME) \
  static VersionStamp vs##NAME(__FILE__, __DATE__, __TIME__, VER)
#define FILE_VERSION(VER, NAME) FILEVERHELP(VER, NAME)

FILE_VERSION("$Revision: 1 $", BuildConfig_h);

#endif // BUILDCONFIG_H
