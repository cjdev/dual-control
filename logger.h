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

