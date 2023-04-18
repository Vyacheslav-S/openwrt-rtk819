/*
 * Copyright (C) 2000-2012 Free Software Foundation, Inc.
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

/* Functions that relate to base64 encoding and decoding.
 */

#include "gnutls_int.h"
#include "gnutls_errors.h"
#include <gnutls_datum.h>
#include <x509_b64.h>
#include <base64.h>

#define INCR(what, size, max_len) \
	do { \
	what+=size; \
	if (what > max_len) { \
		gnutls_assert(); \
		gnutls_free( result->data); result->data = NULL; \
		return GNUTLS_E_INTERNAL_ERROR; \
	} \
	} while(0)

/* encodes data and puts the result into result (locally allocated)
 * The result_size (including the null terminator) is the return value.
 */
int
_gnutls_fbase64_encode(const char *msg, const uint8_t * data,
		       size_t data_size, gnutls_datum_t * result)
{
	int tmp;
	unsigned int i;
	char tmpres[66];
	uint8_t *ptr;
	char top[80];
	char bottom[80];
	size_t size, max, bytes;
	int pos, top_len, bottom_len;

	if (msg == NULL || strlen(msg) > 50) {
		gnutls_assert();
		return GNUTLS_E_BASE64_ENCODING_ERROR;
	}

	_gnutls_str_cpy(top, sizeof(top), "-----BEGIN ");
	_gnutls_str_cat(top, sizeof(top), msg);
	_gnutls_str_cat(top, sizeof(top), "-----\n");

	_gnutls_str_cpy(bottom, sizeof(bottom), "-----END ");
	_gnutls_str_cat(bottom, sizeof(bottom), msg);
	_gnutls_str_cat(bottom, sizeof(bottom), "-----\n");

	top_len = strlen(top);
	bottom_len = strlen(bottom);

	max = B64FSIZE(top_len + bottom_len, data_size);

	result->data = gnutls_malloc(max + 1);
	if (result->data == NULL) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	bytes = pos = 0;
	INCR(bytes, top_len, max);
	pos = top_len;

	memcpy(result->data, top, top_len);

	for (i = 0; i < data_size; i += 48) {
		if (data_size - i < 48)
			tmp = data_size - i;
		else
			tmp = 48;

		base64_encode((void *) &data[i], tmp, tmpres,
			      sizeof(tmpres));
		size = strlen(tmpres);

		INCR(bytes, size + 1, max);
		ptr = &result->data[pos];

		memcpy(ptr, tmpres, size);
		ptr += size;
		*ptr++ = '\n';

		pos += size + 1;
	}

	INCR(bytes, bottom_len, max);

	memcpy(&result->data[bytes - bottom_len], bottom, bottom_len);
	result->data[bytes] = 0;
	result->size = bytes;

	return max + 1;
}

/**
 * gnutls_pem_base64_encode:
 * @msg: is a message to be put in the header
 * @data: contain the raw data
 * @result: the place where base64 data will be copied
 * @result_size: holds the size of the result
 *
 * This function will convert the given data to printable data, using
 * the base64 encoding. This is the encoding used in PEM messages.
 *
 * The output string will be null terminated, although the size will
 * not include the terminating null.
 *
 * Returns: On success %GNUTLS_E_SUCCESS (0) is returned,
 *   %GNUTLS_E_SHORT_MEMORY_BUFFER is returned if the buffer given is
 *   not long enough, or 0 on success.
 **/
int
gnutls_pem_base64_encode(const char *msg, const gnutls_datum_t * data,
			 char *result, size_t * result_size)
{
	gnutls_datum_t res;
	int ret;

	ret = _gnutls_fbase64_encode(msg, data->data, data->size, &res);
	if (ret < 0)
		return ret;

	if (result == NULL || *result_size < (unsigned) res.size) {
		gnutls_free(res.data);
		*result_size = res.size + 1;
		return GNUTLS_E_SHORT_MEMORY_BUFFER;
	} else {
		memcpy(result, res.data, res.size);
		gnutls_free(res.data);
		*result_size = res.size;
	}

	return 0;
}

