#if !defined(AFX_FILEINFO_H__4C620721_CFC7_11D2_AA42_A54123628E22__INCLUDED_)
#define AFX_FILEINFO_H__4C620721_CFC7_11D2_AA42_A54123628E22__INCLUDED_

class DLP_CPtrArray;
class CStr;
class CDebugInfo;
class CDLPFileInfo;
class CDLPFileManager;

typedef __int64 t64;


////////////////////////////////////////////////
#define SECS_TO_FT_MULT 10000000

////////////////////////////////////////////////
class CDLP_TIME
{
private:
	SYSTEMTIME oSystemTime;

public:
	CDLP_TIME();
	virtual ~CDLP_TIME();
	void vGetCurrentTime(void);
	void vSetTime(const FILETIME *poTime);
	CStr * poGetDate(void);

	void CDLP_TIME::T64ToFileTime(t64 *pt,FILETIME *pft);
	void CDLP_TIME::FileTimeToT64(FILETIME *pft,t64 *pt);
	t64 FindTimeTBase(void);
	void CDLP_TIME::SystemTimeToT64(SYSTEMTIME *pst,t64 *pt);
	void CDLP_TIME::T64ToSystemTime(t64 *pt,SYSTEMTIME *pst);
	t64 CDLP_TIME::time_64(t64 *pt);
};



/**
 * @class Stores information about a file in a way like <c CFindFile> does
 */
class CFileInfo 
{  
private:
	CStr *poFileName;
	CStr *poFileDrive;
	CStr *poFileDir;
	CStr *poFileTitle;
	CStr *poFileExt;
	CStr *poFileRoot;
	CStr *poFilePath;

public:
	friend CDLPFileManager;
	friend CDLP_TIME;
	friend CDLPFileInfo;
   /** @access Public members */
   CFileInfo();
   /**
    * @cmember Copy constructor
    * @parm CFileInfo to copy member variables from.
    */
   CFileInfo(const CFileInfo& finf);

   /**
    * @cmember Destructor
    */
   ~CFileInfo();

   /**
    * @cmember Initializes CFileInfo member variables.
    * @parm Values to init member variables.
    * @parm Path of the file the CFileInfo refers to.
    * @parmopt User defined parameter.
    */
   void Create(const WIN32_FIND_DATA* pwfd, const char *strPath, LPARAM lParam=NULL);

   /**
    * @cmember Initializes CFileInfo member variables.
    * @parm Absolute path for file or directory
    * @parmopt User defined parameter.
    */
   void Create(const char *strFilePath, LPARAM lParam = NULL);
   
   /**
    * @cmember Calcs 32bit checksum of file (i.e. sum of all the DWORDS of the file, 
    * truncated to 32bit).
    * @parmopt Number of maximum bytes read for checksum calculation. This number is 
    * up-rounded to a multiple of 4 bytes (DWORD). If 0 or bigger than uhFileSize, checksum
    * for all the file is calculated.
    * @parmopt Force recalculation of checksum (otherwise if checksum has already
    * been calculated, it isn't calculated again and previous calculated value is returned).
    * @parmopt Flag to allow calling application to abort the calculation of 
    * checksum (for multithreaded applications).
    * @parmopt Pointer to counter of bytes whose checksum has been calculated. 
    * This value is updated while checksum is being calculated, so calling application
    * can view the progress of checksum calc (for multithreaded applications).
    * Maximum value for pulCount is uhFileSize.
    */
   DWORD GetChecksum(const ULONGLONG uhUpto=0, const BOOL bRecalc = FALSE, 
      const volatile BOOL* pbAbort=NULL, volatile ULONG* pulCount = NULL);

   /**
    * @cmember Calcs 32bit CRC of file contents (i.e. CRC of all the DWORDS of the file).
    * @parmopt Number of maximum bytes read for CRC calculation. This number is 
    * up-rounded to a multiple of 4 bytes (DWORD). If 0 or bigger than uhFileSize, CRC
    * for all the file is calculated.
    * @parmopt Force recalculation of CRC (otherwise if CRC has already
    * been calculated, it isn't calculated again and previous calculated value is returned).
    * @parmopt pbAbort Flag to allow calling application to abort the calculation of 
    * CRC (for multithreaded applications).
    * @parmopt Pointer to counter of bytes whose CRC has been calculated. 
    * This value is updated while CRC is being calculated, so calling application
    * can view the progress of CRC calc (for multithreaded applications).
    * Maximum value for pulCount is uhFileSize.
    */
   DWORD GetCRC(const ULONGLONG dhUpto=0, const BOOL bRecalc = FALSE,
      const volatile BOOL* pbAbort=NULL, volatile ULONG* pulCount = NULL);

   /** @cmember File size in bytes as a DWORD value. */
   DWORD GetLength(void) const { return (DWORD) m_uhFileSize; };
   /** @cmember File size in bytes as an ULONGLONG value. */
   ULONGLONG GetLength64(void) const { return m_uhFileSize; };
   
