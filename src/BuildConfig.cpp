//*****************************************************************************|
//*  FILE:    BuildConfig.cpp
//*
//*  AUTHOR:  David Potter
//*           Copyright (c) 2003
//*           
//*           
//*
//*  CREATED: Sept 18, 2003
//*
//*  SUMMARY:
//* 
//*    Implementation of the BuildConfig
//*
//*****************************************************************************|
//       1         2         3         4         5         6         7         8
//345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma warning( disable : 4786 )
#include "BuildConfig.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <string.h>
#include <assert.h>
#include "cstr.h"

FILE_VERSION("$Revision: 1 $", BuildConfig_cpp);


//*****************************************************************************|
//*   VersionStamp Implementation
//*****************************************************************************|

VersionStamp::VersionStamp(const char* file,
                           const char* date,
                           const char* time,
                           const char* ver)
  : mFileName(file)
  , mBuildDate(date)
  , mBuildTime(time)
  , mVersion(ver)
{
  m_poVersion = new CStr;
  BuildConfiguration::Instance().AddFile(this);
}

VersionStamp::~VersionStamp()
{
	delete m_poVersion;

}

const char* VersionStamp::FileName() const
{
  return mFileName;
}


char * VersionStamp::Version() const
{
  //std::string temp(mVersion);
   m_poVersion->Format("%s",mVersion);
  
  //std::string::size_type first = temp.find_first_of("0123456789");
  //std::string::size_type last = temp.find_last_of("0123456789");
 
  //temp.substr(first, last-first + 1);
  
  return m_poVersion->GetString(); 
}


const char* VersionStamp::BuildDate() const
{
  return mBuildDate;
}


const char* VersionStamp::BuildTime() const
{
  return mBuildTime;
}


bool VersionStamp::operator<(const VersionStamp& rhs) const
{
  return strcmp(mFileName, rhs.mFileName) < 0;
}


bool VersionStampPtrLess::operator ()(const VersionStamp* lhs,
                                       const VersionStamp* rhs) const
{
  return (*lhs) < (*rhs);
}


//*****************************************************************************|
//*   BuildConfiguration Implementation
//*****************************************************************************|

BuildConfiguration& BuildConfiguration::Instance()
{
  static std::unique_ptr<BuildConfiguration>
    instance(new BuildConfiguration);

  return *instance;
}


BuildConfiguration::BuildConfiguration()
{
}


void BuildConfiguration::Output(std::ostream& out) const
{
  using namespace std;

  typedef VersionInfo::const_iterator It;
  ios::fmtflags oldFlags = out.flags();
  
  out << setw(41) << left << "File Name"
      << setw(12) << right << "Version"
      << setw(15) << right << "Build Date" 
      << setw(10) << right << "Time" << '\n';

  out << "-------------------------------------------"
         "------------------------------------\n";

  for (It it(mConfig.begin()), end(mConfig.end()); it != end; ++it) {
    const VersionStamp& vs = **it;

    out << setw(41) << left << vs.FileName()
        << setw(12) << right << vs.Version() 
        << setw(15) << right << vs.BuildDate()
        << setw(10) << right << vs.BuildTime()
        << '\n';
  }

  out.flags(oldFlags);
}


void BuildConfiguration::AddFile(const VersionStamp* fileStamp)
{
  assert(fileStamp);
  mConfig.insert(fileStamp);
}


BuildConfiguration::const_iterator BuildConfiguration::begin() const
{
  return mConfig.begin();
}


BuildConfiguration::const_iterator BuildConfiguration::end() const
{
  return mConfig.end();
}
