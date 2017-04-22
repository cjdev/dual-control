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

#include "sys_unistd.h"
#include <unistd.h>

namespace
{
class impl : public unistd_ifc
{
public:
    long int sysconf (int name)
    {
        return ::sysconf (name);
    }
};
static unistd sys_unistd (unistd::delegate (new impl));
}

unistd unistd::system()
{
    return sys_unistd;
}

