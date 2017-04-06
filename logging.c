#include "logging.h"
#include "dc_syslog.h"


static const char program_name[] = "pam_dual_control";

void log_success() {
    dc_openlog(program_name, 0, LOG_AUTHPRIV);
    dc_syslog(LOG_NOTICE, "dual control succeeded");
    dc_closelog();
}

void log_failure() {
    dc_openlog(program_name, 0, LOG_AUTHPRIV);
    dc_syslog(LOG_NOTICE, "dual control failed");
    dc_closelog();
}


