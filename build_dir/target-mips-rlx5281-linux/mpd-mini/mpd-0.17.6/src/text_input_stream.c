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
#include "text_input_stream.h"
#include "input_stream.h"
#include "fifo_buffer.h"

#include <glib.h>

#include <assert.h>
#include <string.h>

struct text_input_stream {
	struct input_stream *is;

	struct fifo_buffer *buffer;

	char *line;
};

struct text_input_stream *
text_input_stream_new(struct input_stream *is)
{
	struct text_input_stream *tis = g_new(struct text_input_stream, 1);

	tis->is = is;
	tis->buffer = fifo_buffer_new(4096);
	tis->line = NULL;

	return tis;
}

void
text_input_stream_free(struct text_input_stream *tis)
{
	fifo_buffer_free(tis->buffer);
	g_free(tis->line);
	g_free(tis);
}

const char *
text_input_stream_read(struct text_input_stream *tis)
{
	GError *error = NULL;
	void *dest;
	const char *src, *p;
	size_t length, nbytes;

	g_free(tis->line);
	tis->line = NULL;

	do {
		dest = fifo_buffer_write(tis->buffer, &length);
		if (dest != NULL && length >= 2) {
			/* reserve one byte for the null terminator if
			   the last line is not terminated by a
			   newline character */
			--length;

			nbytes = input_stream_lock_read(tis->is, dest, length,
							&error);
			if (nbytes > 0)
				fifo_buffer_append(tis->buffer, nbytes);
			else if (error != NULL) {
				g_warning("%s", error->message);
				g_error_free(error);
				return NULL;
			}
		} else
			nbytes = 0;

		src = fifo_buffer_read(tis->buffer, &length);
		if (src == NULL)
			return NULL;

		p = memchr(src, '\n', length);
		if (p == NULL && nbytes == 0) {
			/* end of file (or line too long): terminate
			   the current line */
			dest = fifo_buffer_write(tis->buffer, &nbytes);
			assert(dest != NULL);
			*(char *)dest = '\n';
			fifo_buffer_append(tis->buffer, 1);
		}
	} while (p == NULL);

	length = p - src + 1;
	while (p > src && g_ascii_isspace(p[-1]))
		--p;

	tis->line = g_strndup(src, p - src);
	fifo_buffer_consume(tis->buffer, length);
	return tis->line;
}
