/*
 * Copyright (C) 2000-2013 Free Software Foundation, Inc.
 * Copyright (C) 2013 Nikos Mavrogiannopoulos
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/* Some high level functions to be used in the record encryption are
 * included here.
 */

#include "gnutls_int.h"
#include "gnutls_errors.h"
#include "gnutls_compress.h"
#include "gnutls_cipher.h"
#include "algorithms.h"
#include "gnutls_hash_int.h"
#include "gnutls_cipher_int.h"
#include "debug.h"
#include "gnutls_num.h"
#include "gnutls_datum.h"
#include "gnutls_kx.h"
#include "gnutls_record.h"
#include "gnutls_constate.h"
#include "gnutls_mbuffers.h"
#include <gnutls_state.h>
#include <random.h>

static int compressed_to_ciphertext(gnutls_session_t session,
				    uint8_t * cipher_data, int cipher_size,
				    gnutls_datum_t * compressed,
				    size_t min_pad,
				    content_type_t _type,
				    record_parameters_st * params);
static int ciphertext_to_compressed(gnutls_session_t session,
				    gnutls_datum_t * ciphertext,
				    gnutls_datum_t * compressed,
				    uint8_t type,
				    record_parameters_st * params,
				    uint64 * sequence);

inline static int is_write_comp_null(record_parameters_st * record_params)
{
	if (record_params->compression_algorithm == GNUTLS_COMP_NULL)
		return 0;

	return 1;
}

inline static int is_read_comp_null(record_parameters_st * record_params)
{
	if (record_params->compression_algorithm == GNUTLS_COMP_NULL)
		return 0;

	return 1;
}


/* returns ciphertext which contains the headers too. This also
 * calculates the size in the header field.
 * 
 */
int
_gnutls_encrypt(gnutls_session_t session,
		const uint8_t * data, size_t data_size,
		size_t min_pad,
		mbuffer_st * bufel,
		content_type_t type, record_parameters_st * params)
{
	gnutls_datum_t comp;
	int free_comp = 0;
	int ret;

	if (data_size == 0 || is_write_comp_null(params) == 0) {
		comp.data = (uint8_t *) data;
		comp.size = data_size;
	} else {
		/* Here comp is allocated and must be 
		 * freed.
		 */
		free_comp = 1;

		comp.size = _mbuffer_get_udata_size(bufel);
		comp.data = gnutls_malloc(comp.size);
		if (comp.data == NULL)
			return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

		ret =
		    _gnutls_compress(&params->write.compression_state,
				     data, data_size, comp.data, comp.size,
				     session->internals.priorities.
				     stateless_compression);
		if (ret < 0) {
			gnutls_free(comp.data);
			return gnutls_assert_val(ret);
		}

		comp.size = ret;
	}

	ret =
	    compressed_to_ciphertext(session,
				     _mbuffer_get_udata_ptr(bufel),
				     _mbuffer_get_udata_size
				     (bufel), &comp, min_pad, type,
				     params);
	if (free_comp)
		gnutls_free(comp.data);

	if (ret < 0)
		return gnutls_assert_val(ret);

	if (IS_DTLS(session))
		_gnutls_write_uint16(ret,
				     ((uint8_t *)
				      _mbuffer_get_uhead_ptr(bufel)) + 11);
	else
		_gnutls_write_uint16(ret,
				     ((uint8_t *)
				      _mbuffer_get_uhead_ptr(bufel)) + 3);

	_mbuffer_set_udata_size(bufel, ret);
	_mbuffer_set_uhead_size(bufel, 0);

	return _mbuffer_get_udata_size(bufel);
}

/* Decrypts the given data.
 * Returns the decrypted data length.
 *
 * The output is preallocated with the maximum allowed data size.
 */
int
_gnutls_decrypt(gnutls_session_t session,
		gnutls_datum_t * ciphertext,
		gnutls_datum_t * output,
		content_type_t type,
		record_parameters_st * params, uint64 * sequence)
{
	int ret;

	if (ciphertext->size == 0)
		return 0;

	if (is_read_comp_null(params) == 0) {
		ret =
		    ciphertext_to_compressed(session, ciphertext,
					     output, type, params,
					     sequence);
		if (ret < 0)
			return gnutls_assert_val(ret);

		return ret;
	} else {
		gnutls_datum_t tmp;

		tmp.size = output->size;
		tmp.data = gnutls_malloc(tmp.size);
		if (tmp.data == NULL)
			return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

		ret =
		    ciphertext_to_compressed(session, ciphertext,
					     &tmp, type, params,
					     sequence);
		if (ret < 0)
			goto leave;

		tmp.size = ret;

		if (ret != 0) {
			ret =
			    _gnutls_decompress(&params->read.
					       compression_state, tmp.data,
					       tmp.size, output->data,
					       output->size);
			if (ret < 0)
				goto leave;
		}

	      leave:
		gnutls_free(tmp.data);
		return ret;
	}
}


