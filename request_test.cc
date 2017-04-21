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

#include <vector>
#include <string>
#include <security/pam_modules.h>

#include "request.h"
#include "test_util.h"

bool construction_happy()
{
    // given
    pam_handle *handle = reinterpret_cast<pam_handle *> (975);
    int flags = 0x23;
    const char *arg = "an argument";

    // when
    pam_request actual (handle, flags, 1, &arg);

    // then
    check (handle == actual.handle(), "handle does not match");
    check (flags == actual.flags(), "flags does not match");
    std::vector<std::string> expected_args;
    expected_args.push_back (arg);
    check (expected_args == actual.arguments(), "arguments does not match");
    succeed();
}

bool multiple_arguments()
{
    // given
    const char *arg1 = "arg1";
    const char *arg2 = "arg2";
    const char *arg3 = "arg3";
    const char *argv[] = {arg1,arg2, arg3};

    // when
    pam_request actual (0, 0, 3, argv);

    // then
    std::vector<std::string> expected_args;
    expected_args.push_back (arg1);
    expected_args.push_back (arg2);
    expected_args.push_back (arg3);
    check (expected_args == actual.arguments(), "arguments does not match");
    succeed();
}

bool zero_arguments()
{

    // when
    pam_request actual (0, 0, 0, 0);

    // then
    check (std::vector<std::string> (0) == actual.arguments(),
           "arguments does not match");
    succeed();
}

RESET_VARS_START
RESET_VARS_END

int run_tests()
{
    test (construction_happy);
    test (multiple_arguments);
    test (zero_arguments);
    succeed();
}

int main()
{
    return !run_tests();
}

