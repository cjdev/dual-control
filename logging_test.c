#include <stdio.h>

#include "dc_syslog.h"
#include "logging.h"

void dc_syslog(int priority, const char *message, ...) {
}

void dc_closelog(void) {
}

int opened_facility = -1000;
void dc_openlog(const char * ident, int logopt, int facility) {
    opened_facility = facility;
}


int test_log_success_facility_authpriv() {
    // given
    opened_facility = -1000;

    // when
    log_success();

    // then
    return opened_facility == LOG_AUTHPRIV;
}


int main(int numargs, char **args) {
    if(test_log_success_facility_authpriv()) {
        fprintf(stderr, "Success!\n");
        return 0;
    } else {
        return 1;
    }
}

