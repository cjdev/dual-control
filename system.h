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

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <memory>

#include "sys_time.h"
#include "sys_stdlib.h"

class system
{
public:
    system (const stdlib &stdlib, const sys_time &time);
};

#endif

