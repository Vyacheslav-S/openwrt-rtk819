/*
 * Copyright (C) 2001-2013 Free Software Foundation, Inc.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <gnutls_int.h>
#include <gnutls_errors.h>
#include <libtasn1.h>
#include <gnutls_dh.h>
#include <random.h>
#include <gnutls/pkcs11.h>

#include <gnutls_extensions.h>	/* for _gnutls_ext_init */
#include <locks.h>
#include <system.h>
#include <accelerated/cryptodev.h>
#include <accelerated/accelerated.h>
#include <fips.h>

#include "gettext.h"

/* Minimum library versions we accept. */
#define GNUTLS_MIN_LIBTASN1_VERSION "0.3.4"

#ifdef __sun
# pragma fini(lib_deinit)
# pragma init(lib_init)
# define _CONSTRUCTOR
# define _DESTRUCTOR
#else
# define _CONSTRUCTOR __attribute__((constructor))
# define _DESTRUCTOR __attribute__((destructor))
#endif

/* created by asn1c */
extern const ASN1_ARRAY_TYPE gnutls_asn1_tab[];
extern const ASN1_ARRAY_TYPE pkix_asn1_tab[];
void *_gnutls_file_mutex;
void *_gnutls_pkcs11_mutex;

ASN1_TYPE _gnutls_pkix1_asn = ASN1_TYPE_EMPTY;
ASN1_TYPE _gnutls_gnutls_asn = ASN1_TYPE_EMPTY;

gnutls_log_func _gnutls_log_func = NULL;
gnutls_audit_log_func _gnutls_audit_log_func = NULL;
int _gnutls_log_level = 0;	/* default log level */

static void default_log_func(int level, const char* str)
{
	fprintf(stderr, "gnutls[%d]: %s", level, str);
}

/**
 * gnutls_global_set_log_function:
 * @log_func: it's a log function
 *
 * This is the function where you set the logging function gnutls is
 * going to use.  This function only accepts a character array.
 * Normally you may not use this function since it is only used for
 * debugging purposes.
 *
 * @gnutls_log_func is of the form,
 * void (*gnutls_log_func)( int level, const char*);
 **/
void gnutls_global_set_log_function(gnutls_log_func log_func)
{
	_gnutls_log_func = log_func;
}

/**
 * gnutls_global_set_audit_log_function:
 * @log_func: it is the audit log function
 *
 * This is the function to set the audit logging function. This
 * is a function to report important issues, such as possible
 * attacks in the protocol. This is different from gnutls_global_set_log_function()
 * because it will report also session-specific events. The session
 * parameter will be null if there is no corresponding TLS session.
 *
 * @gnutls_audit_log_func is of the form,
 * void (*gnutls_audit_log_func)( gnutls_session_t, const char*);
 *
 * Since: 3.0
 **/
void gnutls_global_set_audit_log_function(gnutls_audit_log_func log_func)
{
	_gnutls_audit_log_func = log_func;
}

/**
 * gnutls_global_set_time_function:
 * @time_func: it's the system time function, a gnutls_time_func() callback.
 *
 * This is the function where you can override the default system time
 * function.  The application provided function should behave the same
 * as the standard function.
 *
 * Since: 2.12.0
 **/
void gnutls_global_set_time_function(gnutls_time_func time_func)
{
	gnutls_time = time_func;
}

/**
 * gnutls_global_set_log_level:
 * @level: it's an integer from 0 to 99.
 *
 * This is the function that allows you to set the log level.  The
 * level is an integer between 0 and 9.  Higher values mean more
 * verbosity. The default value is 0.  Larger values should only be
 * used with care, since they may reveal sensitive information.
 *
 * Use a log level over 10 to enable all debugging options.
 **/
void gnutls_global_set_log_level(int level)
{
	_gnutls_log_level = level;
}

/**
 * gnutls_global_set_mem_functions:
 * @alloc_func: it's the default memory allocation function. Like malloc().
 * @secure_alloc_func: This is the memory allocation function that will be used for sensitive data.
 * @is_secure_func: a function that returns 0 if the memory given is not secure. May be NULL.
 * @realloc_func: A realloc function
 * @free_func: The function that frees allocated data. Must accept a NULL pointer.
 *
 * Deprecated: since 3.3.0 it is no longer possible to replace the internally used 
 *  memory allocation functions
 *
 * This is the function where you set the memory allocation functions
 * gnutls is going to use. By default the libc's allocation functions
 * (malloc(), free()), are used by gnutls, to allocate both sensitive
 * and not sensitive data.  This function is provided to set the
 * memory allocation functions to something other than the defaults
 *
 * This function must be called before gnutls_global_init() is called.
 * This function is not thread safe.
 **/
