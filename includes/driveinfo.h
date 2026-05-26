/*********************************************************************
*	File : DriveInfo.h
*	Desc : Drive Info
*
*	Comment: 
*
*
*
*	Date: 5/5/2004
*********************************************************************/
#if !defined(AFX_DRIVEINFO_INCLUDED_)
#define AFX_DRIVEINFO_INCLUDED_

//  SENDCMDINPARAMS contains the input parameters for the 
//  Send Command to Drive function.
//  IDE registers
typedef struct _IDEREGS
{
   BYTE bFeaturesReg;       // Used for specifying SMART "commands".
   BYTE bSectorCountReg;    // IDE sector count register
   BYTE bSectorNumberReg;   // IDE sector number register
   BYTE bCylLowReg;         // IDE low order cylinder value
   BYTE bCylHighReg;        // IDE high order cylinder value
   BYTE bDriveHeadReg;      // IDE drive/head register
   BYTE bCommandReg;        // Actual IDE command.
   BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;


typedef struct _SENDCMDINPARAMS
{
   DWORD     cBufferSize;   //  Buffer size in bytes
   IDEREGS   irDriveRegs;   //  Structure with drive register values.
   BYTE bDriveNumber;       //  Physical drive number to send 
                            //  command to (0,1,2,3).
   BYTE bReserved[3];       //  Reserved for future expansion.
   DWORD     dwReserved[4]; //  For future use.
   BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

// Status returned from driver
typedef struct _DRIVERSTATUS
{
   BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
   BYTE  bIDEStatus;    //  Contents of IDE Error register.
                        //  Only valid when bDriverError is SMART_IDE_ERROR.
   BYTE  bReserved[2];  //  Reserved for future expansion.
   DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;


// Structure returned by PhysicalDrive IOCTL for several commands
typedef struct _SENDCMDOUTPARAMS
{
   DWORD         cBufferSize;   //  Size of bBuffer in bytes
   DRIVERSTATUS  DriverStatus;  //  Driver status structure.
   BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;


class CDebugInfo;
class DLP_CPtrArray;
class CStr;

class CDriveObj
{
private:
	CDebugInfo *poDebug;

public:
	CStr *poSerialNum;
	CStr *poControllerType;
	CStr *poDriveType;
	CStr *poDriveModel;
	int nMaster;
	int nFixed;
	int nSize;

	unsigned long nHDID;
	
	CDriveObj(CDebugInfo *poDebug);
	virtual ~CDriveObj(void);
};

class CDriveInfo
{
private:
	CDebugInfo *poDebug;

	int nReadPhysicalDriveInNTWithAdminRights(void);
	char * pflipAndCodeBytes (char * str);
	int nReadPhysicalDriveInNTWithZeroRights(void);
	BOOL nDoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                 PDWORD lpcbBytesReturned);
	int nReadDrivePortsInWin9X (void);
	int nReadIdeDriveAsScsiDriveInNT(void);
	void vPrintIdeInfo(int drive, DWORD diskdata [256]);
	char *pConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex);
	unsigned long nConvertID(char *id);


public:
	DLP_CPtrArray *poDriveArray;

	CDriveInfo(CDebugInfo *poDebug);
	virtual ~CDriveInfo(void);
	long nGetHardDriveComputerID(void);
};

#endif