   /** Get File split info (equivalent to CFindFile members) */

   /** 
    * @cmember Gets the file drive 
    * @rdesc Returns C: for C:\WINDOWS\WIN.INI 
    */
   char * pGetFileDrive(void);
   /** 
    * @cmember Gets the file dir 
    * @rdesc Returns \WINDOWS\ for C:\WINDOWS\WIN.INI 
    */
   char * pGetFileDir(void);
   /** @cmember returns WIN for C:\WINDOWS\WIN.INI */
   char * pGetFileTitle(void);
   /** @cmember returns INI for C:\WINDOWS\WIN.INI */
   char * pGetFileExt(void);
   /** @cmember returns C:\WINDOWS\ for C:\WINDOWS\WIN.INI */
   char * pGetFileRoot(void); 

   /** @cmember returns WIN.INI for C:\WINDOWS\WIN.INI */
   char * pGetFileName(void);

   /** @cmember returns C:\WINDOWS\WIN.INI for C:\WINDOWS\WIN.INI */
   char * pGetFilePath(void);

   /* Get File times info (equivalent to CFindFile members) */
   /** @cmember returns creation time */
   CDLP_TIME * poGetCreationTime(void) { return &m_timCreation; };
   /** @cmember returns last access time */
   CDLP_TIME * poGetLastAccessTime(void) { return &m_timLastAccess; };
   /** @cmember returns las write time */
   CDLP_TIME * poGetLastWriteTime(void) { return &m_timLastWrite; };

   /* Get File attributes info (equivalent to CFindFile members) */
   /** @cmember returns file attributes */
   DWORD GetAttributes(void) const { return m_dwAttributes; };
   /** @cmember returns TRUE if the file is a directory */
   BOOL IsDirectory(void) const { return m_dwAttributes & FILE_ATTRIBUTE_DIRECTORY; };
   /** @cmember Returns TRUE if the file has archive bit set */
   BOOL IsArchived(void) const { return m_dwAttributes & FILE_ATTRIBUTE_ARCHIVE; };
   /** @cmember Returns TRUE if the file is read-only */
   BOOL IsReadOnly(void) const { return m_dwAttributes & FILE_ATTRIBUTE_READONLY; };
   /** @cmember Returns TRUE if the file is compressed */
   BOOL IsCompressed(void) const { return m_dwAttributes & FILE_ATTRIBUTE_COMPRESSED; };
   /** @cmember Returns TRUE if the file is a system file */
   BOOL IsSystem(void) const { return m_dwAttributes & FILE_ATTRIBUTE_SYSTEM; };
   /** @cmember Returns TRUE if the file is hidden */
   BOOL IsHidden(void) const { return m_dwAttributes & FILE_ATTRIBUTE_HIDDEN; };
   /** @cmember Returns TRUE if the file is temporary */
   BOOL IsTemporary(void) const { return m_dwAttributes & FILE_ATTRIBUTE_TEMPORARY; };
   /** @cmember Returns TRUE if the file is a normal file */
   BOOL IsNormal(void) const { return m_dwAttributes == 0; };
   
   LPARAM m_lParam;        /** User-defined parameter */
private:
   /** @access Private members */

   CStr m_strFilePath;  /** @cmember Full filepath of file (directory+filename) */
   DWORD m_dwAttributes;   /** @cmember File attributes of file (as returned by FindFile() */
   ULONGLONG m_uhFileSize; /** @cmember File of size. (COM states LONGLONG as hyper, so "uh" means 
                          unsigned hyper) */
   CDLP_TIME m_timCreation;    /** @cmember Creation time */
   CDLP_TIME m_timLastAccess;  /** @cmember Last Access time */
   CDLP_TIME m_timLastWrite;   /** @cmember Last write time */

   DWORD m_dwChecksum;     /** @cmember Checksum calculated for the first m_uhChecksumBytes bytes */
   DWORD m_dwCRC;          /** @cmember CRC calculated for the first m_uhCRCBytes bytes */
   DWORD m_uhCRCBytes;     /** @cmember Number of file bytes with CRC calc'ed (4 multiple or filesize ) */
   DWORD m_uhChecksumBytes;/** @cmember Number of file bytes with Checksum calc'ed (4 multiple or filesize) */
}; 

/**
 * @class Allows to retrieve <c CFileInfo>s from files/directories in a directory
 */
class CFileInfoArray 
{
private:
	CDebugInfo *poDebug;

public:
   /** @access Public members */
	DLP_CPtrArray *poArray;

   /**
    * @cmember Default constructor
    */
   CFileInfoArray(CDebugInfo *poDebug);
   virtual ~CFileInfoArray();


