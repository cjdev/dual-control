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

#ifndef _BECOME_H
#define _BECOME_H

#include "user.h"
#include "sys_unistd.h"

class become
{
private:
    unistd unistd_;
public:
    become (user user, unistd &unistd) :
        unistd_ (unistd)
    {
        uid_t uid = user.uid();
        unistd_.seteuid (uid);
    }

    ~become()
    {
        unistd_.seteuid (0);
    }
};

#endif

