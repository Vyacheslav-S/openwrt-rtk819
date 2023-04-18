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
#include "mixer_type.h"

#include <assert.h>
#include <string.h>

enum mixer_type
mixer_type_parse(const char *input)
{
	assert(input != NULL);

	if (strcmp(input, "none") == 0 || strcmp(input, "disabled") == 0)
		return MIXER_TYPE_NONE;
	else if (strcmp(input, "hardware") == 0)
		return MIXER_TYPE_HARDWARE;
	else if (strcmp(input, "software") == 0)
		return MIXER_TYPE_SOFTWARE;
	else
		return MIXER_TYPE_UNKNOWN;
}
