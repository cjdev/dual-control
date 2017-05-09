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

class mock_syslog : public sys_syslog_ifc
{
public:
    int facility;
    std::string message;
    int priority;
    bool closed;
    std::string ident;
    mock_syslog() : closed (false), facility (-1000), priority (-1000) {}
    void openlog (const char *ident, int logopt, int facility)
    {
        this->facility = facility;
        this->ident = ident;
    }
    void vsyslog (int priority, const char *message, va_list args)
    {
        this->priority = priority;
        this->message = message;
    }
    void closelog()
    {
        this->closed = true;
    }

};

int logs_success()
{
    //given
    mock_syslog *capture = new mock_syslog;
    sys_syslog::delegate test_delegate (capture);
    sys_syslog test_syslog (test_delegate);
    logger logger = logger::create (test_syslog);
    std::string requester_user ("requester_user");
    std::string authorizer_user ("authorizer_user");
    std::string token ("token");

    //when
    logger.log (PAM_SUCCESS, requester_user, authorizer_user, token);

    //then
    check (capture->facility == LOG_AUTHPRIV, "facility does not match");
    check (capture->message == requester_user + " " + authorizer_user + " " +
           "success",
           "message does not match");
    check (capture->priority == LOG_NOTICE, "priority does not match");
    check (capture->closed, "syslog not closed");
    check (capture->ident == "dual-control", "dual-control");
    succeed();
}

int logs_failure()
{
    //given
    mock_syslog *capture = new mock_syslog;
    sys_syslog::delegate test_delegate (capture);
    sys_syslog test_syslog (test_delegate);
    logger logger = logger::create (test_syslog);
    std::string requester ("requestuser");
    std::string authorizer ("authuser");
    std::string token ("token");

    //when
    logger.log (PAM_AUTH_ERR, requester, authorizer, token);

    //then
    check (capture->facility == LOG_AUTHPRIV, "facility does not match");
    check (capture->message == requester + " " + authorizer + " "
           + "fail",
           "message does not match");
    check (capture->priority == LOG_NOTICE, "priority does not match");
    check (capture->closed, "syslog not closed");
    check (capture->ident == "dual-control", "dual-control");
    succeed();
}

int logs_pam_service_error()
{
    //given
    mock_syslog *capture = new mock_syslog;
    sys_syslog::delegate test_delegate (capture);
    sys_syslog test_syslog (test_delegate);
    logger logger = logger::create (test_syslog);
    std::string requester ("user");
    std::string authorizer ("user");
    std::string token ("token");

    //when
    logger.log (PAM_SERVICE_ERR, requester, authorizer, token);

    //then
    check (capture->facility == LOG_AUTH, "facility does not match");
    check (capture->message == requester + " " + authorizer +
           " pam returned error",
           "message does not match");
    check (capture->priority == LOG_ERR, "priority does not match");
    check (capture->closed, "syslog not closed");
    check (capture->ident == "dual-control", "dual-control");
    succeed();
}

int run_tests()
{
    test (logs_success);
    test (logs_failure);
    test (logs_pam_service_error);
    succeed();
}

int main (int numargs, char **args)
{
    return !run_tests();
}

