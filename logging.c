#include "logging.h"
#include <syslog.h>

/*
void openlog(const char *ident, int option, int facility);
void syslog(int priority, const char *format, ...);
void closelog(void);
void vsyslog(int priority, const char *format, va_list ap);
*/

static const char program_name[] = "pam_dual_control";

void log_dual_control() {
    openlog(program_name, 0, LOG_AUTH);
    syslog(LOG_INFO, "Logged: ");
    closelog();
}

