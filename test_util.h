#ifndef _TESTUTIL_H
#define _TESTUTIL_H

#include <cstring>
#include <cstdio>
#define check(assertion, msg) \
    if (!(assertion)) { \
      fprintf(stderr, "assertion failed: %s\n", msg); \
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
      __reset_vars(); \
      int result = NAME (); \
      if (!result) { \
          fprintf(stderr, "test failed: %s\n", #NAME); \
          return 0; \
      } \
    }

#define succeed() return 1
#define fail() return 0

#endif

