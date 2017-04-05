#include <stdarg.h>
#include "dc_syslog.h"

void dc_openlog(const char *ident, int logopt, int facility) {
    openlog(ident, logopt, facility);
}

void dc_syslog(int priority, const char *format, ...) {
    va_list varargs;
    va_start(varargs, format);
    vsyslog(priority, format, varargs);
    va_end(varargs);
}
void dc_closelog(void) {
    closelog();
}

