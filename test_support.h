/* Include this in a module that will be under test coverage
 *
 */

#ifndef _TEST_SUPPORT_H
#define _TEST_SUPPORT_H
#include <cstdlib>
#include <cstdio>
#include <pwd.h>
#include <sys/stat.h>
// SYSLOG
void fake_openlog (const char *ident, int logopt, int facility);
void fake_syslog (int priority, const char *format, ...);
void fake_closelog (void);

// PWD
int fake_getpwnam_r (const char *nam, struct passwd *pwd, char *buffer,
                     size_t bufsize, struct passwd **result);

// SYS_STAT
int fake_stat (const char *path, struct stat *stat);

// STDIO
FILE *fake_fopen (const char *path, const char *mode);
char *fake_fgets (char *buf, int n, FILE *fp);
int fake_fclose (FILE *fp);

#ifdef UNIT_TEST
// SYSLOG
#define openlog(IDENT, LOGOPT, FACILITY) fake_openlog(IDENT, LOGOPT, FACILITY)
#define syslog(PRIORITY, ...) fake_syslog(PRIORITY, __VA_ARGS__)
#define closelog() fake_closelog()

// PWD
#define getpwnam_r(USER, PASSWD, BUFFER, BUFSIZE, PRESULT) fake_getpwnam_r(USER, PASSWD, BUFFER, BUFSIZE, PRESULT)

// SYS_STAT
#define stat(PATH, STRUCT) fake_stat(PATH, STRUCT)

// STDIO
#define fopen(PATH, MODE) fake_fopen(PATH, MODE)
#define fgets(DEST, DEST_SIZE, FILE_HANDLE) fake_fgets(DEST, DEST_SIZE, FILE_HANDLE)
#define fclose(FILE_HANDLE) fake_fclose(FILE_HANDLE)

#endif

#endif

