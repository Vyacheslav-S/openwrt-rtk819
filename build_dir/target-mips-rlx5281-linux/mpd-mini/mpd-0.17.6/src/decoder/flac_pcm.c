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
#include "flac_pcm.h"

#include <assert.h>

static void flac_convert_stereo16(int16_t *dest,
				  const FLAC__int32 * const buf[],
				  unsigned int position, unsigned int end)
{
	for (; position < end; ++position) {
		*dest++ = buf[0][position];
		*dest++ = buf[1][position];
	}
}

static void
flac_convert_16(int16_t *dest,
		unsigned int num_channels,
		const FLAC__int32 * const buf[],
		unsigned int position, unsigned int end)
{
	unsigned int c_chan;

	for (; position < end; ++position)
		for (c_chan = 0; c_chan < num_channels; c_chan++)
			*dest++ = buf[c_chan][position];
}

/**
 * Note: this function also handles 24 bit files!
 */
static void
flac_convert_32(int32_t *dest,
		unsigned int num_channels,
		const FLAC__int32 * const buf[],
		unsigned int position, unsigned int end)
{
	unsigned int c_chan;

	for (; position < end; ++position)
		for (c_chan = 0; c_chan < num_channels; c_chan++)
			*dest++ = buf[c_chan][position];
}

static void
flac_convert_8(int8_t *dest,
	       unsigned int num_channels,
	       const FLAC__int32 * const buf[],
	       unsigned int position, unsigned int end)
{
	unsigned int c_chan;

	for (; position < end; ++position)
		for (c_chan = 0; c_chan < num_channels; c_chan++)
			*dest++ = buf[c_chan][position];
}

void
flac_convert(void *dest,
	     unsigned int num_channels, enum sample_format sample_format,
	     const FLAC__int32 *const buf[],
	     unsigned int position, unsigned int end)
{
	switch (sample_format) {
	case SAMPLE_FORMAT_S16:
		if (num_channels == 2)
			flac_convert_stereo16((int16_t*)dest, buf,
					      position, end);
		else
			flac_convert_16((int16_t*)dest, num_channels, buf,
					position, end);
		break;

	case SAMPLE_FORMAT_S24_P32:
	case SAMPLE_FORMAT_S32:
		flac_convert_32((int32_t*)dest, num_channels, buf,
				position, end);
		break;

	case SAMPLE_FORMAT_S8:
		flac_convert_8((int8_t*)dest, num_channels, buf,
			       position, end);
		break;

	case SAMPLE_FORMAT_FLOAT:
	case SAMPLE_FORMAT_DSD:
	case SAMPLE_FORMAT_UNDEFINED:
		/* unreachable */
		assert(false);
	}
}
