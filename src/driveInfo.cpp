/*********************************************************************
*	File : DriveInfo.cpp
*	Desc : Drive Info
*
*	Comment: 
*
*
*
*	Date: 5/5/2004
*********************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <windows.h>
#include "utilheader.h"
#include <winioctl.h>
#include <drivestructs.h>

 
//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CDriveObj::CDriveObj(CDebugInfo *potDebug)
{
	poDebug = potDebug;
	poSerialNum			= new CStr;
	poControllerType	= new CStr;
	poDriveType			= new CStr;
	poDriveModel		= new CStr;
	nMaster				= 0;
	nHDID				= 0;
	nFixed				= 0;
	nSize				= 0;
}

//////////////////////////////////////////////////////
//
//	Constructor
//
//////////////////////////////////////////////////////
CDriveObj::~CDriveObj(void)
{
	delete poControllerType;
	delete poDriveType;
	delete poDriveModel;
	delete poSerialNum;

	poControllerType = 0;
	poDriveType = 0;
	poDriveModel = 0;
	poSerialNum = 0;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CDriveInfo::CDriveInfo(CDebugInfo *potDebug)
{
		poDebug = potDebug;
		poDriveArray = new DLP_CPtrArray;
}

//////////////////////////////////////////////////////
//
//	Destructor
//
//////////////////////////////////////////////////////
CDriveInfo::~CDriveInfo(void)
{
	for(int i=0; i<poDriveArray->nGetSize(); i++)
	{
		CDriveObj *poDrive = (CDriveObj *)poDriveArray->pGetAt(i);
		delete poDrive;
	}

	poDriveArray->vRemoveAll();
	delete poDriveArray;
	poDriveArray = 0;
}

//////////////////////////////////////////////////////
//
//	nReadPhysicalDriveInNTWithAdminRights
//
//////////////////////////////////////////////////////
int CDriveInfo::nReadPhysicalDriveInNTWithAdminRights(void)
{
   int done = FALSE;
   int drive = 0;

   for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
   {
      HANDLE hPhysicalDriveIOCTL = 0;

         //  Try to get a handle to PhysicalDrive IOCTL, report failure
         //  and exit if can't.
      char driveName [256];

      sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);

         //  Windows NT, Windows 2000, must have admin rights
      hPhysicalDriveIOCTL = CreateFile (driveName,
                               GENERIC_READ | GENERIC_WRITE, 
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, 0, NULL);
      // if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
      //    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
      //            drive, GetLastError ());

      if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
      {
         GETVERSIONOUTPARAMS VersionParams;
         DWORD               cbBytesReturned = 0;

            // Get the version, etc of PhysicalDrive IOCTL
         memset ((void*) &VersionParams, 0, sizeof(VersionParams));

         if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
                   NULL, 
                   0,
                   &VersionParams,
                   sizeof(VersionParams),
                   &cbBytesReturned, NULL) )
         {         
            // printf ("DFP_GET_VERSION failed for drive %d\n", i);
            // continue;
         }

            // If there is a IDE device at number "i" issue commands
            // to the device
         if (VersionParams.bIDEDeviceMap > 0)
         {
            BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
            SENDCMDINPARAMS  scip;
            //SENDCMDOUTPARAMS OutCmd;

			// Now, get the ID sector for all IDE devices in the system.
               // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
               // otherwise use the IDE_ATA_IDENTIFY command
            bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
                      IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

            memset (&scip, 0, sizeof(scip));
            memset (IdOutCmd, 0, sizeof(IdOutCmd));

            if ( nDoIDENTIFY (hPhysicalDriveIOCTL, 
                       &scip, 
                       (PSENDCMDOUTPARAMS)&IdOutCmd, 
                       (BYTE) bIDCmd,
                       (BYTE) drive,
                       &cbBytesReturned))
            {
               DWORD diskdata [256];
               int ijk = 0;
               USHORT *pIdSector = (USHORT *)
                             ((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;

               for (ijk = 0; ijk < 256; ijk++)
                  diskdata [ijk] = pIdSector [ijk];

               vPrintIdeInfo (drive, diskdata);

               done = TRUE;
            }
	    }

         CloseHandle (hPhysicalDriveIOCTL);
      }
   }

   return done;
}

//////////////////////////////////////////////////////
//
//	pflipAndCodeBytes
//
//  function to decode the serial numbers of IDE hard drives
//  using the IOCTL_STORAGE_QUERY_PROPERTY command 
//////////////////////////////////////////////////////
char * CDriveInfo::pflipAndCodeBytes (char * str)
{
	static char flipped [1000];
	int num = strlen (str);
	strcpy (flipped, "");
	for (int i = 0; i < num; i += 4)
	{
		for (int j = 1; j >= 0; j--)
		{
			int sum = 0;
			for (int k = 0; k < 2; k++)
			{
				sum *= 16;
				switch (str [i + j * 2 + k])
				{
				case '0': sum += 0; break;
				case '1': sum += 1; break;
				case '2': sum += 2; break;
				case '3': sum += 3; break;
				case '4': sum += 4; break;
				case '5': sum += 5; break;
				case '6': sum += 6; break;
				case '7': sum += 7; break;
				case '8': sum += 8; break;
				case '9': sum += 9; break;
				case 'a': sum += 10; break;
				case 'b': sum += 11; break;
				case 'c': sum += 12; break;
				case 'd': sum += 13; break;
				case 'e': sum += 14; break;
				case 'f': sum += 15; break;
				}
			}
			if (sum > 0) 
			{
				char sub [2];
				sub [0] = (char) sum;
				sub [1] = 0;
				strcat (flipped, sub);
			}
		}
	}

	return flipped;
}

//////////////////////////////////////////////////////
//
//	nReadPhysicalDriveInNTWithZeroRights
//
//////////////////////////////////////////////////////
int CDriveInfo::nReadPhysicalDriveInNTWithZeroRights(void)
{
   int done = FALSE;
   int drive = 0;

   for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
   {
      HANDLE hPhysicalDriveIOCTL = 0;

         //  Try to get a handle to PhysicalDrive IOCTL, report failure
         //  and exit if can't.
      char driveName [256];

      sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);

         //  Windows NT, Windows 2000, Windows XP - admin rights not required
      hPhysicalDriveIOCTL = CreateFile (driveName, 0,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, 0, NULL);
      // if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
      //    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
      //            drive, GetLastError ());

      if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
      {
		 STORAGE_PROPERTY_QUERY query;
         DWORD cbBytesReturned = 0;
		 char buffer [10000];

         memset ((void *) & query, 0, sizeof (query));
		 query.PropertyId = StorageDeviceProperty;
		 query.QueryType = PropertyStandardQuery;

		 memset (buffer, 0, sizeof (buffer));

         if ( DeviceIoControl (hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY,
                   & query,
                   sizeof (query),
				   & buffer,
				   sizeof (buffer),
                   & cbBytesReturned, NULL) )
         {         
			 STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *) & buffer;
			 char serialNumber [1000];

			 strcpy (serialNumber, 
					pflipAndCodeBytes ( & buffer [descrip -> SerialNumberOffset]));

					poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,"\n**** STORAGE_DEVICE_DESCRIPTOR for drive %d ****\n"
				     "Vendor Id = %s\n"
					 "Product Id = %s\n"
					 "Product Revision = %s\n"
					 "Serial Number = %s\n",
					 drive,
					 & buffer [descrip -> VendorIdOffset],
					 & buffer [descrip -> ProductIdOffset],
					 & buffer [descrip -> ProductRevisionOffset],
					 serialNumber);

					CDriveObj *poDrive = new CDriveObj(poDebug);
					poDrive->poSerialNum->Format("%s",serialNumber);

					this->poDriveArray->nAdd(poDrive);
         }
		 else
		 {
			 DWORD err = GetLastError ();
			poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO
				,"\nDeviceIOControl IOCTL_STORAGE_QUERY_PROPERTY error = %d\n", err);

		 }

		 memset (buffer, 0, sizeof (buffer));

         if ( DeviceIoControl (hPhysicalDriveIOCTL, IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER,
                   NULL,
                   0,
				   & buffer,
				   sizeof (buffer),
                   & cbBytesReturned, NULL) )
         {         
			 MEDIA_SERIAL_NUMBER_DATA * mediaSerialNumber = 
							(MEDIA_SERIAL_NUMBER_DATA *) & buffer;
			 char serialNumber [1000];

			 strcpy (serialNumber, (char *) mediaSerialNumber -> SerialNumberData);

			 poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
				 "\n**** MEDIA_SERIAL_NUMBER_DATA for drive %d ****\n"
					 "Serial Number = %s\n",
					 drive, serialNumber);

		 }
		 else
		 {
			 DWORD err = GetLastError ();

			 switch (err)
			 {
			 case 1: 
				 poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
					 "\nDeviceIOControl IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER error = \n"
							"              The request is not valid for this device.\n\n");
				 break;
			 case 50:
				poDebug->vPrintf(DEBUG_LEVEL1,
					DEBUG_INFO,"\nDeviceIOControl IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER error = \n"
							"              The request is not supported for this device.\n\n");
				 break;
			 default:
				 poDebug->vPrintf(DEBUG_LEVEL1,
					 DEBUG_INFO,"\nDeviceIOControl IOCTL_STORAGE_GET_MEDIA_SERIAL_NUMBER error = %d\n\n", err);
			 }

		 }

         CloseHandle (hPhysicalDriveIOCTL);
      }
   }

   return done;
}

//////////////////////////////////////////////////////
//
//	nReadPhysicalDriveInNTWithZeroRights
// FUNCTION: Send an IDENTIFY command to the drive
// bDriveNum = 0-3
// bIDCmd = IDE_ATA_IDENTIFY or IDE_ATAPI_IDENTIFY
//////////////////////////////////////////////////////
BOOL CDriveInfo::nDoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                 PDWORD lpcbBytesReturned)
{
      // Set up data structures for IDENTIFY command.
   pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
   pSCIP -> irDriveRegs.bFeaturesReg = 0;
   pSCIP -> irDriveRegs.bSectorCountReg = 1;
   pSCIP -> irDriveRegs.bSectorNumberReg = 1;
   pSCIP -> irDriveRegs.bCylLowReg = 0;
   pSCIP -> irDriveRegs.bCylHighReg = 0;

      // Compute the drive number.
   pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

      // The command can either be IDE identify or ATAPI identify.
   pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
   pSCIP -> bDriveNumber = bDriveNum;
   pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

   return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
               (LPVOID) pSCIP,
               sizeof(SENDCMDINPARAMS) - 1,
               (LPVOID) pSCOP,
               sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
               lpcbBytesReturned, NULL) );
}

//////////////////////////////////////////////////////
//
//	nReadDrivePortsInWin9X
//
//////////////////////////////////////////////////////
int CDriveInfo::nReadDrivePortsInWin9X(void)
{
	int done = FALSE;

   HANDLE VxDHandle = 0;
   pt_IdeDInfo pOutBufVxD = 0;
   DWORD lpBytesReturned = 0;

		//  set the thread priority high so that we get exclusive access to the disk
   BOOL status =
		// SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		SetPriorityClass (GetCurrentProcess (), REALTIME_PRIORITY_CLASS);
		// SetPriorityClass (GetCurrentProcess (), HIGH_PRIORITY_CLASS);

   if (0 == status) 
   {
	   // printf ("\nERROR: Could not SetThreadPriority, LastError: %d\n", GetLastError ());
	   poDebug->vPrintf(DEBUG_LEVEL1,DEBUG_INFO,
	   "\nERROR: Could not SetPriorityClass, LastError: %d\n", GetLastError ());
   }

      // 1. Make an output buffer for the VxD
   rt_IdeDInfo info;
   pOutBufVxD = &info;

      // *****************
      // KLUDGE WARNING!!!
      // HAVE to zero out the buffer space for the IDE information!
      // If this is NOT done then garbage could be in the memory
      // locations indicating if a disk exists or not.
   ZeroMemory (&info, sizeof(info));

      // 1. Try to load the VxD
       //  must use the short file name path to open a VXD file
   //char StartupDirectory [2048];
   //char shortFileNamePath [2048];
   //char *p = NULL;
   //char vxd [2048];
      //  get the directory that the exe was started from
   //GetModuleFileName (hInst, (LPSTR) StartupDirectory, sizeof (StartupDirectory));
      //  cut the exe name from string
   //p = &(StartupDirectory [strlen (StartupDirectory) - 1]);
   //while (p >= StartupDirectory && *p && '\\' != *p) p--;
   //*p = '\0';   
   //GetShortPathName (StartupDirectory, shortFileNamePath, 2048);
   //sprintf (vxd, "\\\\.\\%s\\IDE21201.VXD", shortFileNamePath);
   //VxDHandle = CreateFile (vxd, 0, 0, 0,
   //               0, FILE_FLAG_DELETE_ON_CLOSE, 0);   
   VxDHandle = CreateFile ("\\\\.\\IDE21201.VXD", 0, 0, 0,
							0, FILE_FLAG_DELETE_ON_CLOSE, 0);

   if (VxDHandle != INVALID_HANDLE_VALUE)
   {
         // 2. Run VxD function
      DeviceIoControl (VxDHandle, m_cVxDFunctionIdesDInfo,
					0, 0, pOutBufVxD, sizeof(pt_IdeDInfo), &lpBytesReturned, 0);

         // 3. Unload VxD
      CloseHandle (VxDHandle);
   }
   else
		MessageBox (NULL, "ERROR: Could not open IDE21201.VXD file", 
					"ERROR", MB_ICONSTOP);

      // 4. Translate and store data
   unsigned long int i = 0;
   for (i=0; i<8; i++)
   {
      if((pOutBufVxD->DiskExists[i]) && (pOutBufVxD->IDEExists[i/2]))
      {
			DWORD diskinfo [256];
			for (int j = 0; j < 256; j++) 
				diskinfo [j] = pOutBufVxD -> DisksRawInfo [i * 256 + j];

            // process the information for this buffer
		   vPrintIdeInfo (i, diskinfo);
			done = TRUE;
      }
   }

		//  reset the thread priority back to normal
   // SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_NORMAL);
   SetPriorityClass (GetCurrentProcess (), NORMAL_PRIORITY_CLASS);

   return done;
}

//////////////////////////////////////////////////////
//
//	nReadIdeDriveAsScsiDriveInNT
//
//////////////////////////////////////////////////////
int CDriveInfo::nReadIdeDriveAsScsiDriveInNT(void)
{
   int done = FALSE;
   int controller = 0;

   for (controller = 0; controller < 16; controller++)
   {
      HANDLE hScsiDriveIOCTL = 0;
      char   driveName [256];

         //  Try to get a handle to PhysicalDrive IOCTL, report failure
         //  and exit if can't.
      sprintf (driveName, "\\\\.\\Scsi%d:", controller);

         //  Windows NT, Windows 2000, any rights should do
      hScsiDriveIOCTL = CreateFile (driveName,
                               GENERIC_READ | GENERIC_WRITE, 
                               FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, 0, NULL);
      // if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
      //    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
      //            controller, GetLastError ());

      if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
      {
         int drive = 0;

         for (drive = 0; drive < 2; drive++)
         {
            char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
            SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
            SENDCMDINPARAMS *pin =
                   (SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
            DWORD dummy;
   
            memset (buffer, 0, sizeof (buffer));
            p -> HeaderLength = sizeof (SRB_IO_CONTROL);
            p -> Timeout = 10000;
            p -> Length = SENDIDLENGTH;
            p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
            strncpy ((char *) p -> Signature, "SCSIDISK", 8);
  
            pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
            pin -> bDriveNumber = drive;

            if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
                                 buffer,
                                 sizeof (SRB_IO_CONTROL) +
                                         sizeof (SENDCMDINPARAMS) - 1,
                                 buffer,
                                 sizeof (SRB_IO_CONTROL) + SENDIDLENGTH,
                                 &dummy, NULL))
            {
               SENDCMDOUTPARAMS *pOut =
                    (SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
               IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);
               if (pId -> sModelNumber [0])
               {
                  DWORD diskdata [256];
                  int ijk = 0;
                  USHORT *pIdSector = (USHORT *) pId;
          
                  for (ijk = 0; ijk < 256; ijk++)
                     diskdata [ijk] = pIdSector [ijk];

                  vPrintIdeInfo (controller * 2 + drive, diskdata);

                  done = TRUE;
               }
            }
         }
         CloseHandle (hScsiDriveIOCTL);
      }
   }

   return done;
}

//////////////////////////////////////////////////////
//
//	vPrintIdeInfo
//
//////////////////////////////////////////////////////
void CDriveInfo::vPrintIdeInfo(int drive, DWORD diskdata [256])
{
	CDriveObj *poDrive = new CDriveObj(poDebug);
   char serialnumber [1024];
   __int64 sectors = 0;
   __int64 bytes = 0;

    //copy the hard drive serial number to the buffer
	strcpy (serialnumber, pConvertToString (diskdata, 10, 19));
	poDrive->poSerialNum->Format("%s",serialnumber);
	poDrive->nHDID = this->nConvertID(serialnumber);

   switch (drive / 2)
   {
      case 0:
		  {
			  poDrive->poControllerType->Format("Primary Controller");
			  poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"\nPrimary Controller - ");
		  }
			  break;
      case 1:
		  {
			  poDrive->poControllerType->Format("Secondary Controller");
			  poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"\nSecondary Controller - ");
		  }
			  break;
      case 2:
		  {
			  poDrive->poControllerType->Format("Tertiary Controller");
			  poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"\nTertiary Controller - ");
		  }
			  break;
      case 3:
		  {
			  poDrive->poControllerType->Format("Quaternary Controller");
			  poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"\nQuaternary Controller - ");
		  }
			  break;
   }

   switch (drive % 2)
   {
      case 0: 
		  {
			  poDrive->poDriveType->Format("Master drive");
			  poDrive->nMaster = 1;
			  poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Master drive\n\n");
		  }
			  break;
      case 1:
		  {
			  poDrive->poDriveType->Format("Slave drive");
			   poDrive->nMaster = 0;
			  poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Slave drive\n\n");
		  }
			  break;
   }

   poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Drive Model Number________________: %s\n",
           pConvertToString (diskdata, 27, 46));
 
   poDrive->poDriveModel->Format("%s",pConvertToString (diskdata, 27, 46));

   poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Drive Controller Revision Number__: %s\n",
           pConvertToString (diskdata, 23, 26));

   poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Controller Buffer Size on Drive___: %u bytes\n",
           diskdata [21] * 512);

   poDebug->vPrintf(DEBUG_LEVEL2,DEBUG_INFO,"Drive Type________________________: ");
   if (diskdata [0] & 0x0080)
   {
      printf ("Removable\n");
   }

   else if (diskdata [0] & 0x0040)
   {
      poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO,"Fixed\n");
	  poDrive->nFixed = 1;
   }
   else
   {
	   poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO,"Unknown\n");
	   poDrive->nFixed = 0;
   }

		//  calculate size based on 28 bit or 48 bit addressing
		//  48 bit addressing is reflected by bit 10 of word 83
	if (diskdata [83] & 0x400) 
	{
		sectors = diskdata [103] * 65536I64 * 65536I64 * 65536I64 + 
					diskdata [102] * 65536I64 * 65536I64 + 
					diskdata [101] * 65536I64 + 
					diskdata [100];
	}
	else
	{
		sectors = diskdata [61] * 65536 + diskdata [60];
	}
		//  there are 512 bytes in a sector
	bytes = sectors * 512;

	poDrive->nSize = (int)(bytes / 1000000000);

	poDebug->vPrintf(DEBUG_LEVEL3,DEBUG_INFO,"Drive Size________________________: %I64d bytes\n",
			bytes);

	this->poDriveArray->nAdd(poDrive);
}

//////////////////////////////////////////////////////
//
//	pConvertToString
//
//////////////////////////////////////////////////////
char *CDriveInfo::pConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex)
{
   static char string [1024];
   int index = 0;
   int position = 0;

      //  each integer has two characters stored in it backwards
   for (index = firstIndex; index <= lastIndex; index++)
   {
         //  get high byte for 1st character
      string [position] = (char) (diskdata [index] / 256);
      position++;

         //  get low byte for 2nd character
      string [position] = (char) (diskdata [index] % 256);
      position++;
   }

      //  end the string 
   string [position] = '\0';

      //  cut off the trailing blanks
   for (index = position - 1; index > 0 && ' ' == string [index]; index--)
      string [index] = '\0';

   return string;
}

//////////////////////////////////////////////////////
//
//	nGetHardDriveComputerID
//
//////////////////////////////////////////////////////
long CDriveInfo::nGetHardDriveComputerID(void)
{
   int done = FALSE;
   // char string [1024];
   __int64 id = 0;
   OSVERSIONINFO version;


   memset (&version, 0, sizeof (version));
   version.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
   GetVersionEx (&version);
   if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
	
		//Trying to read the drive IDs using physical access with admin rights
		done = nReadPhysicalDriveInNTWithAdminRights ();

		//Trying to read the drive IDs using the SCSI back door
		if ( ! done)
		{
			done = nReadIdeDriveAsScsiDriveInNT ();
		}

		//nTrying to read the drive IDs using physical access with zero rights
		if ( ! done)
		{
			done = nReadPhysicalDriveInNTWithZeroRights ();
		}

   }
   else
   {
         //  this works under Win9X and calls a VXD
      int attempt = 0;

         //  try this up to 10 times to get a hard drive serial number
		for(int i=0; i<10; i++)
		{
         done = nReadDrivePortsInWin9X ();
		}
   }

return done;
}
//////////////////////////////////////////////////////
//
//	nConvertID
//
//////////////////////////////////////////////////////
unsigned long CDriveInfo::nConvertID(char *hdid)
{
       char *p = hdid;
		unsigned long id = 0;

      //WriteConstantString ("HardDriveSerialNumber", HardDriveSerialNumber);

         //  ignore first 5 characters from western digital hard drives if
         //  the first four characters are WD-W
      if ( ! strncmp (hdid, "WD-W", 4)) p += 5;
      for ( ; p && *p; p++)
      {
         if ('-' == *p) continue;
         id *= 10;
         switch (*p)
         {
            case '0': id += 0; break;
            case '1': id += 1; break;
            case '2': id += 2; break;
            case '3': id += 3; break;
            case '4': id += 4; break;
            case '5': id += 5; break;
            case '6': id += 6; break;
            case '7': id += 7; break;
            case '8': id += 8; break;
            case '9': id += 9; break;
            case 'a': case 'A': id += 10; break;
            case 'b': case 'B': id += 11; break;
            case 'c': case 'C': id += 12; break;
            case 'd': case 'D': id += 13; break;
            case 'e': case 'E': id += 14; break;
            case 'f': case 'F': id += 15; break;
            case 'g': case 'G': id += 16; break;
            case 'h': case 'H': id += 17; break;
            case 'i': case 'I': id += 18; break;
            case 'j': case 'J': id += 19; break;
            case 'k': case 'K': id += 20; break;
            case 'l': case 'L': id += 21; break;
            case 'm': case 'M': id += 22; break;
            case 'n': case 'N': id += 23; break;
            case 'o': case 'O': id += 24; break;
            case 'p': case 'P': id += 25; break;
            case 'q': case 'Q': id += 26; break;
            case 'r': case 'R': id += 27; break;
            case 's': case 'S': id += 28; break;
            case 't': case 'T': id += 29; break;
            case 'u': case 'U': id += 30; break;
            case 'v': case 'V': id += 31; break;
            case 'w': case 'W': id += 32; break;
            case 'x': case 'X': id += 33; break;
            case 'y': case 'Y': id += 34; break;
            case 'z': case 'Z': id += 35; break;
         }                            
      }
   

return id;
}