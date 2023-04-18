/*
 * Copyright (C) 2003-2012 The Music Player Daemon Project
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

#ifndef MPD_UPDATE_ARCHIVE_H
#define MPD_UPDATE_ARCHIVE_H

#include "check.h"

#include <stdbool.h>
#include <sys/stat.h>

struct directory;
struct archive_plugin;

#ifdef ENABLE_ARCHIVE

bool
update_archive_file(struct directory *directory,
		    const char *name, const char *suffix,
		    const struct stat *st);

#else

#include <glib.h>

static inline bool
update_archive_file(G_GNUC_UNUSED struct directory *directory,
		    G_GNUC_UNUSED const char *name,
		    G_GNUC_UNUSED const char *suffix,
		    G_GNUC_UNUSED const struct stat *st)
{
	return false;
}

#endif

#endif
