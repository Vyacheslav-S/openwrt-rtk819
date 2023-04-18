/*
 *   auth.c -- PAM authorization code, common between chsh and chfn
 *   (c) 2012 by Cody Maloney <cmaloney@theoreticalchaos.com>
 *
 *   this program is free software.  you can redistribute it and
 *   modify it under the terms of the gnu general public license.
 *   there is no warranty.
 *
 */

#include "auth.h"
#include "pamfail.h"

int auth_pam(const char *service_name, uid_t uid, const char *username)
{
	if (uid != 0) {
		pam_handle_t *pamh = NULL;
		struct pam_conv conv = { misc_conv, NULL };
		int retcode;

		retcode = pam_start(service_name, username, &conv, &pamh);
		if (pam_fail_check(pamh, retcode))
			return FALSE;

		retcode = pam_authenticate(pamh, 0);
		if (pam_fail_check(pamh, retcode))
			return FALSE;

		retcode = pam_acct_mgmt(pamh, 0);
		if (retcode == PAM_NEW_AUTHTOK_REQD)
			retcode =
			    pam_chauthtok(pamh, PAM_CHANGE_EXPIRED_AUTHTOK);
		if (pam_fail_check(pamh, retcode))
			return FALSE;

		retcode = pam_setcred(pamh, 0);
		if (pam_fail_check(pamh, retcode))
			return FALSE;

		pam_end(pamh, 0);
		/* no need to establish a session; this isn't a
		 * session-oriented activity...  */
	}
	return TRUE;
}