/**
 * gnutls_pem_base64_encode_alloc:
 * @msg: is a message to be put in the encoded header
 * @data: contains the raw data
 * @result: will hold the newly allocated encoded data
 *
 * This function will convert the given data to printable data, using
 * the base64 encoding.  This is the encoding used in PEM messages.
 * This function will allocate the required memory to hold the encoded
 * data.
 *
 * You should use gnutls_free() to free the returned data.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise
 *   an error code is returned.
 **/
int
gnutls_pem_base64_encode_alloc(const char *msg,
			       const gnutls_datum_t * data,
			       gnutls_datum_t * result)
{
	int ret;

	if (result == NULL)
		return gnutls_assert_val(GNUTLS_E_INVALID_REQUEST);

	ret = _gnutls_fbase64_encode(msg, data->data, data->size, result);
	if (ret < 0)
		return gnutls_assert_val(ret);

	return 0;
}

/* copies data to result but removes newlines and <CR>
 * returns the size of the data copied.
 */
inline static int
cpydata(const uint8_t * data, int data_size, gnutls_datum_t * result)
{
	int i, j;

	result->data = gnutls_malloc(data_size + 1);
	if (result->data == NULL)
		return GNUTLS_E_MEMORY_ERROR;

	for (j = i = 0; i < data_size; i++) {
		if (data[i] == '\n' || data[i] == '\r' || data[i] == ' '
		    || data[i] == '\t')
			continue;
		else if (data[i] == '-')
			break;
		result->data[j] = data[i];
		j++;
	}

	result->size = j;
	result->data[j] = 0;
	return j;
}

/* decodes data and puts the result into result (locally allocated)
 * The result_size is the return value
 */
int
_gnutls_base64_decode(const uint8_t * data, size_t data_size,
		      gnutls_datum_t * result)
{
	unsigned int i;
	int pos, tmp, est, ret;
	uint8_t tmpres[48];
	size_t tmpres_size, decode_size;
	gnutls_datum_t pdata;

	ret = cpydata(data, data_size, &pdata);
	if (ret < 0) {
		gnutls_assert();
		return ret;
	}

	est = ((data_size * 3) / 4) + 1;

	result->data = gnutls_malloc(est);
	if (result->data == NULL)
		return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

	pos = 0;
	for (i = 0; i < pdata.size; i += 64) {
		if (pdata.size - i < 64)
			decode_size = pdata.size - i;
		else
			decode_size = 64;

		tmpres_size = sizeof(tmpres);
		tmp =
		    base64_decode((void *) &pdata.data[i], decode_size,
				  (void *) tmpres, &tmpres_size);
		if (tmp == 0) {
			gnutls_assert();
			gnutls_free(result->data);
			result->data = NULL;
			ret = GNUTLS_E_PARSING_ERROR;
			goto cleanup;
		}
		memcpy(&result->data[pos], tmpres, tmpres_size);
		pos += tmpres_size;
	}

	result->size = pos;

	ret = pos;

      cleanup:
	gnutls_free(pdata.data);
	return ret;
}


/* Searches the given string for ONE PEM encoded certificate, and
 * stores it in the result.
 *
 * The result_size is the return value
 */