void
gnutls_global_set_mem_functions(gnutls_alloc_function alloc_func,
				gnutls_alloc_function secure_alloc_func,
				gnutls_is_secure_function is_secure_func,
				gnutls_realloc_function realloc_func,
				gnutls_free_function free_func)
{
	_gnutls_debug_log("called the deprecated gnutls_global_set_mem_functions()\n");
}

GNUTLS_STATIC_MUTEX(global_init_mutex);
static int _gnutls_init = 0;

/* cache the return code */
static int _gnutls_init_ret = 0;

/**
 * gnutls_global_init:
 *
 * This function performs any required precalculations, detects
 * the supported CPU capabilities and initializes the underlying
 * cryptographic backend. In order to free any resources 
 * taken by this call you should gnutls_global_deinit() 
 * when gnutls usage is no longer needed.
 *
 * This function increments a global counter, so that
 * gnutls_global_deinit() only releases resources when it has been
 * called as many times as gnutls_global_init().  This is useful when
 * GnuTLS is used by more than one library in an application.  This
 * function can be called many times, but will only do something the
 * first time.
 *
 * Since GnuTLS 3.3.0 this function is only required in systems that
 * do not support library constructors and static linking. This
 * function also became thread safe.
 *
 * A subsequent call of this function if the initial has failed will
 * return the same error code.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 **/
int gnutls_global_init(void)
{
	int ret = 0, res;
	int level;
	const char* e;
	
	GNUTLS_STATIC_MUTEX_LOCK(global_init_mutex);

	_gnutls_init++;
	if (_gnutls_init > 1) {
		if (_gnutls_init == 2 && _gnutls_init_ret == 0) {
			/* some applications may close the urandom fd 
			 * before calling gnutls_global_init(). in that
			 * case reopen it */
			ret = _gnutls_rnd_check();
			if (ret < 0) {
				gnutls_assert();
				goto out;
			}
		}
		ret = _gnutls_init_ret;
		goto out;
	}

	_gnutls_switch_lib_state(LIB_STATE_INIT);

	e = getenv("GNUTLS_DEBUG_LEVEL");
	if (e != NULL) {
		level = atoi(e);
		gnutls_global_set_log_level(level);
		if (_gnutls_log_func == NULL)
			gnutls_global_set_log_function(default_log_func);
		_gnutls_debug_log("Enabled GnuTLS logging...\n");
	}

	bindtextdomain(PACKAGE, LOCALEDIR);

	res = gnutls_crypto_init();
	if (res != 0) {
		gnutls_assert();
		ret = GNUTLS_E_CRYPTO_INIT_FAILED;
		goto out;
	}

	/* initialize ASN.1 parser
	 */
	if (asn1_check_version(GNUTLS_MIN_LIBTASN1_VERSION) == NULL) {
		gnutls_assert();
		_gnutls_debug_log
		    ("Checking for libtasn1 failed: %s < %s\n",
		     asn1_check_version(NULL),
		     GNUTLS_MIN_LIBTASN1_VERSION);
		ret = GNUTLS_E_INCOMPATIBLE_LIBTASN1_LIBRARY;
		goto out;
	}

	_gnutls_pkix1_asn = ASN1_TYPE_EMPTY;
	res = asn1_array2tree(pkix_asn1_tab, &_gnutls_pkix1_asn, NULL);
	if (res != ASN1_SUCCESS) {
		gnutls_assert();
		ret = _gnutls_asn2err(res);
		goto out;
	}

	res = asn1_array2tree(gnutls_asn1_tab, &_gnutls_gnutls_asn, NULL);
	if (res != ASN1_SUCCESS) {
		gnutls_assert();
		ret = _gnutls_asn2err(res);
		goto out;
	}

	/* Initialize the random generator */
	ret = _gnutls_rnd_init();
	if (ret < 0) {
		gnutls_assert();
		goto out;
	}

	/* Initialize the default TLS extensions */
	ret = _gnutls_ext_init();
	if (ret < 0) {
		gnutls_assert();
		goto out;
	}

	ret = gnutls_mutex_init(&_gnutls_file_mutex);
	if (ret < 0) {
		gnutls_assert();
		goto out;
	}

	ret = gnutls_mutex_init(&_gnutls_pkcs11_mutex);
	if (ret < 0) {
		gnutls_assert();
		goto out;
	}

	ret = gnutls_system_global_init();
	if (ret < 0) {
		gnutls_assert();
		goto out;
	}

#ifdef ENABLE_FIPS140
	res = _gnutls_fips_mode_enabled();
	/* res == 1 -> fips140-2 mode enabled
	 * res == 2 -> only self checks performed - but no failure
	 * res == not in fips140 mode
	 */
	if (res != 0) {
		_gnutls_debug_log("FIPS140-2 mode: %d\n", res);
		_gnutls_priority_update_fips();

		/* first round of self checks, these are done on the
		 * nettle algorithms which are used internally */
		ret = _gnutls_fips_perform_self_checks1();
		if (res != 2) {
			if (ret < 0) {
				gnutls_assert();
				goto out;
			}
		}
	}
#endif

	_gnutls_register_accel_crypto();
	_gnutls_cryptodev_init();

#ifdef ENABLE_FIPS140
	/* These self tests are performed on the overriden algorithms
	 * (e.g., AESNI overriden AES). They are after _gnutls_register_accel_crypto()
	 * intentionally */
	if (res != 0) {
		ret = _gnutls_fips_perform_self_checks2();
		if (res != 2) {
			if (ret < 0) {
				gnutls_assert();
				goto out;
			}
		}
		_gnutls_fips_mode_reset_zombie();
	}
#endif
	_gnutls_switch_lib_state(LIB_STATE_OPERATIONAL);
	ret = 0;

      out:
	_gnutls_init_ret = ret;
	GNUTLS_STATIC_MUTEX_UNLOCK(global_init_mutex);
	return ret;
}

