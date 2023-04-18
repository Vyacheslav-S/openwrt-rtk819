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

#include "clock.h"

#ifdef WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <mach/mach_time.h>
#else
#include <time.h>
#ifndef CLOCK_MONOTONIC
#include <sys/time.h>
#endif
#endif

unsigned
monotonic_clock_ms(void)
{
#ifdef WIN32
	return GetTickCount();
#elif defined(__APPLE__) /* OS X does not define CLOCK_MONOTONIC */
	static mach_timebase_info_data_t base;
	if (base.denom == 0)
		(void)mach_timebase_info(&base);

	return (unsigned)((mach_absolute_time() * base.numer)
			  / (1000000 * base.denom));
#elif defined(CLOCK_MONOTONIC)
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#else
	/* we have no monotonic clock, fall back to gettimeofday() */
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

uint64_t
monotonic_clock_us(void)
{
#ifdef WIN32
	LARGE_INTEGER l_value, l_frequency;

	if (!QueryPerformanceCounter(&l_value) ||
	    !QueryPerformanceFrequency(&l_frequency))
		return 0;

	uint64_t value = l_value.QuadPart;
	uint64_t frequency = l_frequency.QuadPart;

	if (frequency > 1000000) {
		value *= 10000;
		value /= frequency / 100;
	} else if (frequency < 1000000) {
		value *= 10000;
		value /= frequency;
		value *= 100;
	}

	return value;
#elif defined(__APPLE__) /* OS X does not define CLOCK_MONOTONIC */
	static mach_timebase_info_data_t base;
	if (base.denom == 0)
		(void)mach_timebase_info(&base);

	return ((uint64_t)mach_absolute_time() * (uint64_t)base.numer)
		/ (1000 * (uint64_t)base.denom);
#elif defined(CLOCK_MONOTONIC)
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * 1000000 + (uint64_t)(ts.tv_nsec / 1000);
#else
	/* we have no monotonic clock, fall back to gettimeofday() */
	struct timeval tv;
	gettimeofday(&tv, 0);
	return (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec;
#endif
}

