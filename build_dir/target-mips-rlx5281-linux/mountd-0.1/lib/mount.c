#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <scsi/sg.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glob.h>
#include <libgen.h>
#include <poll.h>

#include "include/log.h"
#include "include/list.h"
#include "include/sys.h"
#include "include/signal.h"
#include "include/timer.h"
#include "include/autofs.h"
#include "include/ucix.h"
#include "include/fs.h"

int mount_new(char *path, char *dev);

struct list_head mounts;

struct mount {
	struct list_head list;
	char name[64];
	char dev[64];
	char serial[64];
	char vendor[64];
	char model[64];
	char rev[64];
	int mounted;
	int ignore;
	char size[64];
	char sector_size[64];
	int fs;
};

char *fs_names[] = {
	"",
	"",
	"MBR",
	"EXT2",
	"EXT3",
	"FAT",
	"HFSPLUS",
	"",
	"NTFS",
	"",
	"EXT4",
	"EXFAT"
};

#define MAX_MOUNTED		32
#define MAX_MOUNT_NAME	32

char mounted[MAX_MOUNTED][3][MAX_MOUNT_NAME];
int mounted_count = 0;
extern char uci_path[32];

void mount_dump_uci_state(void)
{
	struct uci_context *ctx;
	struct list_head *p;
	char mountd[] = {"mountd"};
	char type[] = {"mountd_disc"};
	int mounted = 0;
	unsigned long long int size = 0;
	unlink("/var/state/mountd");
	ctx = ucix_init("mountd");
	uci_set_savedir(ctx, "/var/state/");
	ucix_add_option_int(ctx, mountd, mountd, "count", list_count(&mounts));
	list_for_each(p, &mounts)
	{
		struct mount *q = container_of(p, struct mount, list);
		char t[64];
		if(q->fs == EXTENDED)
			continue;
		ucix_add_section(ctx, mountd, q->serial, type);
		strcpy(t, q->dev);
		t[3] = '\0';
		ucix_add_option(ctx, mountd, q->serial, "disc", t);
		ucix_add_option(ctx, mountd, q->serial, "sector_size", q->sector_size);
		snprintf(t, 64, "part%dmounted", atoi(&q->dev[3]));
		ucix_add_option(ctx, mountd, q->serial, t, (q->mounted)?("1"):("0"));
		ucix_add_option(ctx, mountd, q->serial, "vendor", q->vendor);
		ucix_add_option(ctx, mountd, q->serial, "model", q->model);
		ucix_add_option(ctx, mountd, q->serial, "rev", q->rev);
		snprintf(t, 64, "size%d", atoi(&q->dev[3]));
		ucix_add_option(ctx, mountd, q->serial, t, q->size);
		if(q->fs > MBR && q->fs <= EXT4)
		{
			snprintf(t, 64, "fs%d", atoi(&q->dev[3]));
			ucix_add_option(ctx, mountd, q->serial, t, fs_names[q->fs]);
		}
		if(q->mounted)
			mounted++;
		if((!q->ignore) && q->size && q->sector_size)
			size = size + (((unsigned long long int)atoi(q->size)) * ((unsigned long long int)atoi(q->sector_size)));
	}
	ucix_add_option_int(ctx, mountd, mountd, "mounted", mounted);
	ucix_add_option_int(ctx, mountd, mountd, "total", size);
	system_printf("echo -n %llu > /tmp/run/mountd_size", size);
	ucix_save_state(ctx, "mountd");
	ucix_cleanup(ctx);
}

struct mount* mount_find(char *name, char *dev)
{
	struct list_head *p;
	list_for_each(p, &mounts)
	{
		struct mount *q = container_of(p, struct mount, list);
		if(name)
			if(!strcmp(q->name, name))
				return q;
		if(dev)
			if(!strcmp(q->dev, dev))
				return q;
	}
	return 0;
}