static void _gnutls_global_deinit(unsigned destructor)
{
	GNUTLS_STATIC_MUTEX_LOCK(global_init_mutex);

	if (_gnutls_init == 1) {
		_gnutls_init = 0;
		if (_gnutls_init_ret < 0) {
			/* only deinitialize if gnutls_global_init() has
			 * succeeded */
			gnutls_assert();
			goto fail;
		}

		gnutls_crypto_deinit();
		_gnutls_rnd_deinit();
		_gnutls_ext_deinit();
		asn1_delete_structure(&_gnutls_gnutls_asn);
		asn1_delete_structure(&_gnutls_pkix1_asn);

		_gnutls_crypto_deregister();
		gnutls_system_global_deinit();
		_gnutls_cryptodev_deinit();

#ifdef ENABLE_PKCS11
		/* Do not try to deinitialize the PKCS #11 libraries
		 * from the destructor. If we do and the PKCS #11 modules
		 * are already being unloaded, we may crash.
		 */
		if (destructor == 0) {
			gnutls_pkcs11_deinit();
		}
#endif

		gnutls_mutex_deinit(&_gnutls_file_mutex);
		gnutls_mutex_deinit(&_gnutls_pkcs11_mutex);
	} else {
		if (_gnutls_init > 0)
			_gnutls_init--;
	}

 fail:
	GNUTLS_STATIC_MUTEX_UNLOCK(global_init_mutex);
}

/**
 * gnutls_global_deinit:
 *
 * This function deinitializes the global data, that were initialized
 * using gnutls_global_init().
 *
 **/
void gnutls_global_deinit(void)
{
	_gnutls_global_deinit(0);
}

/**
 * gnutls_check_version:
 * @req_version: version string to compare with, or %NULL.
 *
 * Check GnuTLS Library version.
 *
 * See %GNUTLS_VERSION for a suitable @req_version string.
 *
 * Returns: Check that the version of the library is at
 *   minimum the one given as a string in @req_version and return the
 *   actual version string of the library; return %NULL if the
 *   condition is not met.  If %NULL is passed to this function no
 *   check is done and only the version string is returned.
  **/
const char *gnutls_check_version(const char *req_version)
{
	if (!req_version || strverscmp(req_version, VERSION) <= 0)
		return VERSION;

	return NULL;
}

static void _CONSTRUCTOR lib_init(void)
{
int ret;

	ret = gnutls_global_init();
	if (ret < 0) {
		fprintf(stderr, "Error in GnuTLS initialization: %s\n", gnutls_strerror(ret));
		_gnutls_switch_lib_state(LIB_STATE_ERROR);
	}
}

static void _DESTRUCTOR lib_deinit(void)
{
	_gnutls_global_deinit(1);
}
