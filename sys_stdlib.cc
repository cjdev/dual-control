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

#include <cstdlib>
#include <memory>

#include "sys_stdlib.h"

namespace
{
class impl : public stdlib_ifc
{
public:
    int rand() const override
    {
        return ::rand();
    }

};
}

stdlib const &stdlib::get()
{
    static stdlib singleton {std::make_shared<impl>()};
    return singleton;
}