inline static int
calc_enc_length_block(gnutls_session_t session,
		      const version_entry_st * ver,
		      int data_size,
		      int hash_size, uint8_t * pad,
		      unsigned auth_cipher, uint16_t blocksize)
{
	/* pad is the LH pad the user wants us to add. Besides
	 * this LH pad, we only add minimal padding
	 */
	unsigned int pre_length = data_size + hash_size + *pad;
	unsigned int length, new_pad;

	new_pad = (uint8_t) (blocksize - (pre_length % blocksize)) + *pad;

	if (new_pad > 255)
		new_pad -= blocksize;
	*pad = new_pad;

	length = data_size + hash_size + *pad;

	if (_gnutls_version_has_explicit_iv(ver))
		length += blocksize;	/* for the IV */

	return length;
}

inline static int
calc_enc_length_stream(gnutls_session_t session, int data_size,
		       int hash_size, unsigned auth_cipher,
		       unsigned exp_iv_size)
{
	unsigned int length;

	length = data_size + hash_size;
	if (auth_cipher)
		length += exp_iv_size;

	return length;
}

#define MAX_PREAMBLE_SIZE 16

/* generates the authentication data (data to be hashed only
 * and are not to be sent). Returns their size.
 */
static inline int
make_preamble(uint8_t * uint64_data, uint8_t type, unsigned int length,
	      const version_entry_st * ver, uint8_t * preamble)
{
	uint8_t *p = preamble;
	uint16_t c_length;

	c_length = _gnutls_conv_uint16(length);

	memcpy(p, uint64_data, 8);
	p += 8;
	*p = type;
	p++;
	if (ver->id != GNUTLS_SSL3) {	/* TLS protocols */
		*p = ver->major;
		p++;
		*p = ver->minor;
		p++;
	}
	memcpy(p, &c_length, 2);
	p += 2;
	return p - preamble;
}

/* This is the actual encryption 
 * Encrypts the given compressed datum, and puts the result to cipher_data,
 * which has cipher_size size.
 * return the actual encrypted data length.
 */