#define ENDSTR "-----"
int
_gnutls_fbase64_decode(const char *header, const uint8_t * data,
		       size_t data_size, gnutls_datum_t * result)
{
	int ret;
	static const char top[] = "-----BEGIN ";
	static const char bottom[] = "-----END ";
	uint8_t *rdata, *kdata;
	int rdata_size;
	char pem_header[128];

	_gnutls_str_cpy(pem_header, sizeof(pem_header), top);
	if (header != NULL)
		_gnutls_str_cat(pem_header, sizeof(pem_header), header);

	rdata = memmem(data, data_size, pem_header, strlen(pem_header));

	if (rdata == NULL) {
		gnutls_assert();
		_gnutls_hard_log("Could not find '%s'\n", pem_header);
		return GNUTLS_E_BASE64_UNEXPECTED_HEADER_ERROR;
	}

	data_size -= (unsigned long int) rdata - (unsigned long int) data;

	if (data_size < 4 + strlen(bottom)) {
		gnutls_assert();
		return GNUTLS_E_BASE64_DECODING_ERROR;
	}

	kdata =
	    memmem(rdata + 1, data_size - 1, ENDSTR, sizeof(ENDSTR) - 1);
	/* allow CR as well.
	 */
	if (kdata == NULL) {
		gnutls_assert();
		_gnutls_hard_log("Could not find '%s'\n", ENDSTR);
		return GNUTLS_E_BASE64_DECODING_ERROR;
	}
	data_size -= strlen(ENDSTR);
	data_size -= (unsigned long int) kdata - (unsigned long int) rdata;

	rdata = kdata + strlen(ENDSTR);

	/* position is now after the ---BEGIN--- headers */

	kdata = memmem(rdata, data_size, bottom, strlen(bottom));
	if (kdata == NULL) {
		gnutls_assert();
		return GNUTLS_E_BASE64_DECODING_ERROR;
	}

	/* position of kdata is before the ----END--- footer 
	 */
	rdata_size = (unsigned long int) kdata - (unsigned long int) rdata;

	if (rdata_size < 4) {
		gnutls_assert();
		return GNUTLS_E_BASE64_DECODING_ERROR;
	}

	if ((ret = _gnutls_base64_decode(rdata, rdata_size, result)) < 0) {
		gnutls_assert();
		return GNUTLS_E_BASE64_DECODING_ERROR;
	}

	return ret;
}

/**
 * gnutls_pem_base64_decode:
 * @header: A null terminated string with the PEM header (eg. CERTIFICATE)
 * @b64_data: contain the encoded data
 * @result: the place where decoded data will be copied
 * @result_size: holds the size of the result
 *
 * This function will decode the given encoded data.  If the header
 * given is non null this function will search for "-----BEGIN header"
 * and decode only this part.  Otherwise it will decode the first PEM
 * packet found.
 *
 * Returns: On success %GNUTLS_E_SUCCESS (0) is returned,
 *   %GNUTLS_E_SHORT_MEMORY_BUFFER is returned if the buffer given is
 *   not long enough, or 0 on success.
 **/
int
gnutls_pem_base64_decode(const char *header,
			 const gnutls_datum_t * b64_data,
			 unsigned char *result, size_t * result_size)
{
	gnutls_datum_t res;
	int ret;

	ret =
	    _gnutls_fbase64_decode(header, b64_data->data, b64_data->size,
				   &res);
	if (ret < 0)
		return gnutls_assert_val(ret);

	if (result == NULL || *result_size < (unsigned) res.size) {
		gnutls_free(res.data);
		*result_size = res.size;
		return GNUTLS_E_SHORT_MEMORY_BUFFER;
	} else {
		memcpy(result, res.data, res.size);
		gnutls_free(res.data);
		*result_size = res.size;
	}

	return 0;
}

/**
 * gnutls_pem_base64_decode_alloc:
 * @header: The PEM header (eg. CERTIFICATE)
 * @b64_data: contains the encoded data
 * @result: the place where decoded data lie
 *
 * This function will decode the given encoded data. The decoded data
 * will be allocated, and stored into result.  If the header given is
 * non null this function will search for "-----BEGIN header" and
 * decode only this part. Otherwise it will decode the first PEM
 * packet found.
 *
 * You should use gnutls_free() to free the returned data.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise
 *   an error code is returned.
 **/
int
gnutls_pem_base64_decode_alloc(const char *header,
			       const gnutls_datum_t * b64_data,
			       gnutls_datum_t * result)
{
	int ret;

	if (result == NULL)
		return gnutls_assert_val(GNUTLS_E_INVALID_REQUEST);

	ret =
	    _gnutls_fbase64_decode(header, b64_data->data, b64_data->size,
				   result);
	if (ret < 0)
		return gnutls_assert_val(ret);

	return 0;
}