void mount_add_list(char *name, char *dev, char *serial,
	char *vendor, char *model, char *rev, int ignore, char *size, char *sector_size, int fs)
{
	struct mount *mount;
	char tmp[64], tmp2[64];
	if(fs <= MBR || fs > EXFAT)
		return;
	mount  = malloc(sizeof(struct mount));
	INIT_LIST_HEAD(&mount->list);
	strncpy(mount->vendor, vendor, 64);
	strncpy(mount->model, model, 64);
	strncpy(mount->rev, rev, 64);
	strncpy(mount->name, name, 64);
	strncpy(mount->dev, dev, 64);
	strncpy(mount->serial, serial, 64);
	strncpy(mount->size, size, 64);
	strncpy(mount->sector_size, sector_size, 64);
	mount->ignore = ignore;
	mount->mounted = 0;
	mount->fs = fs;
	list_add(&mount->list, &mounts);
	if((!mount->ignore) && (mount->fs > MBR) && (mount->fs <= EXFAT))
	{
		log_printf("new mount : %s -> %s (%s)\n", name, dev, fs_names[mount->fs]);
		snprintf(tmp, 64, "%s%s", uci_path, name);
		snprintf(tmp2, 64, "/tmp/run/mountd/%s", dev);
		symlink(tmp2, tmp);
		mount_new("/tmp/run/mountd/", dev);
	}
}

int mount_check_disc(char *disc)
{
	FILE *fp = fopen("/proc/mounts", "r");
	char tmp[256];
	int avail = -1;
	if(!fp)
	{
		log_printf("error reading /proc/mounts");
		fclose(fp);
		return avail;
	}
	while((fgets(tmp, 256, fp) > 0) && (avail == -1))
	{
		char *t;
		char tmp2[32];
		t = strstr(tmp, " ");
		if(t)
		{
			int l;
			*t = '\0';
			l = snprintf(tmp2, 31, "/dev/%s", disc);

			if(!strncmp(tmp, tmp2, l))
				avail = 0;
		}
	}
	fclose(fp);
	return avail;
}

int mount_wait_for_disc(char *disc)
{
	int i = 10;
	while(i--)
	{
		int ret = mount_check_disc(disc);
		if(!ret)
			return ret;
		poll(0, 0, 100);
	}
	return -1;
}

