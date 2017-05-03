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

using generator = std::function<std::string()>;

inline generator make_generator()
{
    return [] {return std::string ("234567");};
}

#endif

