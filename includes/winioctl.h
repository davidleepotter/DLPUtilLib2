#ifndef WINIOCTL_H
#define WINIOCTL_H

// winioctl.h stub for Linux/GCC builds
// Provides IOCTL constants for disk operations

#include <sys/ioctl.h>
#include <linux/fs.h>

// IOCTL constants
#define IOCTL_DISK_GET_PARTITION_INFO 0x7404
#define IOCTL_DISK_GET_DRIVE_GEOMETRY 0x7000
#define IOCTL_DISK_GET_DRIVE_INFO 0x7000

// Disk geometry structure
typedef struct _DISK_GEOMETRY {
    LARGE_INTEGER Cylinders;
    BYTE MediaType;
    DWORD TracksPerCylinder;
    DWORD SectorsPerTrack;
    DWORD BytesPerSector;
} DISK_GEOMETRY, *PIDSK_GEOMETRY;

// Partition information structure
typedef struct _PARTITION_INFORMATION {
    LARGE_INTEGER StartingOffset;
    LARGE_INTEGER PartitionLength;
    DWORD HiddenSectors;
    DWORD PartitionNumber;
    BYTE PartitionType;
    BYTE BootIndicator;
    BYTE RecognizedPartition;
    BYTE RewritePartition;
} PARTITION_INFORMATION, *PPARTITION_INFORMATION;

#endif // WINIOCTL_H
