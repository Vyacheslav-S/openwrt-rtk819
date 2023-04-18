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

#ifndef MPD_ENCODER_LIST_H
#define MPD_ENCODER_LIST_H

struct encoder_plugin;

extern const struct encoder_plugin *const encoder_plugins[];

#define encoder_plugins_for_each(plugin) \
	for (const struct encoder_plugin *plugin, \
		*const*encoder_plugin_iterator = &encoder_plugins[0]; \
		(plugin = *encoder_plugin_iterator) != NULL; \
		++encoder_plugin_iterator)

/**
 * Looks up an encoder plugin by its name.
 *
 * @param name the encoder name to look for
 * @return the encoder plugin with the specified name, or NULL if none
 * was found
 */
const struct encoder_plugin *
encoder_plugin_get(const char *name);

#endif
