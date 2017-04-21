/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the GNU Public License.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

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

