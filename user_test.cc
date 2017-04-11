#include "user.h"
#include "test_util.h"

int gets_home_directory() {
    //given
    const char *expected_home_directory = "home/msmith";
    struct passwd test_passwd;
    test_passwd.pw_dir = const_cast<char *>(expected_home_directory);
    user test_user(&test_passwd);

    //when
    std::string actual_home_directory = test_user.home_directory();

    //then
    check(expected_home_directory == actual_home_directory, "directories do not match");
    return expected_home_directory == actual_home_directory;
}

RESET_VARS_START
RESET_VARS_END

int run_tests() {
    test(gets_home_directory);
    succeed();
}
int main(int argc, char *argv[]) {
    return !run_tests();
}



