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

#include "sys_pwd.h"

#include <pwd.h>

namespace
{
class impl : public pwd_ifc
{
public:
    int getpwnam_r (const char *user_name, passwd *out, char *buffer,
                    size_t buffer_sz, passwd **result) const override
    {
        return ::getpwnam_r (user_name, out, buffer, buffer_sz, result);
    }

};
}

pwd pwd::create()
{
    static pwd system_pwd (pwd::delegate (new impl));
    return system_pwd;
}

