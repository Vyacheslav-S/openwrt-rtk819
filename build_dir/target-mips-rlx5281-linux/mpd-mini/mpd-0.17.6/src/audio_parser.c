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

/*
 * Parser functions for audio related objects.
 *
 */

#include "config.h"
#include "audio_parser.h"
#include "audio_format.h"
#include "audio_check.h"
#include "gcc.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

/**
 * The GLib quark used for errors reported by this library.
 */
static inline GQuark
audio_parser_quark(void)
{
	return g_quark_from_static_string("audio_parser");
}

static bool
parse_sample_rate(const char *src, bool mask, uint32_t *sample_rate_r,
		  const char **endptr_r, GError **error_r)
{
	unsigned long value;
	char *endptr;

	if (mask && *src == '*') {
		*sample_rate_r = 0;
		*endptr_r = src + 1;
		return true;
	}

	value = strtoul(src, &endptr, 10);
	if (endptr == src) {
		g_set_error(error_r, audio_parser_quark(), 0,
			    "Failed to parse the sample rate");
		return false;
	} else if (!audio_check_sample_rate(value, error_r))
		return false;

	*sample_rate_r = value;
	*endptr_r = endptr;
	return true;
}

static bool
parse_sample_format(const char *src, bool mask,
		    enum sample_format *sample_format_r,
		    const char **endptr_r, GError **error_r)
{
	unsigned long value;
	char *endptr;
	enum sample_format sample_format;

	if (mask && *src == '*') {
		*sample_format_r = SAMPLE_FORMAT_UNDEFINED;
		*endptr_r = src + 1;
		return true;
	}

	if (*src == 'f') {
		*sample_format_r = SAMPLE_FORMAT_FLOAT;
		*endptr_r = src + 1;
		return true;
	}

	if (memcmp(src, "dsd", 3) == 0) {
		*sample_format_r = SAMPLE_FORMAT_DSD;
		*endptr_r = src + 3;
		return true;
	}

	value = strtoul(src, &endptr, 10);
	if (endptr == src) {
		g_set_error(error_r, audio_parser_quark(), 0,
			    "Failed to parse the sample format");
		return false;
	}

	switch (value) {
	case 8:
		sample_format = SAMPLE_FORMAT_S8;
		break;

	case 16:
		sample_format = SAMPLE_FORMAT_S16;
		break;

	case 24:
		if (memcmp(endptr, "_3", 2) == 0)
			/* for backwards compatibility */
			endptr += 2;

		sample_format = SAMPLE_FORMAT_S24_P32;
		break;

	case 32:
		sample_format = SAMPLE_FORMAT_S32;
		break;

	default:
		g_set_error(error_r, audio_parser_quark(), 0,
			    "Invalid sample format: %lu", value);
		return false;
	}

	assert(audio_valid_sample_format(sample_format));

	*sample_format_r = sample_format;
	*endptr_r = endptr;
	return true;
}

static bool
parse_channel_count(const char *src, bool mask, uint8_t *channels_r,
		    const char **endptr_r, GError **error_r)
{
	unsigned long value;
	char *endptr;

	if (mask && *src == '*') {
		*channels_r = 0;
		*endptr_r = src + 1;
		return true;
	}

	value = strtoul(src, &endptr, 10);
	if (endptr == src) {
		g_set_error(error_r, audio_parser_quark(), 0,
			    "Failed to parse the channel count");
		return false;
	} else if (!audio_check_channel_count(value, error_r))
		return false;

	*channels_r = value;
	*endptr_r = endptr;
	return true;
}

bool
audio_format_parse(struct audio_format *dest, const char *src,
		   bool mask, GError **error_r)
{
	uint32_t rate;
	enum sample_format sample_format;
	uint8_t channels;

	audio_format_clear(dest);

	/* parse sample rate */

#if GCC_CHECK_VERSION(4,7)
	/* workaround -Wmaybe-uninitialized false positive */
	rate = 0;
#endif

	if (!parse_sample_rate(src, mask, &rate, &src, error_r))
		return false;

	if (*src++ != ':') {
		g_set_error(error_r, audio_parser_quark(), 0,
			    "Sample format missing");
		return false;
	}

	/* parse sample format */

#if GCC_CHECK_VERSION(4,7)
	/* workaround -Wmaybe-uninitialized false positive */
	sample_format = SAMPLE_FORMAT_UNDEFINED;
#endif

	if (!parse_sample_format(src, mask, &sample_format, &src, error_r))
		return false;

	if (*src++ != ':') {
		g_set_error(error_r, audio_parser_quark(), 0,
			    "Channel count missing");
		return false;
	}

	/* parse channel count */

	if (!parse_channel_count(src, mask, &channels, &src, error_r))
		return false;

	if (*src != 0) {
		g_set_error(error_r, audio_parser_quark(), 0,
			    "Extra data after channel count: %s", src);
		return false;
	}

	audio_format_init(dest, rate, sample_format, channels);
	assert(mask ? audio_format_mask_valid(dest)
	       : audio_format_valid(dest));

	return true;
}
