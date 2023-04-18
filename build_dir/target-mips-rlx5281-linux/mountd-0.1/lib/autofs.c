#include <sys/types.h>
#include <linux/types.h>
#include <paths.h>
#include <limits.h>
#include <time.h>

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>

#include <errno.h>

#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <linux/auto_fs4.h>

#include "include/log.h"
#include "include/sys.h"
#include "include/timer.h"
#include "include/mount.h"
#include "include/signal.h"
#include "include/ucix.h"

int fdin = 0; /* data coming out of the kernel */
int fdout = 0;/* data going into the kernel */
dev_t dev;

time_t uci_timeout;
char uci_path[32];

void umount_autofs(void)
{
	system_printf("umount %s 2> /dev/null", "/tmp/run/mountd/");
}

static int mount_autofs(void)
{
	int pipefd[2];
	struct stat st;
	log_printf("trying to mount %s as the autofs root\n", "/tmp/run/mountd/");
	if(is_mounted(0, "/tmp/run/mountd/"))
	{
		log_printf("%s is already mounted\n", "/tmp/run/mountd/");
		return -1;
	}
	fdout = fdin = -1;
	mkdir("/tmp/run/mountd/", 0555);
	if(pipe(pipefd) < 0)
	{
		log_printf("failed to get kernel pipe\n");
		return -1;
	}
	if(system_printf("/bin/mount -t autofs -o fd=%d,pgrp=%u,minproto=5,maxproto=5 \"mountd(pid%u)\" %s",
		pipefd[1], (unsigned) getpgrp(), getpid(), "/tmp/run/mountd/") != 0)
	{
		log_printf("unable to mount autofs on %s\n", "/tmp/run/mountd/");
		close(pipefd[0]);
		close(pipefd[1]);
		return -1;
	}

	close(pipefd[1]);
	fdout = pipefd[0];

	fdin = open("/tmp/run/mountd/", O_RDONLY);
	if(fdin < 0)
	{
		umount_autofs();
		return -1;
	}
	stat("/tmp/run/mountd/", &st);
	return 0;
}

static void send_ready(unsigned int wait_queue_token)
{
	if(ioctl(fdin, AUTOFS_IOC_READY, wait_queue_token) < 0)
		log_printf("failed to report ready to kernel\n");
}

static void send_fail(unsigned int wait_queue_token)
{
	if(ioctl(fdin, AUTOFS_IOC_FAIL, wait_queue_token) < 0)
		log_printf("failed to report fail to kernel\n");
}

static int autofs_process_request(const struct autofs_v5_packet *pkt)
{
	struct stat st;
	log_printf("kernel is requesting a mount -> %s\n", pkt->name);
	chdir("/tmp/run/mountd/");
	if (lstat(pkt->name, &st) == -1 || (S_ISDIR(st.st_mode) && st.st_dev == dev)) {
		if(!mount_new("/tmp/run/mountd/", (char*)pkt->name))
		{
			send_ready(pkt->wait_queue_token);
		} else {
			send_fail(pkt->wait_queue_token);
			log_printf("failed to mount %s\n", pkt->name);
		}
	} else {
		send_ready(pkt->wait_queue_token);
	}
	chdir("/");

	return 0;
}

void expire_proc(void)
{
	struct autofs_packet_expire pkt;
	while(ioctl(fdin, AUTOFS_IOC_EXPIRE, &pkt) == 0)
		mount_remove("/tmp/run/mountd/", pkt.name);
}

static int fullread(void *ptr, size_t len)
{
	char *buf = (char *) ptr;
	while(len > 0)
	{
		ssize_t r = read(fdout, buf, len);
		if(r == -1)
		{
			if (errno == EINTR)
				continue;
			break;
		}
		buf += r;
		len -= r;
	}
	return len;
}

static int autofs_in(union autofs_v5_packet_union *pkt)
{
	int res;
	struct pollfd fds[1];

	fds[0].fd = fdout;
	fds[0].events = POLLIN;

	while(1)
	{
		res = poll(fds, 1, -1);

		if (res == -1)
		{
			if (errno == EINTR)
				continue;
			log_printf("failed while trying to read packet from kernel\n");
			return -1;
		}
		else if ((res > 0) && (fds[0].revents & POLLIN))
		{
			return fullread(pkt, sizeof(*pkt));
		}
	}
}

pid_t autofs_safe_fork(void)
{
	pid_t pid = fork();
	if(!pid)
	{
		close(fdin);
	    close(fdout);
	}
	return pid;
}

void autofs_cleanup_handler(void)
{
	close(fdin);
	close(fdout);
	umount_autofs();
}

void autofs_init(void)
{
	int kproto_version;
	char *p;
	struct uci_context *ctx;
	signal_init(autofs_cleanup_handler);
	ctx = ucix_init("mountd");
	uci_timeout = ucix_get_option_int(ctx, "mountd", "mountd", "timeout", 60);
	p = ucix_get_option(ctx, "mountd", "mountd", "path");
	ucix_cleanup(ctx);
	if(p)
		snprintf(uci_path, 31, p);
	else
		snprintf(uci_path, 31, "/tmp/mounts/");
	uci_path[31] = '\0';
	mkdir("/tmp/run/", 0555);
	mkdir("/tmp/mounts", 0555);
	system_printf("rm -rf %s*", uci_path);
	if(uci_timeout < 16)
		uci_timeout = 16;
	umount_autofs();
	mount_init();
	if(mount_autofs() < 0)
	{
		closelog();
		exit(1);
	}
	ioctl(fdin, AUTOFS_IOC_PROTOVER, &kproto_version);
	if(kproto_version != 5)
	{
		log_printf("only kernel protocol version 5 is tested. You have %d.\n",
			kproto_version);
		closelog();
		exit(1);
	}
	ioctl(fdin, AUTOFS_IOC_SETTIMEOUT, &uci_timeout);
	timer_add(expire_proc, 15);
}

int autofs_loop(void)
{
	chdir("/");
	autofs_init();
	while(1)
	{
		union autofs_v5_packet_union pkt;
		if(autofs_in(&pkt))
		{
			continue;
		}
		log_printf("Got a autofs packet\n");
		if(pkt.hdr.type == autofs_ptype_missing_indirect)
			autofs_process_request(&pkt.missing_indirect);
		else
			log_printf("unknown packet type %d\n", pkt.hdr.type);
		poll(0, 0, 200);
	}
	umount_autofs();
	log_printf("... quitting\n");
	closelog();
	return 0;
}
