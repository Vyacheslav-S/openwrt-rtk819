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

#ifndef MPD_INPUT_REGISTRY_H
#define MPD_INPUT_REGISTRY_H

#include "check.h"

#include <stdbool.h>

/**
 * NULL terminated list of all input plugins which were enabled at
 * compile time.
 */
extern const struct input_plugin *const input_plugins[];

extern bool input_plugins_enabled[];

#define input_plugins_for_each(plugin) \
	for (const struct input_plugin *plugin, \
		*const*input_plugin_iterator = &input_plugins[0]; \
		(plugin = *input_plugin_iterator) != NULL; \
		++input_plugin_iterator)

#define input_plugins_for_each_enabled(plugin) \
	input_plugins_for_each(plugin) \
		if (input_plugins_enabled[input_plugin_iterator - input_plugins])

#endif
