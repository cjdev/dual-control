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

#include <syslog.h>
#include <security/pam_modules.h>

#include <iostream>

#include "sys_syslog.h"
#include "logger.h"
#include "test_util.h"

class mock_syslog : public sys_syslog_ifc {
    public:
        int facility;
        std::string message;
        int priority;
        bool closed;
        std::string ident;
        mock_syslog() : closed(false), facility(-1000), priority(-1000) {}
        void openlog(const char *ident, int logopt, int facility) {
            this->facility = facility;
            this->ident = ident;
        }
        void vsyslog(int priority, const char *message, va_list args) {
            this->priority = priority;
            this->message = message;
        }
        void closelog()
        {
            this->closed = true;
        }


};


int logs_success() {
    //given
    mock_syslog *capture = new mock_syslog;
    sys_syslog::delegate test_delegate(capture);
    sys_syslog test_syslog(test_delegate);
    logger logger = logger::create(test_syslog);
    std::string user("user");
    std::string token("token");

    //when
    logger.log(PAM_SUCCESS, user, token);

    //then
    check(capture->facility == LOG_AUTHPRIV, "facility does not match");
    check(capture->message == user + " " + token + " " + "success", "message does not match");
    check(capture->priority == LOG_NOTICE, "priority does not match");
    check(capture->closed, "syslog not closed");
    check(capture->ident == "dual-control", "dual-control");
    succeed();
}

int logs_failure() {
    //given
    mock_syslog *capture = new mock_syslog;
    sys_syslog::delegate test_delegate(capture);
    sys_syslog test_syslog(test_delegate);
    logger logger = logger::create(test_syslog);
    std::string user("user");
    std::string token("token");

    //when
    logger.log(PAM_AUTH_ERR, user, token);

    //then
    check(capture->facility == LOG_AUTHPRIV, "facility does not match");
    check(capture->message == user + " " + token + " " + "fail", "message does not match");
    check(capture->priority == LOG_NOTICE, "priority does not match");
    check(capture->closed, "syslog not closed");
    check(capture->ident == "dual-control", "dual-control");
    succeed();
}

RESET_VARS_START
RESET_VARS_END

int run_tests()
{
    test (logs_success);
    test (logs_failure);
    succeed();
}

int main (int numargs, char **args)
{
    return !run_tests();
}


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

