#include <memory>
#include <iostream>
#include "user.h"
#include "test_util.h"

bool gets_home_directory()
{
    //given
    const char *expected_home_directory = "home/msmith";
    struct passwd test_passwd;
    test_passwd.pw_dir = const_cast<char *> (expected_home_directory);
    user test_user (&test_passwd);

    //when
    std::string actual_home_directory = test_user.home_directory();

    //then
    check (expected_home_directory == actual_home_directory,
           "directories do not match");
    return expected_home_directory == actual_home_directory;
}

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

