/*------------------------------------

SDISK II LCD Firmware

2010.11.11 by Koichi Nishida
2012.01.26 by Fábio Belavenuto
2015.07.02 by Alexandre Suaide

-------------------------------------
*/

/*
2015.07.02 by Alexandre Suaide
Added support for SDHC cards and subdirectories
Removed DSK to NIC conversion
FAT16 and FAT32 disks should have at least 64 blocks per cluster
*/

/*
2012.01.26 by Fábio Belavenuto
Added support for image exchange using a button added in the Brazilian version by Victor Trucco
Added support for a 16x2 LCD
*/

/*
This is a part of the firmware for DISK II emulator by Nishida Radio.

Copyright (C) 2010 Koichi NISHIDA
email to Koichi NISHIDA: tulip-house@msf.biglobe.ne.jp

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"

#ifndef _FAT32_H_
#define _FAT32_H_


//Structure to access Master Boot Record for getting info about partioions
struct MBRinfo_Structure{
	unsigned char   nothing[446];		//ignore, placed here to fill the gap in the structure
	unsigned char   partitionData[64];	//partition records (16x4)
	unsigned int    signature;		//0xaa55
};

//Structure to access info of the first partioion of the disk
struct partitionInfo_Structure{
	unsigned char   status;			//0x80 - active partition
	unsigned char   headStart;		//starting head
	unsigned int    cylSectStart;		//starting cylinder and sector
	unsigned char   type;			//partition type
	unsigned char   headEnd;		//ending head of the partition
	unsigned int    cylSectEnd;		//ending cylinder and sector
	unsigned long   firstSector;		//total sectors between MBR & the first sector of the partition
	unsigned long   sectorsTotal;		//size of this partition in sectors
};

//Structure to access boot sector data
struct BS_Structure_1{  // index 0
	unsigned char   jumpBoot[3]; //default: 0x009000EB
	unsigned char   OEMName[8];
	unsigned int    bytesPerSector; //deafault: 512
	unsigned char   sectorPerCluster;
	unsigned int    reservedSectorCount;
	unsigned char   numberOfFATCopies;
	unsigned int    numberOfRootEntries;
	unsigned int    smallNumberOfSectors; //must be 0 for FAT32
	unsigned char   mediaType;
	unsigned int    sectorsPerFAT16; //must be 0 for FAT32
	unsigned int    sectorsPerTrack;
	unsigned int    numberofHeads;
	unsigned long   numberOfHiddenSectors;
	unsigned long   largeNumberOfSectors;
};

struct BS_Structure_2{  // index + 024h (FAT32)
	unsigned long   sectorsPerFAT32; //count of sectors occupied by one FAT
	unsigned int    flags;
	unsigned int    versionOfFAT32; //0x0000 (defines version 0.0)
	unsigned long   clusterOfRootDirectory; //first cluster of root directory (=2)
	unsigned int    sectorNumberOfFileSystemInfo; //sector number of FSinfo structure (=1)
	unsigned int    sectorNumberOfBackupSector;
	unsigned char   reserved[12];
};

//Structure to access FSinfo sector data
struct FSInfo_Structure
{
	unsigned long   leadSignature; //0x41615252
	unsigned char   reserved1[480];
	unsigned long   structureSignature; //0x61417272
	unsigned long   freeClusterCount; //initial: 0xffffffff
	unsigned long   nextFreeCluster; //initial: 0xffffffff
	unsigned char   reserved2[12];
	unsigned long   trailSignature; //0xaa550000
};

//Structure to access Directory Entry in the FAT
struct dir_Structure {
	unsigned char   name[11];
	unsigned char   attrib; //file attributes
	unsigned char   NTreserved; //always 0
	unsigned char   timeTenth; //tenths of seconds, set to 0 here
	unsigned int    createTime; //time file was created
	unsigned int    createDate; //date file was created
	unsigned int    lastAccessDate;
	unsigned int    firstClusterHI; //higher word of the first cluster number
	unsigned int    writeTime; //time of last write
	unsigned int    writeDate; //date of last write
	unsigned int    firstClusterLO; //lower word of the first cluster number
	unsigned long   fileSize; //size of file in bytes
};

#define PARTITION_TYPE_FAT16 0x06
#define PARTITION_TYPE_FAT32 0x0b

extern unsigned long    FAT_offset;
extern unsigned long    FAT_firstDataSector;
extern unsigned char    FAT_partitionType;
extern unsigned int     FAT_bytesPerSector;
extern unsigned char    FAT_sectorsPerCluster;
extern unsigned long    FAT_sectorOfRootDirectory;
extern unsigned long    FAT_sectorOfCurrentDirectory;
//extern unsigned int     FAT_sectorsPerFAT16;
extern unsigned char    FAT_sectorsPerClusterBitShift;

unsigned char           FAT_init(void);
struct dir_Structure*   getFile(unsigned int index);
struct dir_Structure*   anyFile(unsigned int index);
struct dir_Structure*   validFile(struct dir_Structure* file);
unsigned char           cd(struct dir_Structure* dir);
unsigned long           getNextCluster(unsigned long clusterNumber);
unsigned long           allocEmptyCluster();

static inline unsigned long    getSector(unsigned long cluster) { return (((cluster-2)*FAT_sectorsPerCluster)+FAT_firstDataSector);}
static inline unsigned long    getCluster(unsigned long sector) { return (sector-FAT_firstDataSector)/FAT_sectorsPerCluster + 2; }

#endif

