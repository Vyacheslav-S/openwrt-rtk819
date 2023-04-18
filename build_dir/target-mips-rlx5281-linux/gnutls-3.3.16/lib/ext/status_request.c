/*
 * Copyright (C) 2012 Free Software Foundation, Inc.
 *
 * Author: Simon Josefsson, Nikos Mavrogiannopoulos
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

/*
  Status Request (OCSP) TLS extension.  See RFC 6066 section 8:
  https://tools.ietf.org/html/rfc6066#section-8
*/

#include "gnutls_int.h"
#include "gnutls_errors.h"
#include <gnutls_extensions.h>
#include <ext/status_request.h>
#include <gnutls_mbuffers.h>
#include <gnutls_auth.h>
#include <auth/cert.h>
#include <gnutls_handshake.h>

#ifdef ENABLE_OCSP

typedef struct {
	gnutls_datum_t *responder_id;
	size_t responder_id_size;
	gnutls_datum_t request_extensions;
	gnutls_datum_t response;

	unsigned int expect_cstatus;
} status_request_ext_st;

/*
  From RFC 6066.  Client sends:

      struct {
          CertificateStatusType status_type;
          select (status_type) {
              case ocsp: OCSPStatusRequest;
          } request;
      } CertificateStatusRequest;

      enum { ocsp(1), (255) } CertificateStatusType;

      struct {
          ResponderID responder_id_list<0..2^16-1>;
          Extensions  request_extensions;
      } OCSPStatusRequest;

      opaque ResponderID<1..2^16-1>;
      opaque Extensions<0..2^16-1>;
*/

static void deinit_responder_id(status_request_ext_st *priv)
{
unsigned i;

	for (i = 0; i < priv->responder_id_size; i++)
		gnutls_free(priv->responder_id[i].data);

	gnutls_free(priv->responder_id);
	priv->responder_id = NULL;
	priv->responder_id_size = 0;
}


static int
client_send(gnutls_session_t session,
	    gnutls_buffer_st * extdata, status_request_ext_st * priv)
{
	int ret_len = 1 + 2;
	int ret;
	size_t i;

	ret = _gnutls_buffer_append_prefix(extdata, 8, 1);
	if (ret < 0)
		return gnutls_assert_val(ret);

	ret =
	    _gnutls_buffer_append_prefix(extdata, 16,
					 priv->responder_id_size);
	if (ret < 0)
		return gnutls_assert_val(ret);

	for (i = 0; i < priv->responder_id_size; i++) {
		if (priv->responder_id[i].size <= 0)
			return gnutls_assert_val(GNUTLS_E_INVALID_REQUEST);

		ret = _gnutls_buffer_append_data_prefix(extdata, 16,
							priv->
							responder_id[i].
							data,
							priv->
							responder_id[i].
							size);
		if (ret < 0)
			return gnutls_assert_val(ret);

		ret_len += 2 + priv->responder_id[i].size;
	}

	ret = _gnutls_buffer_append_data_prefix(extdata, 16,
						priv->request_extensions.
						data,
						priv->request_extensions.
						size);
	if (ret < 0)
		return gnutls_assert_val(ret);

	ret_len += 2 + priv->request_extensions.size;

	return ret_len;
}

static int
server_recv(gnutls_session_t session,
	    status_request_ext_st * priv,
	    const uint8_t * data, size_t size)
{
	size_t i;
	ssize_t data_size = size;

	/* minimum message is type (1) + responder_id_list (2) +
	   request_extension (2) = 5 */
	if (data_size < 5)
		return
		    gnutls_assert_val(GNUTLS_E_UNEXPECTED_PACKET_LENGTH);

	/* We ignore non-ocsp CertificateStatusType.  The spec is unclear
	   what should be done. */
	if (data[0] != 0x01) {
		gnutls_assert();
		_gnutls_handshake_log("EXT[%p]: unknown status_type %d\n",
				      session, data[0]);
		return 0;
	}
	DECR_LEN(data_size, 1);
	data++;

	priv->responder_id_size = _gnutls_read_uint16(data);

	DECR_LEN(data_size, 2);
	data += 2;

	if (data_size <= (ssize_t) (priv->responder_id_size * 2))
		return
		    gnutls_assert_val(GNUTLS_E_RECEIVED_ILLEGAL_PARAMETER);

	if (priv->responder_id != NULL)
		deinit_responder_id(priv);

	priv->responder_id = gnutls_calloc(1, priv->responder_id_size
					   * sizeof(*priv->responder_id));
	if (priv->responder_id == NULL)
		return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

	for (i = 0; i < priv->responder_id_size; i++) {
		size_t l;

		DECR_LEN(data_size, 2);

		l = _gnutls_read_uint16(data);
		data += 2;

		DECR_LEN(data_size, l);

		priv->responder_id[i].data = gnutls_malloc(l);
		if (priv->responder_id[i].data == NULL)
			return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

		memcpy(priv->responder_id[i].data, data, l);
		priv->responder_id[i].size = l;

		data += l;
	}

	return 0;
}

