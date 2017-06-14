#ifdef _BECOME_H
#define _BECOME_H

#include "user.h"
#include "sys_pwd.h"
#include "sys_unistd.h"
#include "unistd.h"

class become
{
 private:
    directory directory_;
 public:
    become(std::string &username, directory directory) :
        directory_(directory)
    {
        auto found_user = directory_.find_user (username);
        uid_t uid = found_user[0].uid();
        seteuid(uid);
    }

    ~become()
    {
        seteuid(0);
    }
};

#endif
