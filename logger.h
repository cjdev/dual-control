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
    virtual void log (int result, const std::string &user_name,
                      const std::string &token) {};
};

class logger : public logger_ifc
{
public:
    typedef std::shared_ptr<logger_ifc> delegate;
private:
    delegate delegate_;
public:
    logger (const delegate &delegate) : delegate_
        (delegate) {}
    logger() : logger (delegate (new logger_ifc)) {}
    void log (int result, const std::string &user_name,
              const std::string &token)
    {
        delegate_->log (result, user_name, token);
    }
    static logger create (const sys_syslog &sys_syslog);
};

#endif

