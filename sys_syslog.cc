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

namespace
{
class impl : public sys_syslog_ifc
{
public:
    void openlog (const char *ident, int logopt, int facility)
    {
        ::openlog (ident, logopt, facility);
    }

    void vsyslog (int priority, const char *message, va_list args)
    {
        ::vsyslog (priority, message, args);
    }

    void closelog()
    {
        ::closelog();
    }
};
}

sys_syslog sys_syslog::create()
{
    static sys_syslog singleton (sys_syslog::delegate (new impl));
    return singleton;
}

