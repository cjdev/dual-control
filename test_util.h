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

#ifndef _TESTUTIL_H
#define _TESTUTIL_H

#include <cstring>
#include <cstdio>

#define RESET_COLORS "\x1b[0m"
#define FOREGROUND_GREEN "\x1b[32m"
#define FOREGROUND_RED "\x1b[31m"

#define check(assertion, msg) \
    if (!(assertion)) { \
        fprintf(stderr, "> assertion failed: %s\n", msg);    \
      return 0; \
    }

#define checkint(expected, actual, name) \
    check(expected == actual, name " should be " #expected)

#define checkstr(expected, actual, name) \
    check(!strcmp(expected, actual), name " should be '" expected "'")

#define RESET_VARS_START void __reset_vars() {

#define RESET_VARS_END }

#define test(NAME) \
    { \
        int result = NAME (); \
        if (!result) { \
            fprintf(stderr, "%s! <%s:%d> test failed: %s\n%s", FOREGROUND_RED, __FILE__, __LINE__, #NAME, RESET_COLORS); \
	    return 0; \
        } else { \
            fprintf (stderr, "%s> test passed: %s\n%s", FOREGROUND_GREEN, #NAME, RESET_COLORS); \
        } \
        fflush(stderr); \
    }

#define succeed() return 1
#define fail(MSG) {\
        fprintf(stderr, "%s! <%s:%d> test failed: %s\n%s", FOREGROUND_RED, __FILE__, __LINE__, MSG, RESET_COLORS); \
        return 0;                                                       \
    }

#endif

