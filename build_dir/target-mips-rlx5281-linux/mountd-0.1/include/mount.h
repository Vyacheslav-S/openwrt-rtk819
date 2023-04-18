#ifndef _MOUNT_H__
#define _MOUNT_H__

char* is_mounted(char *block, char *path);
int mount_new(char *path, char *name);
int mount_remove(char *path, char *name);
void mount_dump_list(void);
void mount_init(void);

#endif