int mount_new(char *path, char *dev)
{
	struct mount *mount;
	char tmp[256];
	int ret = 1;
	pid_t pid;
	
	int nb_of_modules = 0;
	FILE *fp;
	fp = popen("lsmod | grep ufsd 2>&1", "r");
	if(fp)
	{
		while (fgets(tmp, sizeof(tmp)-1, fp) != NULL)
			++nb_of_modules;
		pclose(fp);
	}
	
	mount = mount_find(0, dev);
	if(!mount)
	{
		log_printf("request for invalid path %s%s\n", path, dev);
		return -1;
	}
	if(mount->ignore || mount->mounted || mount->fs == EXTENDED)
		return -1;
	snprintf(tmp, 256, "%s%s", path, mount->dev);
	log_printf("mounting %s\n", tmp);
	mkdir(tmp, 777);

	pid = autofs_safe_fork();
	if(!pid)
	{
		if(mount->fs == FAT)
		{
			log_printf("mount -t vfat -o rw,uid=1000,gid=1000 /dev/%s %s", mount->dev, tmp);
			ret = system_printf("mount -t vfat -o rw,uid=1000,gid=1000 /dev/%s %s", mount->dev, tmp);
		}
		if(mount->fs == EXT4)
		{
			log_printf("mount -t ext4 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			ret = system_printf("mount -t ext4 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			
			if (WEXITSTATUS(ret) == 0xff) {
				log_printf("----------> ext4 mount failed, try to fix mount volume...\n");
				system_printf("e2fsck -a /dev/%s", mount->dev);
				log_printf("mount -t ext4 -o rw,defaults /dev/%s %s", mount->dev, tmp);
				ret = system_printf("mount -t ext4 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			}
		}
		if(mount->fs == EXT3)
		{
			log_printf("mount -t ext3 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			ret = system_printf("mount -t ext3 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			
			if(WEXITSTATUS(ret) == 0xff) {
				log_printf("----------> ext3 mount failed, try to fix mount volume...\n");
				system_printf("e2fsck -a /dev/%s", mount->dev);
				log_printf("mount -t ext3 -o rw,defaults /dev/%s %s", mount->dev, tmp);
				ret = system_printf("mount -t ext3 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			}
		}
		if(mount->fs == EXT2)
		{
			log_printf("mount -t ext2 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			ret = system_printf("mount -t ext2 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			
			if(WEXITSTATUS(ret) == 0xff) {
				log_printf("----------> ext2 mount failed, try to fix mount volume...\n");
				system_printf("e2fsck -a /dev/%s", mount->dev);
				log_printf("mount -t ext2 -o rw,defaults /dev/%s %s", mount->dev, tmp);
				ret = system_printf("mount -t ext2 -o rw,defaults /dev/%s %s", mount->dev, tmp);
			}
		}
		if(mount->fs == HFSPLUS && nb_of_modules > 0)
		{
			log_printf("ufsd-hfsplus /dev/%s %s", mount->dev, tmp);
			ret = system_printf("mount -t ufsd -o nls=utf8 /dev/%s %s", mount->dev, tmp);
			
			if(WEXITSTATUS(ret) == 0xff) {
				log_printf("----------> ufsd mount failed, try to fix mount volume...\n");
				system_printf("chkhfs -a -f /dev/%s", mount->dev);
				log_printf("ufsd-hfs /dev/%s %s", mount->dev, tmp);
				ret = system_printf("mount -t ufsd -o nls=utf8 /dev/%s %s", mount->dev, tmp);
			}
		}
		else if(mount->fs == HFSPLUS)
		{
			log_printf("mount -t hfsplus -o rw,defaults,uid=1000,gid=1000 /dev/%s %s", mount->dev, tmp);
			ret = system_printf("mount -t hfsplus -o rw,defaults,uid=1000,gid=1000 /dev/%s %s", mount->dev, tmp);
		}
		if(mount->fs == NTFS && nb_of_modules > 0)
		{
			log_printf("ufsd-ntfs /dev/%s %s", mount->dev, tmp);
			ret = system_printf("mount -t ufsd -o nls=utf8 /dev/%s %s", mount->dev, tmp);
											
			if(WEXITSTATUS(ret) == 0xff) {
				log_printf("----------> ufsd mount failed, try to fix mount volume...\n");
				system_printf("chkntfs -a -f /dev/%s", mount->dev);
				log_printf("ufsd-ntfs /dev/%s %s", mount->dev, tmp);
				ret = system_printf("mount -t ufsd -o nls=utf8 /dev/%s %s", mount->dev, tmp);
			}
		}
		else if(mount->fs == NTFS)
		{
			log_printf("ntfs-3g /dev/%s %s -o force", mount->dev, tmp);
			ret = system_printf("ntfs-3g /dev/%s %s -o force", mount->dev, tmp);
		}
		if(mount->fs == EXFAT && nb_of_modules > 0)
		{
			log_printf("ufsd-exfat /dev/%s %s", mount->dev, tmp);
			ret = system_printf("mount -t ufsd -o nls=utf8 /dev/%s %s", mount->dev, tmp);
			
			if(WEXITSTATUS(ret) == 0xff) {
				log_printf("----------> ufsd mount failed, try to fix mount volume...\n");
				system_printf("chkexfat -a -f /dev/%s", mount->dev);
				log_printf("ufsd-exfat /dev/%s %s", mount->dev, tmp);
				ret = system_printf("mount -t ufsd -o nls=utf8 /dev/%s %s", mount->dev, tmp);
			}
		}
		exit(WEXITSTATUS(ret));
	}
	pid = waitpid(pid, &ret, 0);
	ret = WEXITSTATUS(ret);
	log_printf("----------> mount ret = %d\n", ret);
	if(ret && (ret != 0xff))
		return -1;
	if(mount_wait_for_disc(mount->dev) == 0)
	{
		mount->mounted = 1;
		mount_dump_uci_state();
	} else return -1;
	return 0;
}

int mount_remove(char *path, char *dev)
{
	struct mount *mount;
	char tmp[256];
	int ret;
	snprintf(tmp, 256, "%s%s", path, dev);
	log_printf("%s has expired... unmounting\n", tmp);
	ret = system_printf("/bin/umount %s", tmp);
	if(ret != 0)
		return 0;
	rmdir(tmp);
	mount = mount_find(0, dev);
	if(mount)
		mount->mounted = 0;
	log_printf("finished unmounting\n");
	mount_dump_uci_state();
	return 0;
}

int dir_sort(const void *a, const void *b)
{
	return 0;
}

int dir_filter(const struct dirent *a)
{
	if(strstr(a->d_name, ":"))
		return 1;
	return 0;
}

char* mount_get_serial(char *dev)
{
	static char tmp[64];
	static char tmp2[64];
	int disc;
	static struct hd_driveid hd;
	int i;
	static char *serial;
	snprintf(tmp, 64, "/dev/%s", dev);
	disc = open(tmp, O_RDONLY);
	if(!disc)
	{
		log_printf("Trying to open unknown disc\n");
		return 0;
	}
	i = ioctl(disc, HDIO_GET_IDENTITY, &hd);
	close(disc);
	if(!i)
		serial = (char*)hd.serial_no;
	/* if we failed, it probably a usb storage device */
	/* there must be a better way for this */
	if(i)
	{
		struct dirent **namelist;
		int n = scandir("/sys/bus/scsi/devices/", &namelist, dir_filter, dir_sort);
		if(n > 0)
		{
			while(n--)
			{
				char *t = strstr(namelist[n]->d_name, ":");
				if(t)
				{
					int id;
					struct stat buf;
					char tmp3[64];
					int ret;
					*t = 0;
					id = atoi(namelist[n]->d_name);
					*t = ':';
					sprintf(tmp3, "/sys/bus/scsi/devices/%s/block:%s/", namelist[n]->d_name, dev);
					ret = stat(tmp3, &buf);
					if(ret)
					{
						sprintf(tmp3, "/sys/bus/scsi/devices/%s/block/%s/", namelist[n]->d_name, dev);
						ret = stat(tmp3, &buf);
					}
					if(!ret)
					{
						FILE *fp;
						snprintf(tmp2, 64, "/proc/scsi/usb-storage/%d", id);
						fp = fopen(tmp2, "r");
						if(fp)
						{
							while(fgets(tmp2, 64, fp) > 0)
							{
								serial = strstr(tmp2, "Serial Number:");
								if(serial)
								{
									serial += strlen("Serial Number: ");
									serial[strlen(serial) - 1] = '\0';
									i = 0;
									break;
								}
							}
							fclose(fp);
						}
					}
				}
				free(namelist[n]);
			}
			free(namelist);
		}
	}
	if(i)
	{
		log_printf("could not find a serial number for the device %s\n", dev);
	} else {
		/* serial string id is cheap, but makes the discs anonymous */
		unsigned char uniq[6];
		int l = strlen(serial);
		int i;
		static char disc_id[13];
		memset(disc_id, 0, 13);
		memset(uniq, 0, 6);
		for(i = 0; i < l; i++)
		{
			uniq[i%6] += serial[i];
		}
		sprintf(disc_id, "%08X%02X%02X", *((unsigned int*)&uniq[0]), uniq[4], uniq[5]);
		//log_printf("Serial number - %s %s\n", serial, disc_id);
		return disc_id;
	}
	return 0;
}

void mount_dev_add(char *dev)
{
	struct mount *mount = mount_find(0, dev);
	if(!mount)
	{
		char node[64];
		char name[64];
		int ignore = 0;
		char *s;
		char tmp[64];
		char tmp2[64];
		char *p;
		struct uci_context *ctx;
		char vendor[64];
		char model[64];
		char rev[64];
		char size[64];
		char sector_size[64];
		FILE *fp;
		strcpy(name, dev);
		name[3] = '\0';
		s = mount_get_serial(name);
		if(!s)
			return;
		snprintf(tmp, 64, "part%s", &dev[3]);
		snprintf(node, 64, "Disc-%s", &dev[2]);
		if(node[5] >= 'a' && node[5] <= 'z')
		{
			node[5] -= 'a';
			node[5] += 'A';
		}
		ctx = ucix_init("mountd");
		p = ucix_get_option(ctx, "mountd", s, tmp);
		ucix_cleanup(ctx);
		if(p)
		{
			if(strlen(p) == 1)
			{
				if(*p == '0')
					ignore = 1;
			} else {
				snprintf(node, 64, "%s", p);
			}
		}
		strcpy(name, dev);
		name[3] = '\0';
		snprintf(tmp, 64, "/sys/class/block/%s/device/model", name);
		fp = fopen(tmp, "r");
		if(!fp)
		{
			snprintf(tmp, 64, "/sys/block/%s/device/model", name);
			fp = fopen(tmp, "r");
		}
		if(!fp)
			snprintf(model, 64, "unknown");
		else {
			fgets(model, 64, fp);
			model[strlen(model) - 1] = '\0';;
			fclose(fp);
		}
		snprintf(tmp, 64, "/sys/class/block/%s/device/vendor", name);
		fp = fopen(tmp, "r");
		if(!fp)
		{
			snprintf(tmp, 64, "/sys/block/%s/device/vendor", name);
			fp = fopen(tmp, "r");
		}
		if(!fp)
			snprintf(vendor, 64, "unknown");
		else {
			fgets(vendor, 64, fp);
			vendor[strlen(vendor) - 1] = '\0';
			fclose(fp);
		}
		snprintf(tmp, 64, "/sys/class/block/%s/device/rev", name);
		fp = fopen(tmp, "r");
		if(!fp)
		{
			snprintf(tmp, 64, "/sys/block/%s/device/rev", name);
			fp = fopen(tmp, "r");
		}
		if(!fp)
			snprintf(rev, 64, "unknown");
		else {
			fgets(rev, 64, fp);
			rev[strlen(rev) - 1] = '\0';
			fclose(fp);
		}
		snprintf(tmp, 64, "/sys/class/block/%s/size", dev);
		fp = fopen(tmp, "r");
		if(!fp)
		{
			snprintf(tmp, 64, "/sys/block/%s/%s/size", name, dev);
			fp = fopen(tmp, "r");
		}
		if(!fp)
			snprintf(size, 64, "unknown");
		else {
			fgets(size, 64, fp);
			size[strlen(size) - 1] = '\0';
			fclose(fp);
		}
		strcpy(tmp2, dev);
		tmp2[3] = '\0';
		snprintf(tmp, 64, "/sys/block/%s/queue/hw_sector_size", tmp2);
		fp = fopen(tmp, "r");
		if(!fp)
			snprintf(sector_size, 64, "unknown");
		else {
			fgets(sector_size, 64, fp);
			sector_size[strlen(sector_size) - 1] = '\0';
			fclose(fp);
		}
		snprintf(tmp, 64, "/dev/%s", dev);
		mount_add_list(node, dev, s, vendor, model, rev, ignore, size, sector_size, detect_fs(tmp));
		mount_dump_uci_state();
	}
}

void mount_dev_del(char *dev)
{
	struct mount *mount = mount_find(0, dev);
	char tmp[256];
	if(mount)
	{
		if(mount->mounted)
		{
			snprintf(tmp, 256, "%s%s", "/tmp/run/mountd/", mount->name);
			log_printf("%s has dissappeared ... unmounting\n", tmp);
			snprintf(tmp, 256, "%s%s", "/tmp/run/mountd/", mount->dev);
			system_printf("/bin/umount %s", tmp);
			rmdir(tmp);
			snprintf(tmp, 64, "%s%s", uci_path, mount->name);
			unlink(tmp);
			mount_dump_uci_state();
		}
	}
}

void mount_dump_list(void)
{
	struct list_head *p;
	list_for_each(p, &mounts)
	{
		struct mount *q = container_of(p, struct mount, list);
		log_printf("* %s %s %d\n", q->name, q->dev, q->mounted);
	}
}

char* is_mounted(char *block, char *path)
{
	int i;
	for(i = 0; i < mounted_count; i++)
	{
		if(block)
			if(!strncmp(&mounted[i][0][0], block, strlen(&mounted[i][0][0])))
				return &mounted[i][0][0];
		if(path)
			if(!strncmp(&mounted[i][1][1], &path[1], strlen(&mounted[i][1][0])))
				return &mounted[i][0][0];
	}
	return 0;
}

void mount_check_mount_list(void)
{
	FILE *fp = fopen("/proc/mounts", "r");
	char tmp[256];

	if(!fp)
	{
		log_printf("error reading /proc/mounts");
		fclose(fp);
		return;
	}
	mounted_count = 0;
	while(fgets(tmp, 256, fp) > 0)
	{
		char *t, *t2;
		t = strstr(tmp, " ");
		if(t)
		{
			*t = '\0';
			t++;
		} else t = tmp;
		strncpy(&mounted[mounted_count][0][0], tmp, MAX_MOUNT_NAME);
		t2 = strstr(t, " ");
		if(t2)
		{
			*t2 = '\0';
			t2++;
		} else t2 = t;
		strncpy(&mounted[mounted_count][1][0], t, MAX_MOUNT_NAME);
		t = strstr(t2, " ");
		if(t)
		{
			*t = '\0';
			t++;
		} else t = tmp;
		strncpy(&mounted[mounted_count][2][0], t2, MAX_MOUNT_NAME);
	/*	printf("%s %s %s\n",
			mounted[mounted_count][0],
			mounted[mounted_count][1],
			mounted[mounted_count][2]);*/
		if(mounted_count < MAX_MOUNTED - 1)
			mounted_count++;
		else
			log_printf("found more than %d mounts \n", MAX_MOUNTED);
	}
	fclose(fp);
}

/* FIXME: we need ore intelligence here */
int dir_filter2(const struct dirent *a)
{
	if(/*strcmp(a->d_name, "sda") &&*/(!strncmp(a->d_name, "sd", 2)))
		return 1;
	return 0;
}
#define MAX_BLOCK	64
char block[MAX_BLOCK][MAX_BLOCK];
int blk_cnt = 0;

int check_block(char *b)
{
	int i;
	for(i = 0; i < blk_cnt; i++)
	{
		if(!strcmp(block[i], b))
			return 1;
	}
	return 0;
}

void mount_enum_drives(void)
{
	struct dirent **namelist, **namelist2;
	int i, n = scandir("/sys/block/", &namelist, dir_filter2, dir_sort);
	struct list_head *p;
	blk_cnt = 0;
	if(n > 0)
	{
		while(n--)
		{
			if(blk_cnt < MAX_BLOCK)
			{
				int m;
				char tmp[64];
				snprintf(tmp, 64, "/sys/block/%s/", namelist[n]->d_name);
				m = scandir(tmp, &namelist2, dir_filter2, dir_sort);
				while(m--)
				{
					strncpy(&block[blk_cnt][0], namelist2[m]->d_name, MAX_BLOCK);
					blk_cnt++;
					free(namelist2[m]);
				}
				free(namelist2);
			}
			free(namelist[n]);
		}
		free(namelist);
	}
	p = mounts.next;
	while(p != &mounts)
	{
		struct mount *q = container_of(p, struct mount, list);
		char tmp[64];
		struct uci_context *ctx;
		int del = 0;
		char *t;
		snprintf(tmp, 64, "part%s", &q->dev[3]);
		ctx = ucix_init("mountd");
		t = ucix_get_option(ctx, "mountd", q->serial, tmp);
		ucix_cleanup(ctx);
		if(t && !q->mounted)
		{
			if(!strcmp(t, "0"))
			{
				if(!q->ignore)
					del = 1;
			} else if(!strcmp(t, "1"))
			{
				if(strncmp(q->name, "Disc-", 5))
					del = 1;
			} else if(strcmp(q->name, t))
			{
				del = 1;
			}
		}
		if(!check_block(q->dev)||del)
		{
			mount_dev_del(q->dev);
			p->prev->next = p->next;
			p->next->prev = p->prev;
			p = p->next;
			log_printf("removing %s\n", q->dev);
			snprintf(tmp, 64, "%s%s", uci_path, q->name);
			unlink(tmp);
			system_printf("/etc/mountd/event remove %s %s", q->dev, q->name);
			free(q);
			mount_dump_uci_state();
			system_printf("/etc/fonstated/ReloadSamba");
		} else p = p->next;
	}

	for(i = 0; i < blk_cnt; i++)
		mount_dev_add(block[i]);
}

void mount_check_enum(void)
{
	waitpid(-1, 0, WNOHANG);
	mount_enum_drives();
}

void mount_init(void)
{
	INIT_LIST_HEAD(&mounts);
	timer_add(mount_check_mount_list, 2);
	timer_add(mount_check_enum, 1);
	mount_check_mount_list();
}
