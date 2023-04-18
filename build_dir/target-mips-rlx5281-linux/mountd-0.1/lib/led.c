/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Provided by fon.com
 *   Copyright (C) 2009 John Crispin <blogic@openwrt.org> 
 */


#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include "include/ucix.h"
#include "include/log.h"
#include "include/timer.h"

char usbled[16];

void led_ping(void)
{
	FILE *fp;
	static int last = 0;
	static struct uci_context *ctx;
	int mounted, count;
	int led = 1;
	char path[256];
	ctx = ucix_init("mountd");
	mounted = ucix_get_option_int(ctx, "mountd", "mountd", "mounted", 0);
	count = ucix_get_option_int(ctx, "mountd", "mountd", "count", 0);
	ucix_cleanup(ctx);
	if(!count)
		led = 0;
	if(count && !mounted)
		led = 1;
	if(count && mounted)
		last = led = (last + 1) % 2;
	snprintf(path, 256, "/sys/class/leds/%s/brightness", usbled);
	fp = fopen(path, "w");
	if(fp)
	{
		fprintf(fp, "%d", led);
		fclose(fp);
	}
}

void led_init(char *led)
{
	if(led)
	{
		strncpy(usbled, led, 16);
		timer_add(led_ping, 1);
	}
}
