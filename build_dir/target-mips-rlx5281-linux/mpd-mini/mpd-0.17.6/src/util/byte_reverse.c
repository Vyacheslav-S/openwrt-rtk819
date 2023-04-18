/*
 * Copyright (C) 2003-2012 The Music Player Daemon Project
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

#include "byte_reverse.h"

#include <glib.h>
#include <assert.h>

void
reverse_bytes_16(uint16_t *dest, const uint16_t *src, const uint16_t *src_end)
{
	assert(dest != NULL);
	assert(src != NULL);
	assert(src_end >= src);

	while (src < src_end) {
		const uint16_t x = *src++;
		*dest++ = GUINT16_SWAP_LE_BE(x);
	}
}

void
reverse_bytes_32(uint32_t *dest, const uint32_t *src, const uint32_t *src_end)
{
	assert(dest != NULL);
	assert(src != NULL);
	assert(src_end >= src);

	while (src < src_end) {
		const uint32_t x = *src++;
		*dest++ = GUINT32_SWAP_LE_BE(x);
	}
}

void
reverse_bytes_64(uint64_t *dest, const uint64_t *src, const uint64_t *src_end)
{
	assert(dest != NULL);
	assert(src != NULL);
	assert(src_end >= src);

	while (src < src_end) {
		const uint64_t x = *src++;
		*dest++ = GUINT64_SWAP_LE_BE(x);
	}
}

static void
reverse_bytes_linear(uint8_t *dest, const uint8_t *src, size_t n)
{
	src += n;

	while (n-- > 0)
		*dest++ = *--src;
}

static void
reverse_bytes_generic(uint8_t *dest,
		      const uint8_t *src, const uint8_t *src_end,
		      size_t frame_size)
{
	assert(dest != NULL);
	assert(src != NULL);
	assert(src_end >= src);
	assert(frame_size > 0);
	assert((src_end - src) % frame_size == 0);

	while (src < src_end) {
		reverse_bytes_linear(dest, src, frame_size);
		dest += frame_size;
		src += frame_size;
	}
}

void
reverse_bytes(uint8_t *dest, const uint8_t *src, const uint8_t *src_end,
	      size_t frame_size)
{
	assert(dest != NULL);
	assert(src != NULL);
	assert(src_end >= src);
	assert(frame_size > 0);
	assert((src_end - src) % frame_size == 0);

	switch (frame_size) {
	case 2:
		reverse_bytes_16((uint16_t *)dest,
				 (const uint16_t *)src,
				 (const uint16_t *)src_end);
		break;

	case 4:
		reverse_bytes_32((uint32_t *)dest,
				 (const uint32_t *)src,
				 (const uint32_t *)src_end);
		break;

	case 8:
		reverse_bytes_64((uint64_t *)dest,
				 (const uint64_t *)src,
				 (const uint64_t *)src_end);
		break;

	default:
		reverse_bytes_generic(dest, src, src_end, frame_size);
	}
}
