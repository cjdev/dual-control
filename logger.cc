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

#include "sys_syslog.h"
#include "logger.h"

namespace {
    class impl : public logger_ifc {
        private:
            sys_syslog syslog_;
        public:
            impl(const sys_syslog &sys_syslog) : syslog_(sys_syslog) {}
            void log (int result, const std::string &user_name,
              const std::string &token) {
                std::string message(user_name + " " + token + " success");

                syslog_.openlog("dual-control", 0, LOG_AUTHPRIV);
                syslog_.syslog(LOG_NOTICE, message.c_str());
               syslog_.closelog();
            }
    };
}

logger logger::create(const sys_syslog &sys_syslog) {
    return logger(delegate(new impl(sys_syslog)));
}