/*
  Servers return a certificate response along with their certificate
  by sending a "CertificateStatus" message immediately after the
  "Certificate" message (and before any "ServerKeyExchange" or
  "CertificateRequest" messages).  If a server returns a
  "CertificateStatus" message, then the server MUST have included an
  extension of type "status_request" with empty "extension_data" in
  the extended server hello.
*/

static int
server_send(gnutls_session_t session,
	    gnutls_buffer_st * extdata, status_request_ext_st * priv)
{
	int ret;
	gnutls_certificate_credentials_t cred;

	cred = (gnutls_certificate_credentials_t)
	    _gnutls_get_cred(session, GNUTLS_CRD_CERTIFICATE);
	if (cred == NULL)	/* no certificate authentication */
		return gnutls_assert_val(0);

	if (cred->ocsp_func == NULL)
		return gnutls_assert_val(GNUTLS_E_SUCCESS);

	ret =
	    cred->ocsp_func(session, cred->ocsp_func_ptr, &priv->response);
	if (ret == GNUTLS_E_NO_CERTIFICATE_STATUS)
		return 0;
	else if (ret < 0)
		return gnutls_assert_val(ret);

	return GNUTLS_E_INT_RET_0;
}

static int
client_recv(gnutls_session_t session,
	    status_request_ext_st * priv,
	    const uint8_t * data, size_t size)
{
	if (size != 0)
		return
		    gnutls_assert_val(GNUTLS_E_UNEXPECTED_PACKET_LENGTH);
	else {
		priv->expect_cstatus = 1;
		return 0;
	}
}

static int
_gnutls_status_request_send_params(gnutls_session_t session,
				   gnutls_buffer_st * extdata)
{
	extension_priv_data_t epriv;
	status_request_ext_st *priv;
	int ret;

	ret = _gnutls_ext_get_session_data(session,
					   GNUTLS_EXTENSION_STATUS_REQUEST,
					   &epriv);

	if (session->security_parameters.entity == GNUTLS_CLIENT) {
		if (ret < 0 || epriv.ptr == NULL)	/* it is ok not to have it */
			return 0;
		priv = epriv.ptr;

		return client_send(session, extdata, priv);
	} else {
		epriv.ptr = priv = gnutls_calloc(1, sizeof(*priv));
		if (priv == NULL)
			return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

		_gnutls_ext_set_session_data(session,
					     GNUTLS_EXTENSION_STATUS_REQUEST,
					     epriv);

		return server_send(session, extdata, priv);
	}
}

static int
_gnutls_status_request_recv_params(gnutls_session_t session,
				   const uint8_t * data, size_t size)
{
	extension_priv_data_t epriv;
	status_request_ext_st *priv;
	int ret;

	ret = _gnutls_ext_get_session_data(session,
					   GNUTLS_EXTENSION_STATUS_REQUEST,
					   &epriv);
	if (ret < 0 || epriv.ptr == NULL)	/* it is ok not to have it */
		return 0;

	priv = epriv.ptr;

	if (session->security_parameters.entity == GNUTLS_CLIENT)
		return client_recv(session, priv, data, size);
	else
		return server_recv(session, priv, data, size);
}

/**
 * gnutls_ocsp_status_request_enable_client:
 * @session: is a #gnutls_session_t structure.
 * @responder_id: array with #gnutls_datum_t with DER data of responder id
 * @responder_id_size: number of members in @responder_id array
 * @extensions: a #gnutls_datum_t with DER encoded OCSP extensions
 *
 * This function is to be used by clients to request OCSP response
 * from the server, using the "status_request" TLS extension.  Only
 * OCSP status type is supported. A typical server has a single
 * OCSP response cached, so @responder_id and @extensions
 * should be null.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.1.3
 **/
int
gnutls_ocsp_status_request_enable_client(gnutls_session_t session,
					 gnutls_datum_t * responder_id,
					 size_t responder_id_size,
					 gnutls_datum_t * extensions)
{
	status_request_ext_st *priv;
	extension_priv_data_t epriv;

	if (session->security_parameters.entity == GNUTLS_SERVER)
		return gnutls_assert_val(GNUTLS_E_INVALID_REQUEST);

	epriv.ptr = priv = gnutls_calloc(1, sizeof(*priv));
	if (priv == NULL)
		return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

	priv->responder_id = responder_id;
	priv->responder_id_size = responder_id_size;
	if (extensions) {
		priv->request_extensions.data = extensions->data;
		priv->request_extensions.size = extensions->size;
	}

	_gnutls_ext_set_session_data(session,
				     GNUTLS_EXTENSION_STATUS_REQUEST,
				     epriv);

	return 0;
}

