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
#include <vector>

#include "user.h"
#include "sys_unistd.h"
#include "sys_pwd.h"

namespace
{

class user_impl : public user_ifc {
    private:
        std::string home_directory_;
        std::string user_name_;
    public:
        user_impl(const passwd user_info) :
            home_directory_(std::string(user_info.pw_dir)),
            user_name_(std::string(user_info.pw_name)) {}
        std::string home_directory() {
            return home_directory_;
        }
};

class directory_impl : public directory_ifc
{
private:
    unistd unistd_;
    pwd pwd_;
public:
    directory_impl (unistd &unistd, pwd &pwd) : unistd_ (unistd), pwd_ (pwd) {}
    std::vector<user> find_user (const std::string &user_name)
    {
        std::vector<char> buffer (unistd_.sysconf (_SC_GETPW_R_SIZE_MAX));
        passwd sys_passwd;
        passwd *found_passwd (0);
        int result = pwd_.getpwnam_r (user_name.c_str(), &sys_passwd,
                                      buffer.data(), buffer.size(), &found_passwd);
        std::vector<user> return_value;

        if (!result && found_passwd) {
            return_value.push_back (std::shared_ptr<user_ifc>(new user_impl(sys_passwd)));
        }

        return return_value;
    }
};
}

directory directory::create (unistd &unistd, pwd &pwd)
{
    return directory (delegate (new directory_impl (unistd, pwd)));
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

