/* Include this in a module that will be under test coverage
 *
 */

#ifndef _TEST_SUPPORT_H
#define _TEST_SUPPORT_H
#include <stdlib.h>

// SYSLOG
void fake_openlog(const char *ident, int logopt, int facility);
void fake_syslog(int priority, const char *format, ...);
void fake_closelog(void);

// PWD
struct passwd;
int fake_getpwnam_r(const char *nam, struct passwd *pwd, char *buffer, size_t bufsize, struct passwd **result);


#ifdef UNIT_TEST
// SYSLOG
#define openlog(IDENT, LOGOPT, FACILITY) fake_openlog(IDENT, LOGOPT, FACILITY)
#define syslog(PRIORITY, ...) fake_syslog(PRIORITY, __VA_ARGS__)
#define closelog() fake_closelog()

// PWD
#define getpwnam_r(USER, PASSWD, BUFFER, BUFSIZE, PRESULT) fake_getpwnam_r(USER, PASSWD, BUFFER, BUFSIZE, PRESULT)

#endif

#endif

