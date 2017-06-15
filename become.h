#ifndef _BECOME_H
#define _BECOME_H

#include "user.h"
#include "sys_unistd.h"

class become
{
 private:
    unistd unistd_;
 public:
 become(user user, unistd &unistd) :
    unistd_ (unistd)
    {
        uid_t uid = user.uid();
        unistd_.seteuid(uid);
    }

    ~become()
    {
        unistd_.seteuid(0);
    }
};

#endif
