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

#include <string>
#include <vector>
#include <security/pam_modules.h>

#include "request.h"
#include "dual_control.h"
#include "conversation.h"
#include "validator.h"
#include "session.h"
#include "logger.h"

#include "user.h"
#include "sys_pwd.h"
#include "sys_unistd.h"
#include "unistd.h"

int dual_control_ifc::authenticate (const pam_request &request)
{
    return PAM_SERVICE_ERR;
}

int dual_control_ifc::setcred ( const pam_request &request)
{
    return PAM_SERVICE_ERR;
}

namespace
{
class impl : public dual_control_ifc
{
private:
    conversation conversation_;
    validator validator_;
    logger logger_;
    sessions sessions_;
public:
    impl (const dual_control_configuration &configuration);
    int authenticate (const pam_request &request);
    int setcred (const pam_request &request);
};

impl::impl (const dual_control_configuration &configuration) :
    conversation_ (configuration.conversation),
    validator_ (configuration.validator),
    logger_ (configuration.logger),
    sessions_ (configuration.sessions) {}

int impl::setcred (const pam_request &request)
{
    return PAM_SUCCESS;
}

int impl::authenticate (const pam_request &request)
{
    conversation_result input (conversation_.initiate (request));

    auto requester_user_name = sessions_.user_name (request);

    /// PROTOTYPE
    unistd unistd_ = unistd::create();
    pwd pwd_ = pwd::create();
    directory directory_ = directory::create (unistd_, pwd_);
    auto found_user = directory_.find_user ("eng2");
    uid_t uid = found_user[0].uid();
    seteuid(uid);
    ///

    int auth_result = validator_.validate (requester_user_name, input.user_name,
                                           input.token, input.reason) ? PAM_SUCCESS : PAM_AUTH_ERR;

    logger_.log (auth_result, requester_user_name, input.user_name,
                 input.token, input.reason);

    seteuid(0);
    return auth_result;
}
}
dual_control dual_control::create (const dual_control_configuration
                                   &configuration)
{
    return dual_control (std::shared_ptr<dual_control_ifc> (new impl (
                             configuration)));
}
