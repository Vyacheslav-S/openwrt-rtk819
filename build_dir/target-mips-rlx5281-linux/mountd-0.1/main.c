#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "include/log.h"
#include "include/sys.h"
#include "include/timer.h"
#include "include/autofs.h"
#include "include/led.h"

int daemonize = 0;

int main(int argc, char *argv[])
{
	if ((argc < 2) || strcmp(argv[1], "-f"))
		daemon(0,0);

	daemonize = 1;
	log_start();
	log_printf("Starting OpenWrt (auto)mountd V1\n");
	timer_init();
	led_init(0);
	if (geteuid() != 0) {
		fprintf(stderr, "This program must be run by root.\n");
		return 1;
	}
	return autofs_loop();
}
