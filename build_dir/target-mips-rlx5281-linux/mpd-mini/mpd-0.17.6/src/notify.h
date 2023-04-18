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

#ifndef MPD_NOTIFY_H
#define MPD_NOTIFY_H

#include <glib.h>

#include <stdbool.h>

struct notify {
	GMutex *mutex;
	GCond *cond;
	bool pending;
};

void notify_init(struct notify *notify);

void notify_deinit(struct notify *notify);

/**
 * Wait for a notification.  Return immediately if we have already
 * been notified since we last returned from notify_wait().
 */
void notify_wait(struct notify *notify);

/**
 * Notify the thread.  This function never blocks.
 */
void notify_signal(struct notify *notify);

/**
 * Clears a pending notification.
 */
void notify_clear(struct notify *notify);

#endif
