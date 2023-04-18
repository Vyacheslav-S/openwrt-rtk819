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

#ifndef MPD_LS_H
#define MPD_LS_H

#include <stdbool.h>
#include <stdio.h>

struct client;

/**
 * Checks whether the scheme of the specified URI is supported by MPD.
 * It is not allowed to pass an URI without a scheme, check with
 * uri_has_scheme() first.
 */
bool uri_supported_scheme(const char *url);

/**
 * Send a list of supported URI schemes to the client.  This is the
 * response to the "urlhandlers" command.
 */
void print_supported_uri_schemes(struct client *client);

/**
 * Send a list of supported URI schemes to a file pointer.
 */
void print_supported_uri_schemes_to_fp(FILE *fp);

#endif
