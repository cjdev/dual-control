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

#include <string>
#include <memory>

#include "session.h"
#include "test_util.h"
#include "sys_pam.h"
#include "request.h"

class fake_sys_pam : public pam_ifc
{
private:
    std::string user_name_;
public:
    fake_sys_pam (std::string user_name) : user_name_ (user_name) {}
    int get_user (pam_handle *handle, const char **out) const
    {
        *out = user_name_.c_str();
        return PAM_SUCCESS;
    }
};

int gets_user_from_pam()
{
    //given
    std::string user_name ("user");
    pam fake_sys_pam (std::make_shared<fake_sys_pam> (user_name));
    sessions sessions (sessions::create (fake_sys_pam));
    pam_request request (0, 0, 0, 0);

    //when
    std::string actual = sessions.user_name (request);

    //
    //then
    check (actual == user_name, "returned wrong user name");
    succeed();
}

int run_tests()
{
    test (gets_user_from_pam);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}

