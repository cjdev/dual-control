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

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "sys_stdlib.h"

using generator = std::function<std::string()>;

inline    std::string token_from_int(int x) {
        int v = std::abs(x % 1000000);
        std::ostringstream is;
        is << std::setfill('0') << std::setw(6)<< v;
        return is.str();
    }

inline generator make_generator(const stdlib &stdlib)
{
    return [stdlib] {
        return token_from_int(stdlib.rand());
    };
}

#endif

