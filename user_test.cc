#include <memory>

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

std::string fake_home_directory("");
int fake_getpwnam_r(const char *nam, struct passwd *pwd, char *buffer, size_t bufsize, struct passwd **result) {
  pwd->pw_dir = const_cast<char *>(fake_home_directory.c_str());
  result = &pwd;
  return 0;
}

int initialize_concrete_user() {
    // given
    std::string username("msmith");
    std::string home_directory("this is my home");
    fake_home_directory = home_directory;

    // when
    std::shared_ptr<user> user(create_user(username));

    // then
    check(user, "user should be returned");
    check(user->home_directory() == home_directory, "home directory does not match");

    succeed();

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



