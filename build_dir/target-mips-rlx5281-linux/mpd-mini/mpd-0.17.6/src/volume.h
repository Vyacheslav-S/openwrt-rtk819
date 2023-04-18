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

#ifndef MPD_VOLUME_H
#define MPD_VOLUME_H

#include <stdbool.h>
#include <stdio.h>

void volume_init(void);

void volume_finish(void);

int volume_level_get(void);

bool volume_level_change(unsigned volume);

bool
read_sw_volume_state(const char *line);

void save_sw_volume_state(FILE *fp);

/**
 * Generates a hash number for the current state of the software
 * volume control.  This is used by timer_save_state_file() to
 * determine whether the state has changed and the state file should
 * be saved.
 */
unsigned
sw_volume_state_get_hash(void);

#endif
