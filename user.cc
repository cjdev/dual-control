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
class user_impl : public user_ifc
{
private:
    std::string home_directory_;
    std::string user_name_;
    uid_t uid_;
public:
    user_impl (const passwd user_info) :
        home_directory_ (std::string (user_info.pw_dir)),
        user_name_ (std::string (user_info.pw_name)),
        uid_ (user_info.pw_uid) {}
    uid_t uid() const override
    {
        return uid_;
    }
    std::string home_directory() const override
    {
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
    std::vector<user> find_user (const std::string &user_name) const override
    {
        std::vector<char> buffer (unistd_.sysconf (_SC_GETPW_R_SIZE_MAX));
        passwd sys_passwd;
        passwd *found_passwd (0);
        int result = pwd_.getpwnam_r (user_name.c_str(), &sys_passwd,
                                      buffer.data(), buffer.size(), &found_passwd);
        std::vector<user> return_value;

        if (!result && found_passwd) {
            return_value.push_back (user::delegate (new user_impl (sys_passwd)));
        }

        return return_value;
    }

    std::vector<user> get_current_user () const override
    {
        const char *c_user_name = unistd_.getlogin();

        if (c_user_name == nullptr) {
            return {};
        }

        std::string user_name = c_user_name;

        auto found_user = find_user (user_name);

        if (found_user.empty()) {
            return {};
        }

        return {found_user[0]};
    }
};
}

directory directory::create (unistd &unistd, pwd &pwd)
{
    return directory (delegate (new directory_impl (unistd, pwd)));
}