/**
 * gnutls_ocsp_status_request_get:
 * @session: is a #gnutls_session_t structure.
 * @response: a #gnutls_datum_t with DER encoded OCSP response
 *
 * This function returns the OCSP status response received
 * from the TLS server. The @response should be treated as
 * constant. If no OCSP response is available then
 * %GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE is returned.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.1.3
 **/
int
gnutls_ocsp_status_request_get(gnutls_session_t session,
			       gnutls_datum_t * response)
{
	status_request_ext_st *priv;
	extension_priv_data_t epriv;
	int ret;

	if (session->security_parameters.entity == GNUTLS_SERVER)
		return gnutls_assert_val(GNUTLS_E_INVALID_REQUEST);

	ret = _gnutls_ext_get_session_data(session,
					   GNUTLS_EXTENSION_STATUS_REQUEST,
					   &epriv);
	if (ret < 0)
		return gnutls_assert_val(ret);

	priv = epriv.ptr;

	if (priv == NULL || priv->response.data == NULL)
		return
		    gnutls_assert_val
		    (GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE);

	response->data = priv->response.data;
	response->size = priv->response.size;

	return 0;
}

/**
 * gnutls_certificate_set_ocsp_status_request_function:
 * @sc: is a #gnutls_certificate_credentials_t structure.
 * @ocsp_func: function pointer to OCSP status request callback.
 * @ptr: opaque pointer passed to callback function
 *
 * This function is to be used by server to register a callback to
 * handle OCSP status requests from the client.  The callback will be
 * invoked if the client supplied a status-request OCSP extension.
 * The callback function prototype is:
 *
 * typedef int (*gnutls_status_request_ocsp_func)
 *    (gnutls_session_t session, void *ptr, gnutls_datum_t *ocsp_response);
 *
 * The callback will be invoked if the client requests an OCSP certificate
 * status.  The callback may return %GNUTLS_E_NO_CERTIFICATE_STATUS, if
 * there is no recent OCSP response. If the callback returns %GNUTLS_E_SUCCESS,
 * the server will provide the client with the ocsp_response.
 *
 * The response must be a value allocated using gnutls_malloc(), and will be
 * deinitialized when needed.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.1.3
 **/
void
gnutls_certificate_set_ocsp_status_request_function
(gnutls_certificate_credentials_t sc,
gnutls_status_request_ocsp_func ocsp_func, void *ptr)
{

	sc->ocsp_func = ocsp_func;
	sc->ocsp_func_ptr = ptr;
}

static int file_ocsp_func(gnutls_session_t session, void *ptr,
			  gnutls_datum_t * ocsp_response)
{
	int ret;
	gnutls_certificate_credentials_t sc = ptr;

	ret = gnutls_load_file(sc->ocsp_response_file, ocsp_response);
	if (ret < 0)
		return gnutls_assert_val(GNUTLS_E_NO_CERTIFICATE_STATUS);

	return 0;
}

/**
 * gnutls_certificate_set_ocsp_status_request_file:
 * @sc: is a credentials structure.
 * @response_file: a filename of the OCSP response
 * @flags: should be zero
 *
 * This function sets the filename of an OCSP response, that will be
 * sent to the client if requests an OCSP certificate status. This is
 * a convenience function which is inefficient on busy servers since
 * the file is opened on every access. Use 
 * gnutls_certificate_set_ocsp_status_request_function() to fine-tune
 * file accesses.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.1.3
 **/
int
gnutls_certificate_set_ocsp_status_request_file
(gnutls_certificate_credentials_t sc, const char *response_file,
 unsigned int flags)
{
	sc->ocsp_func = file_ocsp_func;
	sc->ocsp_func_ptr = sc;
	sc->ocsp_response_file = gnutls_strdup(response_file);
	if (sc->ocsp_response_file == NULL)
		return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

	return 0;
}

static void _gnutls_status_request_deinit_data(extension_priv_data_t epriv)
{
	status_request_ext_st *priv = epriv.ptr;

	if (priv == NULL)
		return;

	deinit_responder_id(priv);
	gnutls_free(priv->request_extensions.data);
	gnutls_free(priv->response.data);
	gnutls_free(priv);
}

