#include <stdio.h>
#include <string.h>

#include "dc_syslog.h"
#include "logging.h"

int test_priority = -1000;
const char *logged_message = "";
void dc_syslog(int priority, const char *message, ...) {
    test_priority = priority;
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
      fprintf(stderr, "assertion failed %s\n", msg); \
      return 0; \
    }

#define succeed() return 1

int test_log_success() {
    // given
    opened_facility = -1000;
    opened_program_name = "";
    opened_logopt = -1000;
    test_priority = -1000;
    logged_message = "";

    // when
    log_success();

    // then
    check(opened_facility == LOG_AUTHPRIV, "facility should be authpriv");
    check(!strcmp(opened_program_name, "pam_dual_control"), "incorrect program name");
    check(opened_logopt == 0, "incorrect log option");
    check(test_priority == LOG_NOTICE, "incorrect priority");
    check(!strcmp(logged_message, "dual control succeeded"), "incorrect logged message");

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

