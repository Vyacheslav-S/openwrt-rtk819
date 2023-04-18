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
#include "tag_save.h"
#include "tag.h"
#include "tag_internal.h"
#include "song.h"

void tag_save(FILE *file, const struct tag *tag)
{
	if (tag->time >= 0)
		fprintf(file, SONG_TIME "%i\n", tag->time);

	if (tag->has_playlist)
		fprintf(file, "Playlist: yes\n");

	for (unsigned i = 0; i < tag->num_items; i++)
		fprintf(file, "%s: %s\n",
			tag_item_names[tag->items[i]->type],
			tag->items[i]->value);
}
