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
#include "playlist_save.h"
#include "playlist.h"
#include "stored_playlist.h"
#include "queue.h"
#include "song.h"
#include "mapper.h"
#include "path.h"
#include "uri.h"
#include "database.h"
#include "idle.h"
#include "glib_compat.h"

#include <glib.h>

#include <string.h>

void
playlist_print_song(FILE *file, const struct song *song)
{
	if (playlist_saveAbsolutePaths && song_in_database(song)) {
		char *path = map_song_fs(song);
		if (path != NULL) {
			fprintf(file, "%s\n", path);
			g_free(path);
		}
	} else {
		char *uri = song_get_uri(song), *uri_fs;

		uri_fs = utf8_to_fs_charset(uri);
		g_free(uri);

		fprintf(file, "%s\n", uri_fs);
		g_free(uri_fs);
	}
}

void
playlist_print_uri(FILE *file, const char *uri)
{
	char *s;

	if (playlist_saveAbsolutePaths && !uri_has_scheme(uri) &&
	    !g_path_is_absolute(uri))
		s = map_uri_fs(uri);
	else
		s = utf8_to_fs_charset(uri);

	if (s != NULL) {
		fprintf(file, "%s\n", s);
		g_free(s);
	}
}

enum playlist_result
spl_save_queue(const char *name_utf8, const struct queue *queue)
{
	char *path_fs;
	FILE *file;

	if (map_spl_path() == NULL)
		return PLAYLIST_RESULT_DISABLED;

	if (!spl_valid_name(name_utf8))
		return PLAYLIST_RESULT_BAD_NAME;

	path_fs = map_spl_utf8_to_fs(name_utf8);
	if (path_fs == NULL)
		return PLAYLIST_RESULT_BAD_NAME;

	if (g_file_test(path_fs, G_FILE_TEST_EXISTS)) {
		g_free(path_fs);
		return PLAYLIST_RESULT_LIST_EXISTS;
	}

	file = fopen(path_fs, "w");
	g_free(path_fs);

	if (file == NULL)
		return PLAYLIST_RESULT_ERRNO;

	for (unsigned i = 0; i < queue_length(queue); i++)
		playlist_print_song(file, queue_get(queue, i));

	fclose(file);

	idle_add(IDLE_STORED_PLAYLIST);
	return PLAYLIST_RESULT_SUCCESS;
}

enum playlist_result
spl_save_playlist(const char *name_utf8, const struct playlist *playlist)
{
	return spl_save_queue(name_utf8, &playlist->queue);
}

bool
playlist_load_spl(struct playlist *playlist, struct player_control *pc,
		  const char *name_utf8,
		  unsigned start_index, unsigned end_index,
		  GError **error_r)
{
	GPtrArray *list;

	list = spl_load(name_utf8, error_r);
	if (list == NULL)
		return false;

	if (list->len < end_index)
		end_index = list->len;

	for (unsigned i = start_index; i < end_index; ++i) {
		const char *temp = g_ptr_array_index(list, i);

		if (memcmp(temp, "file:///", 8) == 0) {
			const char *path = temp + 7;

			if (playlist_append_file(playlist, pc, path, NULL) != PLAYLIST_RESULT_SUCCESS)
				g_warning("can't add file \"%s\"", path);
			continue;
		}

		if ((playlist_append_uri(playlist, pc, temp, NULL)) != PLAYLIST_RESULT_SUCCESS) {
			/* for windows compatibility, convert slashes */
			char *temp2 = g_strdup(temp);
			char *p = temp2;
			while (*p) {
				if (*p == '\\')
					*p = '/';
				p++;
			}
			if ((playlist_append_uri(playlist, pc, temp2,
						 NULL)) != PLAYLIST_RESULT_SUCCESS) {
				g_warning("can't add file \"%s\"", temp2);
			}
			g_free(temp2);
		}
	}

	spl_free(list);
	return true;
}
