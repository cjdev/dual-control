#ifndef _TESTUTIL_H
#define _TESTUTIL_H

#include <string.h>

#define check(assertion, msg) \
    if (!(assertion)) { \
      fprintf(stderr, "assertion failed: %s\n", msg); \
      return 0; \
    }

#define checkint(expected, actual, name) \
    check(expected == actual, name " should be " #expected)

#define checkstr(expected, actual, name) \
    check(!strcmp(expected, actual), name " should be '" expected "'")

#ifdef reset_vars
#define _reset reset_vars()
#else
#define _reset
#endif

#define test(NAME) \
    { \
      _reset \
      int result = NAME (); \
      if (!result) { \
          fprintf(stderr, "test failed: %s\n", #NAME); \
          return 0; \
      } \
    }

#define succeed() return 1
#define fail() return 0

#endif

