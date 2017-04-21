/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the  GNU General Public License  version 2  or
 * later.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

/* PAM native handler entrypoints. Keep these dumb so the logic can
 * be tested.
 */
#include <security/pam_modules.h>
#include <string>
#include <memory>
#include <vector>

#include "request.h"
#include "dual_control.h"

extern dual_control dc;

PAM_EXTERN int pam_sm_authenticate (pam_handle_t *pamh, int flags, int argc,
                                    const char **argv)
{
    return dc.authenticate (pam_request (pamh, flags, argc, argv));
}

PAM_EXTERN int pam_sm_setcred (pam_handle_t *pamh, int flags, int argc,
                               const char **argv)
{
    return dc.setcred (pam_request ( pamh, flags, argc, argv));
}