static int
compressed_to_ciphertext(gnutls_session_t session,
			 uint8_t * cipher_data, int cipher_size,
			 gnutls_datum_t * compressed,
			 size_t min_pad,
			 content_type_t type,
			 record_parameters_st * params)
{
	uint8_t pad;
	int length, ret;
	uint8_t preamble[MAX_PREAMBLE_SIZE];
	int preamble_size;
	int tag_size =
	    _gnutls_auth_cipher_tag_len(&params->write.cipher_state);
	int blocksize = _gnutls_cipher_get_block_size(params->cipher);
	unsigned block_algo = _gnutls_cipher_is_block(params->cipher);
	uint8_t *data_ptr;
	const version_entry_st *ver = get_version(session);
	int explicit_iv = _gnutls_version_has_explicit_iv(ver);
	int auth_cipher =
	    _gnutls_auth_cipher_is_aead(&params->write.cipher_state);
	uint8_t nonce[MAX_CIPHER_BLOCK_SIZE];
	unsigned iv_size, imp_iv_size, exp_iv_size;

	if (unlikely(ver == NULL))
		return gnutls_assert_val(GNUTLS_E_INTERNAL_ERROR);

	iv_size = _gnutls_cipher_get_iv_size(params->cipher);
	imp_iv_size = _gnutls_cipher_get_implicit_iv_size(params->cipher);
	exp_iv_size = _gnutls_cipher_get_explicit_iv_size(params->cipher);

	_gnutls_hard_log("ENC[%p]: cipher: %s, MAC: %s, Epoch: %u\n",
			 session, _gnutls_cipher_get_name(params->cipher),
			 _gnutls_mac_get_name(params->mac),
			 (unsigned int) params->epoch);

	preamble_size =
	    make_preamble(UINT64DATA
			  (params->write.sequence_number),
			  type, compressed->size, ver, preamble);

	/* Calculate the encrypted length (padding etc.)
	 */
	if (block_algo == CIPHER_BLOCK) {
		/* Call _gnutls_rnd() once. Get data used for the IV
		 */
		ret = _gnutls_rnd(GNUTLS_RND_NONCE, nonce, blocksize);
		if (ret < 0)
			return gnutls_assert_val(ret);

		pad = min_pad;

		length =
		    calc_enc_length_block(session, ver, compressed->size,
					  tag_size, &pad, auth_cipher,
					  blocksize);
	} else {
		pad = 0;
		length =
		    calc_enc_length_stream(session, compressed->size,
					   tag_size, auth_cipher, exp_iv_size);
	}

	if (length < 0)
		return gnutls_assert_val(length);

	/* copy the encrypted data to cipher_data.
	 */
	if (cipher_size < length)
		return gnutls_assert_val(GNUTLS_E_INTERNAL_ERROR);

	data_ptr = cipher_data;

	if (explicit_iv) {	/* TLS 1.1 or later */

		if (block_algo == CIPHER_BLOCK) {
			/* copy the random IV.
			 */
			memcpy(data_ptr, nonce, blocksize);
			_gnutls_auth_cipher_setiv(&params->write.
						  cipher_state, data_ptr,
						  blocksize);

			data_ptr += blocksize;
			cipher_data += blocksize;
		} else if (auth_cipher) {
			/* Values in AEAD are pretty fixed in TLS 1.2 for 128-bit block
			 */
			if (params->write.IV.data == NULL
			    || params->write.IV.size !=
			    imp_iv_size)
				return
				    gnutls_assert_val
				    (GNUTLS_E_INTERNAL_ERROR);

			/* Instead of generating a new nonce on every packet, we use the
			 * write.sequence_number (It is a MAY on RFC 5288).
			 */
			memcpy(nonce, params->write.IV.data,
			       params->write.IV.size);
			memcpy(&nonce[imp_iv_size],
			       UINT64DATA(params->write.sequence_number),
			       8);

			_gnutls_auth_cipher_setiv(&params->write.
						  cipher_state, nonce,
						  imp_iv_size +
						  exp_iv_size);

			/* copy the explicit part */
			memcpy(data_ptr, &nonce[imp_iv_size],
			       exp_iv_size);

			data_ptr += exp_iv_size;
			cipher_data += exp_iv_size;
		} else if (iv_size > 0)
			_gnutls_auth_cipher_setiv(&params->write.
						  cipher_state,
						  UINT64DATA(params->write.
							     sequence_number),
						  8);
	} else {
		/* AEAD ciphers have an explicit IV. Shouldn't be used otherwise.
		 */
		if (auth_cipher)
			return gnutls_assert_val(GNUTLS_E_INTERNAL_ERROR);
		else if (block_algo == CIPHER_STREAM && iv_size > 0)
			_gnutls_auth_cipher_setiv(&params->write.
						  cipher_state,
						  UINT64DATA(params->write.
							     sequence_number),
						  8);
	}

	_gnutls_auth_cipher_set_mac_nonce(&params->write.cipher_state,
					  UINT64DATA(params->write.
						     sequence_number), 8);

	/* add the authenticate data */
	ret =
	    _gnutls_auth_cipher_add_auth(&params->write.cipher_state,
					 preamble, preamble_size);
	if (ret < 0)
		return gnutls_assert_val(ret);

	/* Actual encryption.
	 */
	ret =
	    _gnutls_auth_cipher_encrypt2_tag(&params->write.cipher_state,
					     compressed->data,
					     compressed->size, cipher_data,
					     cipher_size, pad);
	if (ret < 0)
		return gnutls_assert_val(ret);

	return length;
}

static void dummy_wait(record_parameters_st * params,
		       gnutls_datum_t * plaintext, unsigned pad_failed,
		       unsigned int pad, unsigned total)
{
	/* this hack is only needed on CBC ciphers */
	if (_gnutls_cipher_is_block(params->cipher) == CIPHER_BLOCK) {
		unsigned len;

		/* force an additional hash compression function evaluation to prevent timing 
		 * attacks that distinguish between wrong-mac + correct pad, from wrong-mac + incorrect pad.
		 */
		if (pad_failed == 0 && pad > 0) {
			len = _gnutls_mac_block_size(params->mac);
			if (len > 0) {
				/* This is really specific to the current hash functions.
				 * It should be removed once a protocol fix is in place.
				 */
				if ((pad + total) % len > len - 9
				    && total % len <= len - 9) {
					if (len < plaintext->size)
						_gnutls_auth_cipher_add_auth
						    (&params->read.
						     cipher_state,
						     plaintext->data, len);
					else
						_gnutls_auth_cipher_add_auth
						    (&params->read.
						     cipher_state,
						     plaintext->data,
						     plaintext->size);
				}
			}
		}
	}
}

