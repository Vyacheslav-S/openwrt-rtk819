/*
 * Copyright (C) 2003-2011 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef ZEROCONF_INTERNAL_H
#define ZEROCONF_INTERNAL_H

/* The dns-sd service type qualifier to publish */
#define SERVICE_TYPE		"_mpd._tcp"

void init_avahi(const char *service_name);

void avahi_finish(void);

void init_zeroconf_osx(const char *service_name);

void bonjour_finish(void);

#endif
