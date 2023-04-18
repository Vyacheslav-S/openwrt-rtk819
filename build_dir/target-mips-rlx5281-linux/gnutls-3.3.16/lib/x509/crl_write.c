/*
 * Copyright (C) 2003-2012 Free Software Foundation, Inc.
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

/* This file contains functions to handle CRL generation.
 */

#include <gnutls_int.h>

#include <gnutls_datum.h>
#include <gnutls_global.h>
#include <gnutls_errors.h>
#include <common.h>
#include <gnutls_x509.h>
#include <x509_b64.h>
#include <x509_int.h>
#include <libtasn1.h>

static void disable_optional_stuff(gnutls_x509_crl_t crl);

/**
 * gnutls_x509_crl_set_version:
 * @crl: should contain a gnutls_x509_crl_t structure
 * @version: holds the version number. For CRLv1 crls must be 1.
 *
 * This function will set the version of the CRL. This
 * must be one for CRL version 1, and so on. The CRLs generated
 * by gnutls should have a version number of 2.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 **/
int
gnutls_x509_crl_set_version(gnutls_x509_crl_t crl, unsigned int version)
{
	int result;
	uint8_t null = version & 0xFF;

	if (crl == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	if (null > 0)
		null -= 1;

	result =
	    asn1_write_value(crl->crl, "tbsCertList.version", &null, 1);
	if (result != ASN1_SUCCESS) {
		gnutls_assert();
		return _gnutls_asn2err(result);
	}

	return 0;
}

/**
 * gnutls_x509_crl_sign2:
 * @crl: should contain a gnutls_x509_crl_t structure
 * @issuer: is the certificate of the certificate issuer
 * @issuer_key: holds the issuer's private key
 * @dig: The message digest to use. GNUTLS_DIG_SHA1 is the safe choice unless you know what you're doing.
 * @flags: must be 0
 *
 * This function will sign the CRL with the issuer's private key, and
 * will copy the issuer's information into the CRL.
 *
 * This must be the last step in a certificate CRL since all
 * the previously set parameters are now signed.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 *
 **/
int
gnutls_x509_crl_sign2(gnutls_x509_crl_t crl, gnutls_x509_crt_t issuer,
		      gnutls_x509_privkey_t issuer_key,
		      gnutls_digest_algorithm_t dig, unsigned int flags)
{
	int result;
	gnutls_privkey_t privkey;

	if (crl == NULL || issuer == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	result = gnutls_privkey_init(&privkey);
	if (result < 0) {
		gnutls_assert();
		return result;
	}

	result = gnutls_privkey_import_x509(privkey, issuer_key, 0);
	if (result < 0) {
		gnutls_assert();
		goto fail;
	}

	result =
	    gnutls_x509_crl_privkey_sign(crl, issuer, privkey, dig, flags);
	if (result < 0) {
		gnutls_assert();
		goto fail;
	}

	result = 0;

      fail:
	gnutls_privkey_deinit(privkey);

	return result;
}

/**
 * gnutls_x509_crl_sign:
 * @crl: should contain a gnutls_x509_crl_t structure
 * @issuer: is the certificate of the certificate issuer
 * @issuer_key: holds the issuer's private key
 *
 * This function is the same a gnutls_x509_crl_sign2() with no flags, and
 * SHA1 as the hash algorithm.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 *
 * Deprecated: Use gnutls_x509_crl_privkey_sign().
 */
int
gnutls_x509_crl_sign(gnutls_x509_crl_t crl, gnutls_x509_crt_t issuer,
		     gnutls_x509_privkey_t issuer_key)
{
	return gnutls_x509_crl_sign2(crl, issuer, issuer_key,
				     GNUTLS_DIG_SHA1, 0);
}

/**
 * gnutls_x509_crl_set_this_update:
 * @crl: should contain a gnutls_x509_crl_t structure
 * @act_time: The actual time
 *
 * This function will set the time this CRL was issued.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 **/
int gnutls_x509_crl_set_this_update(gnutls_x509_crl_t crl, time_t act_time)
{
	if (crl == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	return _gnutls_x509_set_time(crl->crl, "tbsCertList.thisUpdate",
				     act_time, 0);
}

/**
 * gnutls_x509_crl_set_next_update:
 * @crl: should contain a gnutls_x509_crl_t structure
 * @exp_time: The actual time
 *
 * This function will set the time this CRL will be updated.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 **/
int gnutls_x509_crl_set_next_update(gnutls_x509_crl_t crl, time_t exp_time)
{
	if (crl == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}
	return _gnutls_x509_set_time(crl->crl, "tbsCertList.nextUpdate",
				     exp_time, 0);
}

/**
 * gnutls_x509_crl_set_crt_serial:
 * @crl: should contain a gnutls_x509_crl_t structure
 * @serial: The revoked certificate's serial number
 * @serial_size: Holds the size of the serial field.
 * @revocation_time: The time this certificate was revoked
 *
 * This function will set a revoked certificate's serial number to the CRL.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 **/
int
gnutls_x509_crl_set_crt_serial(gnutls_x509_crl_t crl,
			       const void *serial, size_t serial_size,
			       time_t revocation_time)
{
	int ret;

	if (crl == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	ret =
	    asn1_write_value(crl->crl, "tbsCertList.revokedCertificates",
			     "NEW", 1);
	if (ret != ASN1_SUCCESS) {
		gnutls_assert();
		return _gnutls_asn2err(ret);
	}

	ret =
	    asn1_write_value(crl->crl,
			     "tbsCertList.revokedCertificates.?LAST.userCertificate",
			     serial, serial_size);
	if (ret != ASN1_SUCCESS) {
		gnutls_assert();
		return _gnutls_asn2err(ret);
	}

	ret =
	    _gnutls_x509_set_time(crl->crl,
				  "tbsCertList.revokedCertificates.?LAST.revocationDate",
				  revocation_time, 0);
	if (ret < 0) {
		gnutls_assert();
		return ret;
	}

	ret =
	    asn1_write_value(crl->crl,
			     "tbsCertList.revokedCertificates.?LAST.crlEntryExtensions",
			     NULL, 0);
	if (ret != ASN1_SUCCESS) {
		gnutls_assert();
		return _gnutls_asn2err(ret);
	}

	return 0;
}

/**
 * gnutls_x509_crl_set_crt:
 * @crl: should contain a gnutls_x509_crl_t structure
 * @crt: a certificate of type #gnutls_x509_crt_t with the revoked certificate
 * @revocation_time: The time this certificate was revoked
 *
 * This function will set a revoked certificate's serial number to the CRL.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 **/
int
gnutls_x509_crl_set_crt(gnutls_x509_crl_t crl, gnutls_x509_crt_t crt,
			time_t revocation_time)
{
	int ret;
	uint8_t serial[128];
	size_t serial_size;

	if (crl == NULL || crt == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	serial_size = sizeof(serial);
	ret = gnutls_x509_crt_get_serial(crt, serial, &serial_size);
	if (ret < 0) {
		gnutls_assert();
		return ret;
	}

	ret =
	    gnutls_x509_crl_set_crt_serial(crl, serial, serial_size,
					   revocation_time);
	if (ret < 0) {
		gnutls_assert();
		return _gnutls_asn2err(ret);
	}

	return 0;
}


/* If OPTIONAL fields have not been initialized then
 * disable them.
 */
static void disable_optional_stuff(gnutls_x509_crl_t crl)
{

	if (crl->use_extensions == 0) {
		asn1_write_value(crl->crl, "tbsCertList.crlExtensions",
				 NULL, 0);
	}

	return;
}

/**
 * gnutls_x509_crl_set_authority_key_id:
 * @crl: a CRL of type #gnutls_x509_crl_t
 * @id: The key ID
 * @id_size: Holds the size of the serial field.
 *
 * This function will set the CRL's authority key ID extension.  Only
 * the keyIdentifier field can be set with this function. This may
 * be used by an authority that holds multiple private keys, to distinguish
 * the used key.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 *
 * Since: 2.8.0
 **/
int
gnutls_x509_crl_set_authority_key_id(gnutls_x509_crl_t crl,
				     const void *id, size_t id_size)
{
	int result;
	gnutls_datum_t old_id, der_data;
	unsigned int critical;

	if (crl == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	/* Check if the extension already exists.
	 */
	result =
	    _gnutls_x509_crl_get_extension(crl, "2.5.29.35", 0, &old_id,
					   &critical);

	if (result >= 0)
		_gnutls_free_datum(&old_id);
	if (result != GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	/* generate the extension.
	 */
	result = _gnutls_x509_ext_gen_auth_key_id(id, id_size, &der_data);
	if (result < 0) {
		gnutls_assert();
		return result;
	}

	result =
	    _gnutls_x509_crl_set_extension(crl, "2.5.29.35", &der_data, 0);

	_gnutls_free_datum(&der_data);

	if (result < 0) {
		gnutls_assert();
		return result;
	}

	crl->use_extensions = 1;

	return 0;
}

/**
 * gnutls_x509_crl_set_number:
 * @crl: a CRL of type #gnutls_x509_crl_t
 * @nr: The CRL number
 * @nr_size: Holds the size of the nr field.
 *
 * This function will set the CRL's number extension. This
 * is to be used as a unique and monotonic number assigned to
 * the CRL by the authority.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 *
 * Since: 2.8.0
 **/
int
gnutls_x509_crl_set_number(gnutls_x509_crl_t crl,
			   const void *nr, size_t nr_size)
{
	int result;
	gnutls_datum_t old_id, der_data;
	unsigned int critical;

	if (crl == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	/* Check if the extension already exists.
	 */
	result =
	    _gnutls_x509_crl_get_extension(crl, "2.5.29.20", 0, &old_id,
					   &critical);

	if (result >= 0)
		_gnutls_free_datum(&old_id);
	if (result != GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	/* generate the extension.
	 */
	result = _gnutls_x509_ext_gen_number(nr, nr_size, &der_data);
	if (result < 0) {
		gnutls_assert();
		return result;
	}

	result =
	    _gnutls_x509_crl_set_extension(crl, "2.5.29.20", &der_data, 0);

	_gnutls_free_datum(&der_data);

	if (result < 0) {
		gnutls_assert();
		return result;
	}

	crl->use_extensions = 1;

	return 0;
}

/**
 * gnutls_x509_crl_privkey_sign:
 * @crl: should contain a gnutls_x509_crl_t structure
 * @issuer: is the certificate of the certificate issuer
 * @issuer_key: holds the issuer's private key
 * @dig: The message digest to use. GNUTLS_DIG_SHA1 is the safe choice unless you know what you're doing.
 * @flags: must be 0
 *
 * This function will sign the CRL with the issuer's private key, and
 * will copy the issuer's information into the CRL.
 *
 * This must be the last step in a certificate CRL since all
 * the previously set parameters are now signed.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned, otherwise a
 *   negative error value.
 *
 * Since 2.12.0
 **/
int
gnutls_x509_crl_privkey_sign(gnutls_x509_crl_t crl,
			     gnutls_x509_crt_t issuer,
			     gnutls_privkey_t issuer_key,
			     gnutls_digest_algorithm_t dig,
			     unsigned int flags)
{
	int result;

	if (crl == NULL || issuer == NULL) {
		gnutls_assert();
		return GNUTLS_E_INVALID_REQUEST;
	}

	/* disable all the unneeded OPTIONAL fields.
	 */
	disable_optional_stuff(crl);

	result = _gnutls_x509_pkix_sign(crl->crl, "tbsCertList",
					dig, issuer, issuer_key);
	if (result < 0) {
		gnutls_assert();
		return result;
	}

	return 0;
}
