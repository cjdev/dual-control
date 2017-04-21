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

#include "user.h"
#include "test_util.h"

int find_user_happy() {
    fail();
}

RESET_VARS_START
RESET_VARS_END

int run_tests() {
    test(find_user_happy);
    succeed();
}

int main(int argc, char **argv) {
    return !run_tests();
}

/*
std::shared_ptr<struct passwd> fake_passwd;
int fake_getpwnam_r (const char *nam, struct passwd *pwd, char *buffer,
                     size_t bufsize, struct passwd **result)
{
    if (fake_passwd) {
        *pwd = *fake_passwd;
        *result = pwd;
        return 0;
    }

    return -1;
}

bool create_user_succeeds()
{
    // given
    std::string username ("msmith");
    std::string home_directory ("this is my home");
    fake_passwd.reset (new struct passwd);
    fake_passwd->pw_dir = const_cast<char *> (home_directory.c_str());

    // when
    std::shared_ptr<user> user (create_user (username));

    // then
    check (user, "user should be returned");
    check (user->home_directory() == home_directory,
           "home directory does not match");

    succeed();

}

bool create_user_nonexistent()
{
    // given
    std::string username ("msmith");

    // when
    std::shared_ptr<user> user (create_user (username));

    // then
    check (!user, "no user should be returned");

    succeed();
}

RESET_VARS_START
fake_passwd.reset ((struct passwd *)0);
RESET_VARS_END

int run_tests()
{
    test (gets_home_directory);
    test (create_user_succeeds);
    test (create_user_nonexistent);
    succeed();
}
int main (int argc, char *argv[])
{
    return !run_tests();
}
*/

