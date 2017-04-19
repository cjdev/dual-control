#include <vector>
#include <string>
#include <security/pam_modules.h>

#include "request.h"
#include "test_util.h"

bool construction_happy() {
    // given
    pam_handle *handle = reinterpret_cast<pam_handle *>(975);
    int flags = 0x23;
    const char *arg = "an argument";

    // when
    pam_request actual(handle, flags, 1, &arg);

    // then
    check(handle == actual.handle(), "handle does not match");
    check(flags == actual.flags(), "flags does not match");
    std::vector<std::string> expected_args;
    expected_args.push_back(arg);
    check(expected_args == actual.arguments(), "arguments does not match");
    succeed();
}


//int convert_single_argument_to_cplusplus()
//{
//    //given
//    int nargs = 1;
//    const char *arg = "blah";
//    const char **argv = &arg;
//
//    //when
//    std::vector<const std::string> actual = convert_arguments (nargs, argv);
//
//    //then
//    std::vector<std::string> temp (1);
//    temp[0] = arg;
//    std::vector<const std::string> expected (temp.begin(), temp.end());
//
//    check (actual == expected, "did not convert to c++");
//    succeed();
//}
//
//int convert_no_arguments_to_cplusplus()
//{
//    //given
//    int nargs = 0;
//    const char **argv = 0;
//
//    //when
//    std::vector<const std::string> actual = convert_arguments (nargs, argv);
//
//    //then
//    std::vector<const std::string> expected;
//
//    check (actual == expected, "did not convert to c++");
//    succeed();
//}
//
//int convert_multiple_arguments_to_cplusplus()
//{
//    //given
//    int nargs = 3;
//    const char *arg1 = "one";
//    const char *arg2 = "two";
//    const char *arg3 = "three";
//    const char *argv[] = {arg1,arg2,arg3};
//
//    //when
//    std::vector<const std::string> actual = convert_arguments (nargs, argv);
//
//    //then
//    std::vector<std::string> temp;
//    temp.push_back (arg1);
//    temp.push_back (arg2);
//    temp.push_back (arg3);
//    std::vector<const std::string> expected (temp.begin(), temp.end());
//
//    check (actual == expected, "did not convert to c++");
//    succeed();
//}

RESET_VARS_START
RESET_VARS_END

int run_tests()
{
    test (construction_happy);
    succeed();
}

int main()
{
    return !run_tests();
}

