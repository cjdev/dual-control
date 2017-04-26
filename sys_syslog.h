#ifndef SYS_SYSLOG_H
#define SYS_SYSLOG_H

#include <memory>
#include <cstdarg>
#include <syslog.h>

class sys_syslog_ifc {
    public:
        virtual void openlog(const char *ident, int logopt, int facility) {}
        virtual void vsyslog(int priority, const char *message, va_list args) {}
        virtual void closelog() {}
};

class sys_syslog : public sys_syslog_ifc {
    public:
        typedef std::shared_ptr<sys_syslog_ifc> delegate;
    private:
        delegate delegate_;
    public:
       sys_syslog(const delegate &delegate) : delegate_(delegate) {}
       sys_syslog() : sys_syslog(delegate(new sys_syslog_ifc)) {}
        void openlog(const char *ident, int logopt, int facility) {
            delegate_->openlog(ident, logopt, facility);
        }
        void syslog(int priority, const char *format, ...) {
            va_list vargs;
            va_start(vargs, format);
            vsyslog(priority, format, vargs);
            va_end(vargs);
        }
         void vsyslog(int priority, const char *message, va_list vargs) {
            delegate_->vsyslog(priority, message, vargs);
        }
        void closelog() {
           delegate_->closelog();
        }

        static sys_syslog create();
};

#endif
