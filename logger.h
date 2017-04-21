/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the GNU Public License.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#ifndef __DUAL_CONTROL_LOGGING
#define __DUAL_CONTROL_LOGGING

#include <memory>
#include <string>

class logger_ifc
{
public:
    virtual ~logger_ifc() {}
    virtual void log (int result, const std::string &user_name,
                      const std::string &token) {};
};

class logger : public logger_ifc
{
private:
    std::shared_ptr<logger_ifc> delegate_;
public:
    logger (const std::shared_ptr<logger_ifc> &delegate) : delegate_
        (delegate) {}
    logger() : logger (std::shared_ptr<logger_ifc> (new logger_ifc)) {}
    void log (int result, const std::string &user_name,
              const std::string &token)
    {
        delegate_->log (result, user_name, token);
    }
};

#endif

