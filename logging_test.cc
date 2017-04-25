/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the  GNU General Public License  version 2  or
 * later.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#include <cstdio>
#include <cstring>
#include <syslog.h>

// #include "logging.h"
#include "test_util.h"



/*
int logged_priority = -1000;
const char *logged_message = "";
void fake_syslog (int priority, const char *message, ...)
{
    logged_priority = priority;
    logged_message = message;
}

int close_log_invoked = 0;
void fake_closelog (void)
{
    close_log_invoked = 1;
}

int opened_facility = -1000;
const char *opened_program_name = "";
int opened_logopt = -1000;
void fake_openlog (const char *ident, int logopt, int facility)
{
    opened_facility = facility;
    opened_program_name = ident;
    opened_logopt = logopt;
}

RESET_VARS_START
logged_priority = -1000;
close_log_invoked = 0;
opened_facility = -1000;
const char *opened_program_name = "";
int opened_logopt = -1000;
RESET_VARS_END

int test_log_success()
{
    // given

    // when
    log_success();

    // then
    checkint (LOG_AUTHPRIV, opened_facility, "facility");
    checkint (LOG_NOTICE, logged_priority, "priority");
    checkint (0, opened_logopt, "logopt");
    check (close_log_invoked, "log closed");
    checkstr ("pam_dual_control", opened_program_name, "program name");
    checkstr ("dual control succeeded", logged_message, "logged message");
    succeed();
}

int test_log_failure()
{
    //given

    //when
    log_failure();

    //then
    checkint (LOG_AUTHPRIV, opened_facility, "facility");
    checkint (LOG_NOTICE, logged_priority, "priority");
    checkint (0, opened_logopt, "logopt");
    check (close_log_invoked, "log closed");
    checkstr ("pam_dual_control", opened_program_name, "program name");
    checkstr ("dual control failed", logged_message, "logged message");
    succeed();
}

int test_runner()
{
    test (test_log_success);
    test (test_log_failure);
    succeed();
}

int main (int numargs, char **args)
{
    return !test_runner();
}
*/

