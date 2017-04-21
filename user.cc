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

#include "user.h"

namespace {
    class directory_impl : public directory_ifc {
        public:
          std::vector<user> find_user (const std::string &user_name);
    };
}


/*
class concrete_user : public user
{
private:
    std::vector<char> buffer_;
    std::shared_ptr<struct passwd> store_;
public:
    concrete_user (const std::vector<char> &buffer,
                   const std::shared_ptr<struct passwd> &store);
};

concrete_user::concrete_user (const std::vector<char> &buffer,
                              const std::shared_ptr<struct passwd> &store) :
    buffer_ (buffer),
    store_ (store),
    user (store.get())
{
}

const std::shared_ptr<user> create_user (const std::string &user_name)
{
    std::vector<char> buffer (sysconf (_SC_GETPW_R_SIZE_MAX));
    std::shared_ptr<struct passwd> sys_passwd (new struct passwd);
    struct passwd *found_passwd (0);

    getpwnam_r (user_name.c_str(), sys_passwd.get(), buffer.data(),
                buffer.size(), &found_passwd);

    std::shared_ptr<user> rval;

    if (found_passwd) {
        rval.reset (new concrete_user (buffer, sys_passwd));
    }

    return rval;
}
*/

