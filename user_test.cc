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

#include <iostream>

#include "user.h"
#include "test_util.h"
#include "sys_pwd.h"
#include "sys_unistd.h"

class fake_pwd : public pwd_ifc
{
private:
    std::string expected_user_name_;
    std::string home_directory_;
public:
    fake_pwd (const std::string expected_user_name) : expected_user_name_
        (expected_user_name), home_directory_("/somehome") {}
    int getpwnam_r (const char *user_name, passwd *out, char *buffer,
                    size_t buffer_sz, passwd **result)
    {
        if (expected_user_name_ == user_name)  {
            out->pw_dir = const_cast<char *>(home_directory_.c_str());
            out->pw_name = const_cast<char *>(expected_user_name_.c_str());
            *result = out;
        } else {
            *result = 0;
        }

        return 0;
    }
};

class match_buffer_pwd : public pwd_ifc
{
private:
    long int expected_buffer_sz_;
public:
    match_buffer_pwd (long int buffer_sz) : expected_buffer_sz_ (buffer_sz) {}
    int getpwnam_r (const char *user_name, passwd *out, char *buffer,
                    size_t buffer_sz, passwd **result)
    {

        if (expected_buffer_sz_ == buffer_sz && buffer != 0) {
            *result = out;
        } else {
            *result = 0;
        }

        return 0;
    }
};

class stub_pwnam_err_pwd : public pwd_ifc
{
public:
    int getpwnam_r (const char *user_name, passwd *out, char *buffer,
                    size_t buffer_sz, passwd **result)
    {
        *result = out;
        return 3;
    }

};

class fake_unistd : public unistd_ifc
{
private:
    int expected_name_;
    long int return_value_;
public:
    fake_unistd (int expected_name, long int return_value = 0)
        : expected_name_ (expected_name),
          return_value_ (return_value) {}
    long int sysconf (int name)
    {
        if (name == expected_name_) {
            return return_value_;
        }

        return -1;
    }
};

int find_user_happy()
{
    //given
    std::string user_name ("user");
    std::cout << "here " << __LINE__ << std::endl;
    pwd test_pwd (pwd::delegate (new fake_pwd (user_name)));
    std::cout << "here " << __LINE__ << std::endl;
    unistd test_unistd (unistd::delegate (new fake_unistd (
            _SC_GETPW_R_SIZE_MAX)));
    std::cout << "here " << __LINE__ << std::endl;
    directory directory (directory::create (test_unistd, test_pwd));

    //when
    std::cout << "here " << __LINE__ << std::endl;
    std::vector<user> results = directory.find_user (user_name);

    //then
    std::cout << "here " << __LINE__ << std::endl;
    check (!results.empty(), "user should have been found");
    std::cout << "here " << __LINE__ << std::endl;
    succeed();
}

int user_not_found()
{
    //given
    pwd test_pwd (pwd::delegate (new fake_pwd ("user")));
    unistd test_unistd (unistd::delegate (new fake_unistd (
            _SC_GETPW_R_SIZE_MAX)));
    directory directory (directory::create (test_unistd, test_pwd));

    //when
    std::vector<user> results = directory.find_user ("not_user");

    //then
    check (results.empty(), "user should not have been found");
    succeed();

}

int find_user_passes_buffer_and_size()
{
    //given
    long int buffer_sz = 5976;
    unistd test_unistd (unistd::delegate (new fake_unistd (_SC_GETPW_R_SIZE_MAX,
                                          buffer_sz)));
    pwd match_pwd (pwd::delegate (new match_buffer_pwd (buffer_sz)));
    directory directory (directory::create (test_unistd, match_pwd));

    //when
    std::vector<user> results = directory.find_user ("does_not_matter");

    // then
    check (!results.empty(), "match failed");
    succeed();
}

int find_user_fails_on_pwnam_r_error_and_result_ok()
{
    //given
    unistd test_unistd (unistd::delegate (new fake_unistd (
            _SC_GETPW_R_SIZE_MAX)));
    pwd stub_pwd (pwd::delegate (new stub_pwnam_err_pwd));
    directory directory (directory::create (test_unistd, stub_pwd));

    //when
    std::vector<user> results = directory.find_user ("does_not_matter");

    // then
    check (results.empty(), "did not check return");
    succeed();
}

RESET_VARS_START
RESET_VARS_END

int run_tests()
{
    test (find_user_happy);
//    test (user_not_found);
//    test (find_user_passes_buffer_and_size);
//    test (find_user_fails_on_pwnam_r_error_and_result_ok);
    succeed();
}

int main (int argc, char **argv)
{
    return !run_tests();
}

