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

#include <syslog.h>
#include <security/pam_modules.h>

#include "sys_syslog.h"
#include "logger.h"

namespace
{
class impl : public logger_ifc
{
private:
    sys_syslog syslog_;
public:
    impl (const sys_syslog &sys_syslog) : syslog_ (sys_syslog) {}
    void log (int result, const std::string &requester_user_name, const std::string &authorizer_user_name,
              const std::string &token)
    {
        std::string message;
        int facility;
        int priority;

        switch (result) {
        case PAM_SUCCESS:
            facility = LOG_AUTHPRIV;
            priority = LOG_NOTICE;
            message = requester_user_name + " " + authorizer_user_name + " " + token + " " + "success";
            break;

        case PAM_AUTH_ERR:
            facility = LOG_AUTHPRIV;
            priority = LOG_NOTICE;
            message = requester_user_name + " " + authorizer_user_name + " " + token + " " + "fail";
            break;

        default:
            facility = LOG_AUTH;
            priority = LOG_ERR;
            message = requester_user_name + " " + authorizer_user_name + " pam returned error";
            break;
        }

        syslog_.openlog ("dual-control", 0, facility);
        syslog_.syslog (priority, message.c_str());
        syslog_.closelog();
    }
};
}

logger logger::create (const sys_syslog &sys_syslog)
{
    return logger (delegate (new impl (sys_syslog)));
}

