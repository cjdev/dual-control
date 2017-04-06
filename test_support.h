/* Include this in a module that will be under test coverage
 *
 */

#ifndef _TEST_SUPPORT_H
#define _TEST_SUPPORT_H

void fake_openlog(const char *ident, int logopt, int facility);
void fake_syslog(int priority, const char *format, ...);
void fake_closelog(void);

/*
 * replace C library functions with fake counterparts when UINT_TEST symbol
 * is defined
 */
#ifdef UNIT_TEST
#define openlog(IDENT, LOGOPT, FACILITY) fake_openlog(IDENT, LOGOPT, FACILITY)
#define syslog(PRIORITY, ...) fake_syslog(PRIORITY, __VA_ARGS__)
#define closelog() fake_closelog()
#endif

#endif