   /** 
    * @cmember,menum Default values for <md CFileInfoArray.lAddParam>
    */   
   enum { 
      AP_NOSORT=0,         /** @@emem Insert <c CFileInfo>s in a unordered manner */
      AP_SORTASCENDING=0,  /** @@emem Insert <c CFileInfo>s in a ascending order */
      AP_SORTDESCENDING=1, /** @@emem Insert <c CFileInfo>s in a descending number */
      AP_SORTBYSIZE=2,     /** @@emem AP_SORTBYSIZE | Insert <c CFileInfo>s ordered by uhFileSize (presumes array is 
                               previously ordered by uhFileSize). */
      AP_SORTBYNAME=4      /** @@emem AP_SORTBYNAME | Insert <c CFileInfo>s ordered by strFilePath (presumes array is 
                                 previously ordered by strFilePath) */
   };

   /**
    * @cmember Adds a file or all contained in a directory to the CFileInfoArray
    * Only "static" data for CFileInfo is filled (by default CRC and checksum are NOT 
    * calculated when inserting CFileInfos).<nl> Returns the number of <c CFileInfo>s added to the array
    * @parm Name of the directory, ended in backslash.
    * @parm Mask of files to add in case that strDirName is a directory
    * @parm Wether to recurse or not subdirectories
    * @parmopt Parameter to pass to protected member function AddFileInfo
    * @parmopt Wether to add or not CFileInfos for directories
    * @parmopt Pointer to a variable to signal abort of directory retrieval 
    * (multithreaded apps).
    * @parmopt pulCount Pointer to a variable incremented each time a CFileInfo is added to the
    * array (multithreaded apps).
    * @xref <mf CFileInfoArray.AddFile> <mf CFileInfoArray.AddFileInfo> <md CFileInfoArray.AP_NOSORT>
    */
   int AddDir(const char * strDirName, const char * strMask, const BOOL bRecurse, 
      LPARAM lAddParam=AP_NOSORT, const BOOL bIncludeDirs=FALSE, 
      const volatile BOOL* pbAbort = NULL, volatile ULONG* pulCount = NULL);

   /**
    * @cmember Adds a single file or directory to the CFileInfoArray. In case of directory, files
    * contained in the directory are NOT added to the array.<nl>
    * Returns the position in the array where the <c CFileInfo> was added (-1 if <c CFileInfo>
    * wasn't added)
    * @parm Name of the file or directory to add. NOT ended with backslash.
    * @parm Parameter to pass to protected member function AddFileInfo.
    * @xref <mf CFileInfoArray.AddDir> <mf CFileInfoArray.AddFileInfo>
    */
   int AddFile(const char * strFilePath, LPARAM lAddParam);


protected:
   /** @access Protected Members */

   /**
    * @cmember Called by AddXXXX to add a CFileInfo to the array. Can be overriden to:
    * 1. Add only desired CFileInfos (filter)
    * 2. Fill user param lParam
    * 3. Change sort order/criteria<nl>
    * Returns the position in the array where the CFileInfo was added or -1 if the CFileInfo 
    * wasn't added to the array.
    * Default implementation sorts by lAddParam values and adds all CFileInfos 
    * (no filtering)
    * @parm CFileInfo to insert in the array.
    * @parm Parameter passed from AddDir function.
    * @xref <mf CFileInfoArray.AddDir>
    */
   virtual int AddFileInfo(CFileInfo *finf, LPARAM lAddParam);
};


class CDLPFileInfo
{
private:
	CDebugInfo *poDebug;
	CFileInfo *poFileInfo;

	
public:

	unsigned long nUID;
	CStr *poFileName;	
	unsigned long nSize;
	CStr *poDate;		
	CStr *poVersion;		
	CStr *poPassword;	
	CStr *poCRC;

	CDLPFileInfo(CDebugInfo *poDebug);
	virtual ~CDLPFileInfo();
	unsigned long nGetUID(void);

	   void vSetInfo(unsigned long nUID,
					char *poFileName,	
					unsigned long nSize,
					char *poDate,		
					char *poVersion,		
					char *poPassword,	
					char *poCRC
					);

};


#endif // !defined(AFX_FILEINFO_H__4C620721_CFC7_11D2_AA42_A54123628E22__INCLUDED_)

/*

	const volatile BOOL pbAbort = 0;
	m_poFilearray->AddDir("D:\\filedirtest\\", "*.*",1, 
            CFileInfoArray::AP_SORTBYSIZE | CFileInfoArray::AP_SORTASCENDING, 1, 
            &pbAbort);


	for(int i=0; i<m_poFilearray->poArray->nGetSize(); i++)
	{
		CFileInfo *poFile = (CFileInfo *)m_poFilearray->poArray->pGetAt(i);
	
		CDLP_TIME *poTime = poFile->GetCreationTime();


		CStr oStr;
		oStr.Format("%s",poFile->GetFileName());

		m_c_list1.AddString(oStr.GetString());

	}

*/