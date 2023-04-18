#include <stdio.h>
#include <syslog.h>
#include <stdarg.h>

extern int daemonize;

void log_start(void)
{
	openlog("mountd", LOG_PID, LOG_DAEMON);
}

void log_stop(void)
{
	closelog();
}

void log_printf(char *fmt, ...)
{
	char p[256];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(p, 256, fmt, ap);
	va_end(ap);

	if(daemonize)
		syslog(10, p);
	else
		printf(p);
}
