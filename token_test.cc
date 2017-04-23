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


#include "token.h"
#include "test_util.h"
#include "user.h"

class fake_file_reader : public file_reader_ifc {
    public:
        std::string read(std::string file_path) {
            return file_path;
        }
};

class fake_user : public user_ifc {
    private:
        std::string home_directory_;
    public:
        fake_user(std::string &user_name) :
            home_directory_("home/" + user_name) {
            }
        std::string home_directory() {
            return home_directory_;
        }
};

int reads_from_the_right_file () {
    //given
    file_reader test_file_reader(file_reader::delegate(new fake_file_reader));
    std::string user_name = "user";
    std::string expected = "home/" + user_name + "/.dual_control";
    user test_user(user::delegate(new fake_user(user_name)));
    user_token_supplier supplier(user_token_supplier::create(test_file_reader));

    //when
    std::string actual = supplier.token(test_user);

    //then
    check(actual == expected, "read wrong file");
    succeed();
}



RESET_VARS_START
RESET_VARS_END

int run_tests() {
    test(reads_from_the_right_file);
    succeed();
}

int main(int argc, char *argv[]) {
    return !run_tests();
}

