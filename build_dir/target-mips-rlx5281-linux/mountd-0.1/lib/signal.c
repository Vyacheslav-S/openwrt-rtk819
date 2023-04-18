#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/log.h"
#include "include/list.h"
#include "include/led.h"

void (*crtlc_cb)(void) = 0;

void handlerINT(int s)
{
	log_printf("caught sig int ... cleaning up\n");
	if(crtlc_cb)
		crtlc_cb();
	exit(0);
}

void signal_init(void (*_crtlc_cb)(void))
{
	struct sigaction s;
	crtlc_cb = _crtlc_cb;
	s.sa_handler = handlerINT;
	s.sa_flags = 0;
	sigaction(SIGINT, &s, NULL);
}
