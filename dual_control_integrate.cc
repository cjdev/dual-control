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

#include "conversation.h"
#include "dual_control.h"
#include "generator.h"
#include "logger.h"
#include "request.h"
#include "session.h"
#include "sys_fstream.h"
#include "sys_pam.h"
#include "sys_pwd.h"
#include "sys_syslog.h"
#include "sys_unistd.h"
#include "token.h"
#include "user.h"
#include "validator.h"

namespace
{
dual_control initialize()
{
    dual_control_configuration configuration;
    pwd pwd (pwd::create());
    unistd unistd (unistd::create());
    directory directory (directory::create (unistd, pwd));
    fstreams fstreams (fstreams::create());
    sys_time time (sys_time::get());
    int code_digits = 6;
    totp_generator generator = totp_generator (time, code_digits);
    tokens tokens (tokens::create (fstreams, generator));
    validator validator (validator::create (directory, tokens));
    pam pam (pam::create());
    conversation conversation (conversation::create (pam));
    sys_syslog sys_syslog (sys_syslog::create());
    logger logger (logger::create (sys_syslog));
    sessions sessions (sessions::create (pam));
    configuration.validator = validator;
    configuration.logger = logger;
    configuration.conversation = conversation;
    configuration.sessions = sessions;
    return dual_control::create (configuration);
}
dual_control dc = initialize();
}

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

