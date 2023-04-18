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

#include "config.h"
#include "filter_registry.h"
#include "filter_plugin.h"

#include <stddef.h>
#include <string.h>

const struct filter_plugin *const filter_plugins[] = {
	&null_filter_plugin,
	&route_filter_plugin,
	&normalize_filter_plugin,
	&volume_filter_plugin,
	&replay_gain_filter_plugin,
	NULL,
};

const struct filter_plugin *
filter_plugin_by_name(const char *name)
{
	for (unsigned i = 0; filter_plugins[i] != NULL; ++i)
		if (strcmp(filter_plugins[i]->name, name) == 0)
			return filter_plugins[i];

	return NULL;
}
