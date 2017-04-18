#ifndef __DUAL_CONTROL_LOGGING
#define __DUAL_CONTROL_LOGGING

#include <memory>
#include <string>

class logger_ifc {
    public:
        virtual ~logger_ifc() {}
        virtual void log(int result, const std::string &user_name, const std::string &token) {};
};
typedef std::shared_ptr<logger_ifc> logger;

#endif