/* Deciphers the ciphertext packet, and puts the result to compress_data, of compress_size.
 * Returns the actual compressed packet size.
 */
static int
ciphertext_to_compressed(gnutls_session_t session,
			 gnutls_datum_t * ciphertext,
			 gnutls_datum_t * compressed,
			 uint8_t type, record_parameters_st * params,
			 uint64 * sequence)
{
	uint8_t tag[MAX_HASH_SIZE];
	const uint8_t *tag_ptr;
	unsigned int pad = 0, i;
	int length, length_to_decrypt;
	uint16_t blocksize;
	int ret;
	unsigned int tmp_pad_failed = 0;
	unsigned int pad_failed = 0;
	uint8_t preamble[MAX_PREAMBLE_SIZE];
	unsigned int preamble_size;
	const version_entry_st *ver = get_version(session);
	unsigned int tag_size =
	    _gnutls_auth_cipher_tag_len(&params->read.cipher_state);
	unsigned int explicit_iv = _gnutls_version_has_explicit_iv(ver);
	unsigned imp_iv_size, iv_size, exp_iv_size;

	if (unlikely(ver == NULL))
		return gnutls_assert_val(GNUTLS_E_INTERNAL_ERROR);

	imp_iv_size = _gnutls_cipher_get_implicit_iv_size(params->cipher);
	exp_iv_size = _gnutls_cipher_get_explicit_iv_size(params->cipher);
	iv_size = _gnutls_cipher_get_iv_size(params->cipher);
	blocksize = _gnutls_cipher_get_block_size(params->cipher);

	/* actual decryption (inplace)
	 */
	switch (_gnutls_cipher_is_block(params->cipher)) {
	case CIPHER_STREAM:
		/* The way AEAD ciphers are defined in RFC5246, it allows
		 * only stream ciphers.
		 */
		if (explicit_iv
		    && _gnutls_auth_cipher_is_aead(&params->read.
						   cipher_state)) {
			uint8_t nonce[blocksize];
			/* Values in AEAD are pretty fixed in TLS 1.2 for 128-bit block
			 */
			if (unlikely
			    (params->read.IV.data == NULL
			     || params->read.IV.size != 4))
				return
				    gnutls_assert_val
				    (GNUTLS_E_INTERNAL_ERROR);

			if (unlikely
			    (ciphertext->size <
			     tag_size + exp_iv_size))
				return
				    gnutls_assert_val
				    (GNUTLS_E_UNEXPECTED_PACKET_LENGTH);

			memcpy(nonce, params->read.IV.data,
			       imp_iv_size);
			memcpy(&nonce[imp_iv_size],
			       ciphertext->data, exp_iv_size);

			_gnutls_auth_cipher_setiv(&params->read.
						  cipher_state, nonce,
						  exp_iv_size +
						  imp_iv_size);

			ciphertext->data += exp_iv_size;
			ciphertext->size -= exp_iv_size;

			length = length_to_decrypt =
			    ciphertext->size - tag_size;
			tag_ptr = ciphertext->data + length_to_decrypt;
		} else if (iv_size > 0) {	/* a stream cipher with explicit IV */
			_gnutls_auth_cipher_setiv(&params->read.
						  cipher_state,
						  UINT64DATA(*sequence),
						  8);
			length_to_decrypt = ciphertext->size;
			length = ciphertext->size - tag_size;
			tag_ptr = compressed->data + length;
		} else {
			if (unlikely(ciphertext->size < tag_size))
				return
				    gnutls_assert_val
				    (GNUTLS_E_UNEXPECTED_PACKET_LENGTH);

			length_to_decrypt = ciphertext->size;
			length = ciphertext->size - tag_size;
			tag_ptr = compressed->data + length;
		}


		/* Pass the type, version, length and compressed through
		 * MAC.
		 */
		preamble_size =
		    make_preamble(UINT64DATA(*sequence), type,
				  length, ver, preamble);

		_gnutls_auth_cipher_set_mac_nonce(&params->read.
						  cipher_state,
						  UINT64DATA(*sequence),
						  8);
		ret =
		    _gnutls_auth_cipher_add_auth(&params->read.
						 cipher_state, preamble,
						 preamble_size);
		if (unlikely(ret < 0))
			return gnutls_assert_val(ret);

		if (unlikely
		    ((unsigned) length_to_decrypt > compressed->size)) {
			_gnutls_audit_log(session,
					  "Received %u bytes, while expecting less than %u\n",
					  (unsigned int) length_to_decrypt,
					  (unsigned int) compressed->size);
			return
			    gnutls_assert_val(GNUTLS_E_DECRYPTION_FAILED);
		}

		ret =
		    _gnutls_auth_cipher_decrypt2(&params->read.
						 cipher_state,
						 ciphertext->data,
						 length_to_decrypt,
						 compressed->data,
						 compressed->size);

		if (unlikely(ret < 0))
			return gnutls_assert_val(ret);

		break;
	case CIPHER_BLOCK:
		if (unlikely
		    (ciphertext->size < blocksize
		     || (ciphertext->size % blocksize != 0)))
			return
			    gnutls_assert_val
			    (GNUTLS_E_UNEXPECTED_PACKET_LENGTH);

		/* ignore the IV in TLS 1.1+
		 */
		if (explicit_iv) {
			_gnutls_auth_cipher_setiv(&params->read.
						  cipher_state,
						  ciphertext->data,
						  blocksize);

			ciphertext->size -= blocksize;
			ciphertext->data += blocksize;
		}

		if (unlikely(ciphertext->size < tag_size + 1))
			return
			    gnutls_assert_val(GNUTLS_E_DECRYPTION_FAILED);

		/* we don't use the auth_cipher interface here, since
		 * TLS with block ciphers is impossible to be used under such
		 * an API. (the length of plaintext is required to calculate
		 * auth_data, but it is not available before decryption).
		 */
		if (unlikely(ciphertext->size > compressed->size))
			return
			    gnutls_assert_val(GNUTLS_E_DECRYPTION_FAILED);

		ret =
		    _gnutls_cipher_decrypt2(&params->read.cipher_state.
					    cipher, ciphertext->data,
					    ciphertext->size,
					    compressed->data,
					    compressed->size);
		if (unlikely(ret < 0))
			return gnutls_assert_val(ret);

		pad = compressed->data[ciphertext->size - 1];	/* pad */

		/* Check the pading bytes (TLS 1.x). 
		 * Note that we access all 256 bytes of ciphertext for padding check
		 * because there is a timing channel in that memory access (in certain CPUs).
		 */
		if (ver->id != GNUTLS_SSL3)
			for (i = 2; i <= MIN(256, ciphertext->size); i++) {
				tmp_pad_failed |=
				    (compressed->
				     data[ciphertext->size - i] != pad);
				pad_failed |=
				    ((i <= (1 + pad)) & (tmp_pad_failed));
			}

		if (unlikely
		    (pad_failed != 0
		     || (1 + pad > ((int) ciphertext->size - tag_size)))) {
			/* We do not fail here. We check below for the
			 * the pad_failed. If zero means success.
			 */
			pad_failed = 1;
			pad = 0;
		}

		length = ciphertext->size - tag_size - pad - 1;
		tag_ptr = &compressed->data[length];

		/* Pass the type, version, length and compressed through
		 * MAC.
		 */
		preamble_size =
		    make_preamble(UINT64DATA(*sequence), type,
				  length, ver, preamble);

		_gnutls_auth_cipher_set_mac_nonce(&params->read.
						  cipher_state,
						  UINT64DATA(*sequence),
						  8);
		ret =
		    _gnutls_auth_cipher_add_auth(&params->read.
						 cipher_state, preamble,
						 preamble_size);
		if (unlikely(ret < 0))
			return gnutls_assert_val(ret);

		ret =
		    _gnutls_auth_cipher_add_auth(&params->read.
						 cipher_state,
						 compressed->data, length);
		if (unlikely(ret < 0))
			return gnutls_assert_val(ret);

		break;
	default:
		return gnutls_assert_val(GNUTLS_E_INTERNAL_ERROR);
	}

	ret =
	    _gnutls_auth_cipher_tag(&params->read.cipher_state, tag,
				    tag_size);
	if (unlikely(ret < 0))
		return gnutls_assert_val(ret);

	/* Here there could be a timing leakage in CBC ciphersuites that
	 * could be exploited if the cost of a successful memcmp is high. 
	 * A constant time memcmp would help there, but it is not easy to maintain
	 * against compiler optimizations. Currently we rely on the fact that
	 * a memcmp comparison is negligible over the crypto operations.
	 */
	if (unlikely
	    (memcmp(tag, tag_ptr, tag_size) != 0 || pad_failed != 0)) {
		/* HMAC was not the same. */
		dummy_wait(params, compressed, pad_failed, pad,
			   length + preamble_size);

		return gnutls_assert_val(GNUTLS_E_DECRYPTION_FAILED);
	}

	return length;
}
