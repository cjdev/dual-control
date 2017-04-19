#include <syslog.h>

#include "logging.h"
#include "test_support.h"


static const char program_name[] = "pam_dual_control";

void log_success()
{
    openlog (program_name, 0, LOG_AUTHPRIV);
    syslog (LOG_NOTICE, "dual control succeeded");
    closelog();
}

void log_failure()
{
    openlog (program_name, 0, LOG_AUTHPRIV);
    syslog (LOG_NOTICE, "dual control failed");
    closelog();
}


