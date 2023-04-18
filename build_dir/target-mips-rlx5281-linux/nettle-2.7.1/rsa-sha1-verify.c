/* rsa-sha1-verify.c
 *
 * Verifying signatures created with RSA and SHA1.
 */

/* nettle, low-level cryptographics library
 *
 * Copyright (C) 2001, 2003 Niels Möller
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

#include <assert.h>

#include "rsa.h"

#include "bignum.h"
#include "pkcs1.h"

int
rsa_sha1_verify(const struct rsa_public_key *key,
                struct sha1_ctx *hash,
                const mpz_t s)
{
  int res;
  mpz_t m;

  mpz_init(m);
  
  res = (pkcs1_rsa_sha1_encode(m, key->size, hash)
	 && _rsa_verify(key, m, s));
  
  mpz_clear(m);

  return res;
}

int
rsa_sha1_verify_digest(const struct rsa_public_key *key,
		       const uint8_t *digest,
		       const mpz_t s)
{
  int res;
  mpz_t m;

  mpz_init(m);
  
  res = (pkcs1_rsa_sha1_encode_digest(m, key->size, digest)
	 && _rsa_verify(key, m, s));
  
  mpz_clear(m);

  return res;
}
