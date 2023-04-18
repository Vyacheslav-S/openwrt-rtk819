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
#include "encoder_api.h"
#include "encoder_plugin.h"
#include "audio_format.h"
#include "pcm_buffer.h"
#include "fifo_buffer.h"
#include "growing_fifo.h"

#include <assert.h>
#include <string.h>

#include <FLAC/stream_encoder.h>

struct flac_encoder {
	struct encoder encoder;

	struct audio_format audio_format;
	unsigned compression;

	FLAC__StreamEncoder *fse;

	struct pcm_buffer expand_buffer;

	/**
	 * This buffer will hold encoded data from libFLAC until it is
	 * picked up with flac_encoder_read().
	 */
	struct fifo_buffer *output_buffer;
};

extern const struct encoder_plugin flac_encoder_plugin;


static inline GQuark
flac_encoder_quark(void)
{
	return g_quark_from_static_string("flac_encoder");
}

static bool
flac_encoder_configure(struct flac_encoder *encoder,
		const struct config_param *param, G_GNUC_UNUSED GError **error)
{
	encoder->compression = config_get_block_unsigned(param,
						"compression", 5);

	return true;
}

static struct encoder *
flac_encoder_init(const struct config_param *param, GError **error)
{
	struct flac_encoder *encoder;

	encoder = g_new(struct flac_encoder, 1);
	encoder_struct_init(&encoder->encoder, &flac_encoder_plugin);

	/* load configuration from "param" */
	if (!flac_encoder_configure(encoder, param, error)) {
		/* configuration has failed, roll back and return error */
		g_free(encoder);
		return NULL;
	}

	return &encoder->encoder;
}

static void
flac_encoder_finish(struct encoder *_encoder)
{
	struct flac_encoder *encoder = (struct flac_encoder *)_encoder;

	/* the real libFLAC cleanup was already performed by
	   flac_encoder_close(), so no real work here */
	g_free(encoder);
}

static bool
flac_encoder_setup(struct flac_encoder *encoder, unsigned bits_per_sample,
		   GError **error)
{
#if !defined(FLAC_API_VERSION_CURRENT) || FLAC_API_VERSION_CURRENT <= 7
#else
	if ( !FLAC__stream_encoder_set_compression_level(encoder->fse,
					encoder->compression)) {
		g_set_error(error, flac_encoder_quark(), 0,
			    "error setting flac compression to %d",
			    encoder->compression);
		return false;
	}
#endif
	if ( !FLAC__stream_encoder_set_channels(encoder->fse,
					encoder->audio_format.channels)) {
		g_set_error(error, flac_encoder_quark(), 0,
			    "error setting flac channels num to %d",
			    encoder->audio_format.channels);
		return false;
	}
	if ( !FLAC__stream_encoder_set_bits_per_sample(encoder->fse,
							bits_per_sample)) {
		g_set_error(error, flac_encoder_quark(), 0,
			    "error setting flac bit format to %d",
			    bits_per_sample);
		return false;
	}
	if ( !FLAC__stream_encoder_set_sample_rate(encoder->fse,
					encoder->audio_format.sample_rate)) {
		g_set_error(error, flac_encoder_quark(), 0,
			    "error setting flac sample rate to %d",
			    encoder->audio_format.sample_rate);
		return false;
	}
	return true;
}

static FLAC__StreamEncoderWriteStatus
flac_write_callback(G_GNUC_UNUSED const FLAC__StreamEncoder *fse,
		    const FLAC__byte data[],
#if !defined(FLAC_API_VERSION_CURRENT) || FLAC_API_VERSION_CURRENT <= 7
		    unsigned bytes,
#else
		    size_t bytes,
#endif
		    G_GNUC_UNUSED unsigned samples,
	G_GNUC_UNUSED unsigned current_frame, void *client_data)
{
	struct flac_encoder *encoder = (struct flac_encoder *) client_data;

	//transfer data to buffer
	growing_fifo_append(&encoder->output_buffer, data, bytes);

	return FLAC__STREAM_ENCODER_WRITE_STATUS_OK;
}

static void
flac_encoder_close(struct encoder *_encoder)
{
	struct flac_encoder *encoder = (struct flac_encoder *)_encoder;

	FLAC__stream_encoder_delete(encoder->fse);

	pcm_buffer_deinit(&encoder->expand_buffer);
	fifo_buffer_free(encoder->output_buffer);
}

