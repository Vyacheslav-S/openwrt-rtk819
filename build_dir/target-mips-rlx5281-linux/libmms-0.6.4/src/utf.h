/*
 * Copyright (C) 2014 the libmms project
 * 
 * This file is part of LibMMS, an MMS protocol handling library.
 * This file was originally a part of xine, a free video player.
 *
 * Libmms is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Libmms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 */
#ifndef HAVE_UTF_H
#define HAVE_UTF_H

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int mms_utf8_to_utf16le(char* dest, size_t dest_size, const char* str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

