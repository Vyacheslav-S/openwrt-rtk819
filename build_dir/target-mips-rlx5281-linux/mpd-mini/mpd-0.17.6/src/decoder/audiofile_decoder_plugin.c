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
#include "decoder_api.h"
#include "audio_check.h"
#include "tag_handler.h"

#include <audiofile.h>
#include <af_vfs.h>
#include <assert.h>
#include <glib.h>
#include <stdio.h>

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "audiofile"

/* pick 1020 since its devisible for 8,16,24, and 32-bit audio */
#define CHUNK_SIZE		1020

static int audiofile_get_duration(const char *file)
{
	int total_time;
	AFfilehandle af_fp = afOpenFile(file, "r", NULL);
	if (af_fp == AF_NULL_FILEHANDLE) {
		return -1;
	}
	total_time = (int)
	    ((double)afGetFrameCount(af_fp, AF_DEFAULT_TRACK)
	     / afGetRate(af_fp, AF_DEFAULT_TRACK));
	afCloseFile(af_fp);
	return total_time;
}

static ssize_t
audiofile_file_read(AFvirtualfile *vfile, void *data, size_t length)
{
	struct input_stream *is = (struct input_stream *) vfile->closure;
	GError *error = NULL;
	size_t nbytes;

	nbytes = input_stream_lock_read(is, data, length, &error);
	if (nbytes == 0 && error != NULL) {
		g_warning("%s", error->message);
		g_error_free(error);
		return -1;
	}

	return nbytes;
}

static AFfileoffset
audiofile_file_length(AFvirtualfile *vfile)
{
	struct input_stream *is = (struct input_stream *) vfile->closure;
	return is->size;
}

static AFfileoffset
audiofile_file_tell(AFvirtualfile *vfile)
{
	struct input_stream *is = (struct input_stream *) vfile->closure;
	return is->offset;
}

static void
audiofile_file_destroy(AFvirtualfile *vfile)
{
	assert(vfile->closure != NULL);

	vfile->closure = NULL;
}

static AFfileoffset
audiofile_file_seek(AFvirtualfile *vfile, AFfileoffset offset, int is_relative)
{
	struct input_stream *is = (struct input_stream *) vfile->closure;
	int whence = (is_relative ? SEEK_CUR : SEEK_SET);
	if (input_stream_lock_seek(is, offset, whence, NULL)) {
		return is->offset;
	} else {
		return -1;
	}
}

static AFvirtualfile *
setup_virtual_fops(struct input_stream *stream)
{
	AFvirtualfile *vf = g_malloc(sizeof(AFvirtualfile));
	vf->closure = stream;
	vf->write   = NULL;
	vf->read    = audiofile_file_read;
	vf->length  = audiofile_file_length;
	vf->destroy = audiofile_file_destroy;
	vf->seek    = audiofile_file_seek;
	vf->tell    = audiofile_file_tell;
	return vf;
}

static enum sample_format
audiofile_bits_to_sample_format(int bits)
{
	switch (bits) {
	case 8:
		return SAMPLE_FORMAT_S8;

	case 16:
		return SAMPLE_FORMAT_S16;

	case 24:
		return SAMPLE_FORMAT_S24_P32;

	case 32:
		return SAMPLE_FORMAT_S32;
	}

	return SAMPLE_FORMAT_UNDEFINED;
}

static enum sample_format
audiofile_setup_sample_format(AFfilehandle af_fp)
{
	int fs, bits;

	afGetSampleFormat(af_fp, AF_DEFAULT_TRACK, &fs, &bits);
	if (!audio_valid_sample_format(audiofile_bits_to_sample_format(bits))) {
		g_debug("input file has %d bit samples, converting to 16",
			bits);
		bits = 16;
	}

	afSetVirtualSampleFormat(af_fp, AF_DEFAULT_TRACK,
	                         AF_SAMPFMT_TWOSCOMP, bits);
	afGetVirtualSampleFormat(af_fp, AF_DEFAULT_TRACK, &fs, &bits);

	return audiofile_bits_to_sample_format(bits);
}

static void
audiofile_stream_decode(struct decoder *decoder, struct input_stream *is)
{
	GError *error = NULL;
	AFvirtualfile *vf;
	int fs, frame_count;
	AFfilehandle af_fp;
	struct audio_format audio_format;
	float total_time;
	uint16_t bit_rate;
	int ret;
	char chunk[CHUNK_SIZE];
	enum decoder_command cmd;

	if (!is->seekable) {
		g_warning("not seekable");
		return;
	}

	vf = setup_virtual_fops(is);

	af_fp = afOpenVirtualFile(vf, "r", NULL);
	if (af_fp == AF_NULL_FILEHANDLE) {
		g_warning("failed to input stream\n");
		return;
	}

	if (!audio_format_init_checked(&audio_format,
				       afGetRate(af_fp, AF_DEFAULT_TRACK),
				       audiofile_setup_sample_format(af_fp),
				       afGetVirtualChannels(af_fp, AF_DEFAULT_TRACK),
				       &error)) {
		g_warning("%s", error->message);
		g_error_free(error);
		afCloseFile(af_fp);
		return;
	}

	frame_count = afGetFrameCount(af_fp, AF_DEFAULT_TRACK);

	total_time = ((float)frame_count / (float)audio_format.sample_rate);

	bit_rate = (uint16_t)(is->size * 8.0 / total_time / 1000.0 + 0.5);

	fs = (int)afGetVirtualFrameSize(af_fp, AF_DEFAULT_TRACK, 1);

	decoder_initialized(decoder, &audio_format, true, total_time);

	do {
		ret = afReadFrames(af_fp, AF_DEFAULT_TRACK, chunk,
				   CHUNK_SIZE / fs);
		if (ret <= 0)
			break;

		cmd = decoder_data(decoder, NULL,
				   chunk, ret * fs,
				   bit_rate);

		if (cmd == DECODE_COMMAND_SEEK) {
			AFframecount frame = decoder_seek_where(decoder) *
				audio_format.sample_rate;
			afSeekFrame(af_fp, AF_DEFAULT_TRACK, frame);

			decoder_command_finished(decoder);
			cmd = DECODE_COMMAND_NONE;
		}
	} while (cmd == DECODE_COMMAND_NONE);

	afCloseFile(af_fp);
}

static bool
audiofile_scan_file(const char *file,
		    const struct tag_handler *handler, void *handler_ctx)
{
	int total_time = audiofile_get_duration(file);

	if (total_time < 0) {
		g_debug("Failed to get total song time from: %s\n",
			file);
		return false;
	}

	tag_handler_invoke_duration(handler, handler_ctx, total_time);
	return true;
}

static const char *const audiofile_suffixes[] = {
	"wav", "au", "aiff", "aif", NULL
};

static const char *const audiofile_mime_types[] = {
	"audio/x-wav",
	"audio/x-aiff",
	NULL
};

const struct decoder_plugin audiofile_decoder_plugin = {
	.name = "audiofile",
	.stream_decode = audiofile_stream_decode,
	.scan_file = audiofile_scan_file,
	.suffixes = audiofile_suffixes,
	.mime_types = audiofile_mime_types,
};
