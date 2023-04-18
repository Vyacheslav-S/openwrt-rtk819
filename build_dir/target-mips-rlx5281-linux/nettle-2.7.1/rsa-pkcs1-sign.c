/* rsa-pkcs1-sign.c
 *
 * PKCS#1 version 1.5 signatures.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2012 Niels Möller
 *  
 * The nettle library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 * 
 * The nettle library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with the nettle library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02111-1301, USA.
 */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "rsa.h"

#include "pkcs1.h"

int
rsa_pkcs1_sign(const struct rsa_private_key *key,
	       unsigned length, const uint8_t *digest_info,
	       mpz_t s)
{  
  if (pkcs1_rsa_digest_encode (s, key->size, length, digest_info))
    {
      rsa_compute_root(key, s, s);
      return 1;
    }
  else
    {
      mpz_set_ui(s, 0);
      return 0;
    }    
}
