#include <stdio.h>
#include <string.h>

#include "dc_syslog.h"
#include "logging.h"


void dc_syslog(int priority, const char *message, ...) {
}

void dc_closelog(void) {
}

int opened_facility = -1000;
const char *opened_program_name = "";
void dc_openlog(const char *ident, int logopt, int facility) {
    opened_facility = facility;
    opened_program_name = ident;
}


int test_log_success() {
    // given
    opened_facility = -1000;
    opened_program_name = "";

    // when
    log_success();

    // then
    return opened_facility == LOG_AUTHPRIV && !strcmp(opened_program_name, "pam_dual_control");
}


int main(int numargs, char **args) {
    if(test_log_success()) {
        fprintf(stderr, "Success!\n");
        return 0;
    } else {
        return 1;
    }
}

