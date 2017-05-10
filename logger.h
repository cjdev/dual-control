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

#ifndef __DUAL_CONTROL_LOGGING
#define __DUAL_CONTROL_LOGGING

#include <memory>
#include <string>

#include "sys_syslog.h"

class logger_ifc
{
public:
    virtual ~logger_ifc() {}
    virtual void log (int result, const std::string &requester_user_name,
                      const std::string &authorizer_user_name,
                      const std::string &token,
                      const std::string &reason) {};
};

class logger
{
public:
    typedef std::shared_ptr<logger_ifc> delegate;
private:
    delegate delegate_;
public:
    logger (const delegate &delegate) : delegate_
        (delegate) {}
    logger() : logger (delegate (new logger_ifc)) {}
    void log (int result, const std::string &requester_user_name,
              const std::string &authorizer_user_name,
              const std::string &token,
              const std::string &reason)
    {
        delegate_->log (result, requester_user_name, authorizer_user_name, token,
                        reason);
    }
    static logger create (const sys_syslog &sys_syslog);
};

#endif

