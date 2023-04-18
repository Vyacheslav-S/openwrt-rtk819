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
#include <unistd.h>
#include <stdio.h>

/********** logging **********/
#define lprintf(...) \
    if (getenv("LIBMMS_DEBUG")) \
        fprintf(stderr, "mms: " __VA_ARGS__)

#define __MMS_C__

#include "utf.h"

static size_t utf16le_encode(unsigned char *dest, size_t dest_size, unsigned int cp, const unsigned char *prev)
{
    unsigned short val;

    if (cp <= 0xffff)
    {
        if (dest_size < 2)
        {
            lprintf("mms: Buffer too small to encode string\n");
            return 0;
        }

        if (cp >= 0xdc00 && cp <= 0xdfff && prev)
        {
            /* Translate reserved code points so long they can not be misinterpreted.
               In partiuclar, a lead surrogate should never be followed by a trail surrogate. */
            val = prev[0] | (prev[1] << 8);
            if (val >= 0xd800 && val <= 0xdbff)
            {
                /* We are writing a trail surrogate and the previous character was a lead surrogate */
                lprintf("mms: Cannot encode reserved character\n");
                return 0;
            }
        }

        val = cp & 0xffff;
        dest[0] = val & 0xff;
        dest[1] = (val >> 8) & 0xff;
        return 2;
    }
    else if (cp >= 0x10000 && cp <= 0x10ffff)
    {
        if (dest_size < 4)
        {
            lprintf("mms: Buffer too small to encode string\n");
            return 0;
        }

        cp -= 0x10000;

        /* Write lead surrogate for supplementary plane */
        val = ((cp & 0xffc00) >> 10) + 0xd800;
        dest[0] = val & 0xff;
        dest[1] = (val >> 8) & 0xff;

        /* Write trail surrogate for supplementary plane */
        val = (cp & 0x3ff) + 0xdc00;
        dest[2] = val & 0xff;
        dest[3] = (val >> 8) & 0xff;
        return 4;
    }

    lprintf("mms: Cannot encode character\n");
    return 0;
}

static size_t utf8_seqlen(unsigned char ch)
{
    if (ch <= 0x7f)
        return 1;
    if ((ch & 0xe0) == 0xc0)
        return 2;
    if ((ch & 0xf0) == 0xe0)
        return 3;
    if ((ch & 0xf8) == 0xf0)
        return 4;
    if ((ch & 0xfc) == 0xf8)
        return 5;
    if ((ch & 0xfe) == 0xfc)
        return 6;
    return 0;
}

static size_t utf8_decode(const unsigned char* ip, unsigned int* cp)
{
    size_t slen, i;

    slen = utf8_seqlen(ip[0]);
    if (slen == 0)
    {
        lprintf("mms: Invalid utf8 character\n");
        return 0;
    }

    if (slen == 1)
        *cp = ip[0] & 0x7f;
    else
        *cp = ip[0] & ((1 << (8 - slen - 1)) - 1);

    for (i = 1; i < slen; i++)
    {   
        if (!ip[i])
        {   
            lprintf("mms: Unexpected utf8 termination\n");
            return 0;
        }
        else if ((ip[i] & 0xc0) != 0x80)
        {   
            lprintf("mms: Malformed utf8 character\n");
            return 0;
        }

        *cp = (*cp << 6) | (ip[i] & 0x3f);
    }

    return slen;
}

int mms_utf8_to_utf16le(char* dest, size_t dest_size, const char* str)
{
    unsigned char* op = (unsigned char*)dest;
    const unsigned char *prev_op = NULL;
    unsigned int cp;
    size_t slen, olen;

    if (dest_size < 2)
    {
        lprintf("mms: Cannot convert string to utf16le: Buffer too small\n");
        return 0;
    }

    dest_size -= 2;

    while (*str)
    {
        slen = utf8_decode((const unsigned char*)str, &cp);
        if (slen == 0)
            return 0;

        olen = utf16le_encode(op, dest_size, cp, prev_op);
        if (!olen)
            return 0;

        str += slen;
        dest_size -= olen;
        op += olen;
        prev_op = op - 2;
    }

    *op++ = 0;
    *op++ = 0;

    return op - (unsigned char *)dest;
}

