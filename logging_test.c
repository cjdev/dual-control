#include <stdio.h>
#include <string.h>

#include "dc_syslog.h"
#include "logging.h"

int logged_priority = -1000;
const char *logged_message = "";
void dc_syslog(int priority, const char *message, ...) {
    logged_priority = priority;
    logged_message = message;
}

void dc_closelog(void) {
}

int opened_facility = -1000;
const char *opened_program_name = "";
int opened_logopt = -1000;
void dc_openlog(const char *ident, int logopt, int facility) {
    opened_facility = facility;
    opened_program_name = ident;
    opened_logopt = logopt;
}


#define check(assertion, msg) \
    if (!(assertion)) { \
      fprintf(stderr, "assertion failed: %s\n", msg); \
      return 0; \
    }

#define checkint(expected, actual, name) \
    check(expected == actual, name " should be " #expected)

#define checkstr(expected, actual, name) \
    check(!strcmp(actual, expected), name " should be '" expected "'")

#define succeed() return 1

int test_log_success() {
    // given
    opened_facility = -1000;
    opened_program_name = "";
    opened_logopt = -1000;
    logged_priority = -1000;
    logged_message = "";

    // when
    log_success();

    // then
    checkint(LOG_AUTHPRIV, opened_facility, "facility");
    checkint(LOG_NOTICE, logged_priority, "priority");
    checkint(0, opened_logopt, "logopt");
    checkstr("pam_dual_control", opened_program_name, "program name");
    checkstr("dual control succeeded", logged_message, "logged message");

    succeed();
}


int main(int numargs, char **args) {
    if(test_log_success()) {
        fprintf(stderr, "Success!\n");
        return 0;
    } else {
        return 1;
    }
}

