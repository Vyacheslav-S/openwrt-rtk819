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
 * Common functions used for Ogg data streams (Ogg-Vorbis and OggFLAC)
 */

#include "config.h"
#include "_ogg_common.h"

ogg_stream_type ogg_stream_type_detect(struct input_stream *inStream)
{
	/* oggflac detection based on code in ogg123 and this post
	 * http://lists.xiph.org/pipermail/flac/2004-December/000393.html
	 * ogg123 trunk still doesn't have this patch as of June 2005 */
	unsigned char buf[41];
	size_t r;

	r = decoder_read(NULL, inStream, buf, sizeof(buf));
	if (r < sizeof(buf) || memcmp(buf, "OggS", 4) != 0)
		return VORBIS;

	if ((memcmp(buf + 29, "FLAC", 4) == 0 &&
	     memcmp(buf + 37, "fLaC", 4) == 0) ||
	    memcmp(buf + 28, "FLAC", 4) == 0 ||
	    memcmp(buf + 28, "fLaC", 4) == 0)
		return FLAC;

	return VORBIS;
}