static int
_gnutls_status_request_pack(extension_priv_data_t epriv,
			    gnutls_buffer_st * ps)
{
	status_request_ext_st *priv = epriv.ptr;
	int ret;

	BUFFER_APPEND_PFX4(ps, priv->response.data, priv->response.size);

	return 0;

}

static int
_gnutls_status_request_unpack(gnutls_buffer_st * ps,
			      extension_priv_data_t * epriv)
{
	status_request_ext_st *priv;
	int ret;

	priv = gnutls_calloc(1, sizeof(*priv));
	if (priv == NULL) {
		gnutls_assert();
		return GNUTLS_E_MEMORY_ERROR;
	}

	BUFFER_POP_DATUM(ps, &priv->response);

	epriv->ptr = priv;

	return 0;

      error:
	gnutls_free(priv);
	return ret;
}

extension_entry_st ext_mod_status_request = {
	.name = "STATUS REQUEST",
	.type = GNUTLS_EXTENSION_STATUS_REQUEST,
	.parse_type = GNUTLS_EXT_TLS,
	.recv_func = _gnutls_status_request_recv_params,
	.send_func = _gnutls_status_request_send_params,
	.pack_func = _gnutls_status_request_pack,
	.unpack_func = _gnutls_status_request_unpack,
	.deinit_func = _gnutls_status_request_deinit_data
};

/* Functions to be called from handshake */

int
_gnutls_send_server_certificate_status(gnutls_session_t session, int again)
{
	mbuffer_st *bufel = NULL;
	uint8_t *data;
	int data_size = 0;
	int ret;
	status_request_ext_st *priv = NULL;
	extension_priv_data_t epriv;
	if (again == 0) {
		ret =
		    _gnutls_ext_get_session_data(session,
						 GNUTLS_EXTENSION_STATUS_REQUEST,
						 &epriv);
		if (ret < 0)
			return 0;
		priv = epriv.ptr;

		if (!priv->response.size)
			return 0;

		data_size = priv->response.size + 4;
		bufel =
		    _gnutls_handshake_alloc(session, data_size);
		if (!bufel)
			return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

		data = _mbuffer_get_udata_ptr(bufel);

		data[0] = 0x01;
		_gnutls_write_uint24(priv->response.size, &data[1]);
		memcpy(&data[4], priv->response.data, priv->response.size);

		_gnutls_free_datum(&priv->response);
	}
	return _gnutls_send_handshake(session, data_size ? bufel : NULL,
				      GNUTLS_HANDSHAKE_CERTIFICATE_STATUS);
}

int _gnutls_recv_server_certificate_status(gnutls_session_t session)
{
	uint8_t *data;
	int data_size;
	size_t r_size;
	gnutls_buffer_st buf;
	int ret;
	status_request_ext_st *priv = NULL;
	extension_priv_data_t epriv;

	ret =
	    _gnutls_ext_get_session_data(session,
					 GNUTLS_EXTENSION_STATUS_REQUEST,
					 &epriv);
	if (ret < 0)
		return 0;

	priv = epriv.ptr;

	if (!priv->expect_cstatus)
		return 0;

	ret = _gnutls_recv_handshake(session,
				     GNUTLS_HANDSHAKE_CERTIFICATE_STATUS,
				     0, &buf);
	if (ret < 0)
		return gnutls_assert_val_fatal(ret);

	priv->expect_cstatus = 0;

	data = buf.data;
	data_size = buf.length;

	/* minimum message is type (1) + response (3) + data */
	if (data_size == 0)
		return 0;
	else if (data_size < 4)
		return
		    gnutls_assert_val(GNUTLS_E_UNEXPECTED_PACKET_LENGTH);

	if (data[0] != 0x01) {
		gnutls_assert();
		_gnutls_handshake_log("EXT[%p]: unknown status_type %d\n",
				      session, data[0]);
		return 0;
	}
	DECR_LENGTH_COM(data_size, 1, ret =
			GNUTLS_E_UNEXPECTED_PACKET_LENGTH;
			goto error);
	data++;

	DECR_LENGTH_COM(data_size, 3, ret =
			GNUTLS_E_UNEXPECTED_PACKET_LENGTH;
			goto error);
	r_size = _gnutls_read_uint24(data);
	data += 3;

	DECR_LENGTH_COM(data_size, r_size, ret =
			GNUTLS_E_UNEXPECTED_PACKET_LENGTH;
			goto error);

	ret = _gnutls_set_datum(&priv->response, data, r_size);
	if (ret < 0)
		goto error;

	ret = 0;

      error:
	_gnutls_buffer_clear(&buf);

	return ret;
}

#endif
