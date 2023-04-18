#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "include/fs.h"

typedef int (*dfunc)(int);

unsigned short
get_le_short(void *from)
{
	unsigned char *p = from;
	return ((unsigned short)(p[1]) << 8) +
		(unsigned short)p[0];
}

unsigned int get_le_long(void *from)
{
	unsigned char *p = from;
	return ((unsigned int)(p[3]) << 24) +
		((unsigned int)(p[2]) << 16) +
		((unsigned int)(p[1]) << 8) +
		(unsigned int)p[0];
}

unsigned short get_be_short(void *from)
{
	unsigned char *p = from;
	return ((unsigned short)(p[0]) << 8) +
		(unsigned short)p[1];
}

unsigned int get_be_long(void *from)
{
	unsigned char *p = from;
	return ((unsigned int)(p[0]) << 24) +
		((unsigned int)(p[1]) << 16) +
		((unsigned int)(p[2]) << 8) +
		(unsigned int)p[3];
}

int get_buffer(int fd, unsigned char *b, int offset, int len)
{
	if(lseek(fd, offset, SEEK_SET) != offset)
		return -1;
	if(read(fd, b, len) != len)
		return -1;
	return 0;
}

#define MBR_BUF_SIZE	512
int detect_mbr(int fd)
{
	int ret = NONE;
	unsigned char *buffer = (unsigned char*)malloc(MBR_BUF_SIZE);
	if(get_buffer(fd, buffer, 0, MBR_BUF_SIZE) != 0)
		goto out;
	if((buffer[510] == 0x55) && (buffer[511] == 0xAA))
		ret = MBR;
out:
	free(buffer);
	return ret;
}

#define EFI_BUF_OFFSET	512
#define EFI_BUF_SIZE	512
int detect_efi(int fd)
{
	int ret = NONE;
	unsigned char *buffer = (unsigned char*)malloc(EFI_BUF_SIZE);
	if(get_buffer(fd, buffer, EFI_BUF_OFFSET, EFI_BUF_SIZE) != 0)
		goto out;
	if(!memcmp(buffer, "EFI PART", 8))
		ret = EFI;
out:
	free(buffer);
	return ret;
}

#define EXT2_BUF_SIZE	1024
int detect_ext23(int fd)
{
	int ret = NONE;
	unsigned char *buffer = (unsigned char*)malloc(EXT2_BUF_SIZE);
	if(get_buffer(fd, buffer, 1024, EXT2_BUF_SIZE) != 0)
		goto out;
	if(get_le_short(buffer + 56) == 0xEF53)
	{
		if(get_le_long(buffer + 92) & 0x0004)
		{
			if ((get_le_long(buffer + 96) < 0x0000040)
				&& (get_le_long(buffer + 100) < 0x0000008))
				ret = EXT3;
			else
				ret = EXT4;
		}
		else
			ret = EXT2;
	}
out:
	free(buffer);
	return ret;
}

#define FAT_BUF_SIZE	512
int detect_fat(int fd)
{
	int ret = NONE;
	unsigned char *buffer = (unsigned char*)malloc(FAT_BUF_SIZE);
	if(get_buffer(fd, buffer, 0, FAT_BUF_SIZE) != 0)
		goto out;

	if (((((buffer[0] & 0xff) == 0xEB) && ((buffer[2] & 0xff) == 0x90)) || ((buffer[0] & 0xff) == 0xE9))
		&& ((buffer[510] & 0xff) == 0x55) /*&& ((buffer[511] & 0xff == 0xAA))*/
		&& (memcmp(buffer + 3, "NTFS    ", 8))
		&& (memcmp(buffer + 3, "EXFAT   ", 8)))
		ret = FAT;
out:
	free(buffer);
	return ret;
}

#define HFSPLUS_VOL_JOURNALED	(1 << 13)
#define HFSPLUS_BUF_SIZE			512
int detect_hfsplus(int fd)
{
	int ret = NONE;
	unsigned short magic;
	unsigned int journal;
	unsigned char *buffer = (unsigned char*)malloc(HFSPLUS_BUF_SIZE);
	if(get_buffer(fd, buffer, 1024, HFSPLUS_BUF_SIZE) != 0)
		goto out;
	magic = get_be_short(buffer);
	journal = get_be_long(buffer + 4) & HFSPLUS_VOL_JOURNALED;
	if(magic == 0x482B)
	{
		if(!journal)
			ret = HFSPLUS;
	//	else
	//		ret = HFSPLUSJOURNAL;
	}
out:
	free(buffer);
	return ret;
}

#define NTFS_BUF_SIZE	512
int detect_ntfs(int fd)
{
	int ret = NONE;
	unsigned char *buffer = (unsigned char*)malloc(NTFS_BUF_SIZE);
	if(get_buffer(fd, buffer, 0, NTFS_BUF_SIZE) != 0)
		goto out;
	if(!memcmp(buffer + 3, "NTFS    ", 8))
		ret = NTFS;
out:
	free(buffer);
	return ret;
}

#define EXTENDED_BUF_SIZE	512
int detect_extended(int fd)
{
	int ret = NONE;
	unsigned char *buffer = (unsigned char*)malloc(MBR_BUF_SIZE);
	if(get_buffer(fd, buffer, 0, 512) != 0)
		goto out;
	if((((buffer[0] & 0xff) == 0xEB) && ((buffer[2] & 0xff) == 0x90)) || ((buffer[0] & 0xff) == 0xE9))
		goto out;
	if(((buffer[510] & 0xff) == 0x55) && ((buffer[511] & 0xff) == 0xAA))
		ret = EXTENDED;
out:
	free(buffer);
	return ret;
}

#define EXFAT_BUF_SIZE	512
int detect_exfat(int fd)
{
	int ret = NONE;
	unsigned char *buffer = (unsigned char*)malloc(EXFAT_BUF_SIZE);
	if(get_buffer(fd, buffer, 0, EXFAT_BUF_SIZE) != 0)
		goto out;

	if (((buffer[0] & 0xff) == 0xEB) && ((buffer[2] & 0xff) == 0x90)
		&& ((buffer[510] & 0xff) == 0x55) && ((buffer[511] & 0xff) == 0xAA)
		&& (!memcmp(buffer + 3, "EXFAT   ", 8)))
		ret = EXFAT;
out:
	free(buffer);
	return ret;
}

dfunc funcs[] = {
	detect_ext23,
	detect_fat,
	detect_ntfs,
	detect_exfat,
	detect_hfsplus,
	detect_extended,
	detect_efi,
	detect_mbr,
};

int detect_fs(char *device)
{
	int i = 0;
	int ret = NONE;
	int fd;

	fd = open(device, O_RDONLY);
	if(!fd)
		return NONE;

	while((i < 7) && (ret == NONE))
		ret = funcs[i++](fd);

	close(fd);

	return ret;
}
