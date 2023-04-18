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

#ifndef MPD_TAG_RVA2_H
#define MPD_TAG_RVA2_H

#include "check.h"

#include <stdbool.h>

struct id3_tag;
struct replay_gain_info;

/**
 * Parse the RVA2 tag, and fill the #replay_gain_info struct.  This is
 * used by decoder plugins with ID3 support.
 *
 * @return true on success
 */
bool
tag_rva2_parse(struct id3_tag *tag, struct replay_gain_info *replay_gain_info);

#endif
