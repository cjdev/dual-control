#ifndef _DC_SYSLOG_H
#define _DC_SYSLOG_H

#include <syslog.h>

void dc_openlog(const char *ident, int logopt, int facility);
void dc_syslog(int priority, const char *message, ...);
void dc_closelog(void);

#endif
