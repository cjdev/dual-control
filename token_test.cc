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
#include <cstring>
#include <pwd.h>
#include <cstdio>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

#include "token.h"
#include "test_util.h"
#include "user.h"
#include "sys_fstream.h"

class fake_user : public user_ifc
{
private:
    std::string home_directory_;
public:
    fake_user() {}
    fake_user (const std::string &home_directory) :
        home_directory_ (home_directory)
    {
    }
    std::string home_directory()
    {
        return home_directory_;
    }
};

class fake_fstreams : public fstreams_ifc {
    private:
        std::string expected_file_path_;
        std::string file_contents_;
    public:
        fake_fstreams(const std::string &expected_file_path, const std::string &file_contents)
            : expected_file_path_(expected_file_path),
              file_contents_(file_contents) {}
        pstream open_fstream(const std::string &file_path) {
            if (file_path == expected_file_path_) {
                return fstreams::pstream(new std::istringstream(file_contents_));
            } else {
                return fstreams_ifc::open_fstream(file_path);
            }

        }
};

int reads_from_the_right_file ()
{
    //given
    std::string home_directory = "/somedir";
    // hardcoded file name is .dual_control in the user's home directory
    std::string token_file = home_directory + "/.dual_control";
    std::string token("123456");
    fstreams test_streams(fstreams::delegate(new fake_fstreams(token_file, token)));

    //file_reader test_file_reader (file_reader::delegate (new fake_file_reader));
    user test_user (user::delegate (new fake_user (home_directory)));
    user_token_supplier supplier (user_token_supplier::create (
                                      test_streams));

    //when
    std::string actual = supplier.token (test_user);

    //then
    check (actual == token, "token does not match");
    succeed();
}

int returns_empty_string_if_file_open_fail()
{
    //given
    std::string home_directory = "/somedir";
    // hardcoded file name is .dual_control in the user's home directory
    std::string token_file = home_directory + "/.not_dual_control";
    fstreams test_streams(fstreams::delegate(new fake_fstreams(token_file, "654321")));
    user test_user (user::delegate (new fake_user (home_directory)));
    user_token_supplier supplier (user_token_supplier::create (
                                      test_streams));


    //when
    std::string actual = supplier.token (test_user);

    //then
    check (actual == "", "should have returned empty string");
    succeed();
}

RESET_VARS_START
RESET_VARS_END

int run_tests()
{
    test (reads_from_the_right_file);
    test (returns_empty_string_if_file_open_fail);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}

