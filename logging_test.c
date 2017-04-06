#include <stdio.h>
#include <string.h>

#include "dc_syslog.h"
#include "logging.h"

#include "testutil.h"

#define reset_vars() \
    logged_priority = -1000; \
    close_log_invoked = 0; \
    opened_facility = -1000; \
    const char *opened_program_name = ""; \
    int opened_logopt = -1000; \

int logged_priority = -1000;
const char *logged_message = "";
void dc_syslog(int priority, const char *message, ...) {
    logged_priority = priority;
    logged_message = message;
}

int close_log_invoked = 0;
void dc_closelog(void) {
    close_log_invoked = 1;
}

int opened_facility = -1000;
const char *opened_program_name = "";
int opened_logopt = -1000;
void dc_openlog(const char *ident, int logopt, int facility) {
    opened_facility = facility;
    opened_program_name = ident;
    opened_logopt = logopt;
}

int test_log_success() {
    // given

    // when
    log_success();

    // then
    checkint(LOG_AUTHPRIV, opened_facility, "facility");
    checkint(LOG_NOTICE, logged_priority, "priority");
    checkint(0, opened_logopt, "logopt");
    check(close_log_invoked, "log closed");
    checkstr("pam_dual_control", opened_program_name, "program name");
    checkstr("dual control succeeded", logged_message, "logged message");
    succeed();
}

int test_runner() {
    test(test_log_success);
    succeed();
}

int main(int numargs, char **args) {
    return !test_runner();
}