static bool
flac_encoder_open(struct encoder *_encoder, struct audio_format *audio_format,
		     GError **error)
{
	struct flac_encoder *encoder = (struct flac_encoder *)_encoder;
	unsigned bits_per_sample;

	encoder->audio_format = *audio_format;

	/* FIXME: flac should support 32bit as well */
	switch (audio_format->format) {
	case SAMPLE_FORMAT_S8:
		bits_per_sample = 8;
		break;

	case SAMPLE_FORMAT_S16:
		bits_per_sample = 16;
		break;

	case SAMPLE_FORMAT_S24_P32:
		bits_per_sample = 24;
		break;

	default:
		bits_per_sample = 24;
		audio_format->format = SAMPLE_FORMAT_S24_P32;
	}

	/* allocate the encoder */
	encoder->fse = FLAC__stream_encoder_new();
	if (encoder->fse == NULL) {
		g_set_error(error, flac_encoder_quark(), 0,
			    "flac_new() failed");
		return false;
	}

	if (!flac_encoder_setup(encoder, bits_per_sample, error)) {
		FLAC__stream_encoder_delete(encoder->fse);
		return false;
	}

	pcm_buffer_init(&encoder->expand_buffer);

	encoder->output_buffer = growing_fifo_new();

	/* this immediately outputs data through callback */

#if !defined(FLAC_API_VERSION_CURRENT) || FLAC_API_VERSION_CURRENT <= 7
	{
		FLAC__StreamEncoderState init_status;

		FLAC__stream_encoder_set_write_callback(encoder->fse,
					    flac_write_callback);

		init_status = FLAC__stream_encoder_init(encoder->fse);

		if (init_status != FLAC__STREAM_ENCODER_OK) {
			g_set_error(error, flac_encoder_quark(), 0,
			    "failed to initialize encoder: %s\n",
			    FLAC__StreamEncoderStateString[init_status]);
			flac_encoder_close(_encoder);
			return false;
		}
	}
#else
	{
		FLAC__StreamEncoderInitStatus init_status;

		init_status = FLAC__stream_encoder_init_stream(encoder->fse,
			    flac_write_callback,
			    NULL, NULL, NULL, encoder);

		if(init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
			g_set_error(error, flac_encoder_quark(), 0,
			    "failed to initialize encoder: %s\n",
			    FLAC__StreamEncoderInitStatusString[init_status]);
			flac_encoder_close(_encoder);
			return false;
		}
	}
#endif

	return true;
}


static bool
flac_encoder_flush(struct encoder *_encoder, G_GNUC_UNUSED GError **error)
{
	struct flac_encoder *encoder = (struct flac_encoder *)_encoder;

	(void) FLAC__stream_encoder_finish(encoder->fse);
	return true;
}

static inline void
pcm8_to_flac(int32_t *out, const int8_t *in, unsigned num_samples)
{
	while (num_samples > 0) {
		*out++ = *in++;
		--num_samples;
	}
}

static inline void
pcm16_to_flac(int32_t *out, const int16_t *in, unsigned num_samples)
{
	while (num_samples > 0) {
		*out++ = *in++;
		--num_samples;
	}
}

static bool
flac_encoder_write(struct encoder *_encoder,
		      const void *data, size_t length,
		      G_GNUC_UNUSED GError **error)
{
	struct flac_encoder *encoder = (struct flac_encoder *)_encoder;
	unsigned num_frames, num_samples;
	void *exbuffer;
	const void *buffer = NULL;

	/* format conversion */

	num_frames = length / audio_format_frame_size(&encoder->audio_format);
	num_samples = num_frames * encoder->audio_format.channels;

	switch (encoder->audio_format.format) {
	case SAMPLE_FORMAT_S8:
		exbuffer = pcm_buffer_get(&encoder->expand_buffer, length*4);
		pcm8_to_flac(exbuffer, data, num_samples);
		buffer = exbuffer;
		break;

	case SAMPLE_FORMAT_S16:
		exbuffer = pcm_buffer_get(&encoder->expand_buffer, length*2);
		pcm16_to_flac(exbuffer, data, num_samples);
		buffer = exbuffer;
		break;

	case SAMPLE_FORMAT_S24_P32:
	case SAMPLE_FORMAT_S32:
		/* nothing need to be done; format is the same for
		   both mpd and libFLAC */
		buffer = data;
		break;
	}

	/* feed samples to encoder */

	if (!FLAC__stream_encoder_process_interleaved(encoder->fse, buffer,
							num_frames)) {
		g_set_error(error, flac_encoder_quark(), 0,
			    "flac encoder process failed");
		return false;
	}

	return true;
}

static size_t
flac_encoder_read(struct encoder *_encoder, void *dest, size_t length)
{
	struct flac_encoder *encoder = (struct flac_encoder *)_encoder;

	size_t max_length;
	const char *src = fifo_buffer_read(encoder->output_buffer,
					   &max_length);
	if (src == NULL)
		return 0;

	if (length > max_length)
		length = max_length;

	memcpy(dest, src, length);
	fifo_buffer_consume(encoder->output_buffer, length);
	return length;
}

static const char *
flac_encoder_get_mime_type(G_GNUC_UNUSED struct encoder *_encoder)
{
	return "audio/flac";
}

const struct encoder_plugin flac_encoder_plugin = {
	.name = "flac",
	.init = flac_encoder_init,
	.finish = flac_encoder_finish,
	.open = flac_encoder_open,
	.close = flac_encoder_close,
	.end = flac_encoder_flush,
	.flush = flac_encoder_flush,
	.write = flac_encoder_write,
	.read = flac_encoder_read,
	.get_mime_type = flac_encoder_get_mime_type,
};

