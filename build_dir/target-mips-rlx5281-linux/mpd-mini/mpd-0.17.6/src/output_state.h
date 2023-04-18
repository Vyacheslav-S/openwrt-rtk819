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
 * Saving and loading the audio output states to/from the state file.
 *
 */

#ifndef OUTPUT_STATE_H
#define OUTPUT_STATE_H

#include <stdbool.h>
#include <stdio.h>

bool
audio_output_state_read(const char *line);

void
audio_output_state_save(FILE *fp);

/**
 * Generates a version number for the current state of the audio
 * outputs.  This is used by timer_save_state_file() to determine
 * whether the state has changed and the state file should be saved.
 */
unsigned
audio_output_state_get_version(void);

#endif
