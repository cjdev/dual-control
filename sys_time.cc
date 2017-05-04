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

#include <memory>
#include <ctime>

#include "sys_time.h"

namespace
{
class impl : public sys_time_ifc
{
public:
    time_t time (time_t *timer) const override
    {
        return ::time (timer);
    }
};
}

const sys_time &sys_time::get()
{
    static sys_time singleton (std::make_shared<impl>());
    return singleton;
}

