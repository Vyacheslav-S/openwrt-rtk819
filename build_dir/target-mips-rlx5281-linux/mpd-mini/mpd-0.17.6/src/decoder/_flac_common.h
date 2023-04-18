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
 * Common data structures and functions used by FLAC and OggFLAC
 */

#ifndef MPD_FLAC_COMMON_H
#define MPD_FLAC_COMMON_H

#include "decoder_api.h"
#include "pcm_buffer.h"

#include <glib.h>

#include <FLAC/stream_decoder.h>
#include <FLAC/metadata.h>

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "flac"

struct flac_data {
	struct pcm_buffer buffer;

	/**
	 * The size of one frame in the output buffer.
	 */
	unsigned frame_size;

	/**
	 * Has decoder_initialized() been called yet?
	 */
	bool initialized;

	/**
	 * Does the FLAC file contain an unsupported audio format?
	 */
	bool unsupported;

	/**
	 * The validated audio format of the FLAC file.  This
	 * attribute is defined if "initialized" is true.
	 */
	struct audio_format audio_format;

	/**
	 * The total number of frames in this song.  The decoder
	 * plugin may initialize this attribute to override the value
	 * provided by libFLAC (e.g. for sub songs from a CUE sheet).
	 */
	FLAC__uint64 total_frames;

	/**
	 * The number of the first frame in this song.  This is only
	 * non-zero if playing sub songs from a CUE sheet.
	 */
	FLAC__uint64 first_frame;

	/**
	 * The number of the next frame which is going to be decoded.
	 */
	FLAC__uint64 next_frame;

	FLAC__uint64 position;
	struct decoder *decoder;
	struct input_stream *input_stream;
	struct tag *tag;
};

/* initializes a given FlacData struct */
void
flac_data_init(struct flac_data *data, struct decoder * decoder,
	       struct input_stream *input_stream);

void
flac_data_deinit(struct flac_data *data);

void flac_metadata_common_cb(const FLAC__StreamMetadata * block,
			     struct flac_data *data);

void flac_error_common_cb(FLAC__StreamDecoderErrorStatus status,
			  struct flac_data *data);

FLAC__StreamDecoderWriteStatus
flac_common_write(struct flac_data *data, const FLAC__Frame * frame,
		  const FLAC__int32 *const buf[],
		  FLAC__uint64 nbytes);

#endif /* _FLAC_COMMON_H */
