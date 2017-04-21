#ifndef _SYS_PWD_H
#define _SYS_PWD_H

#include <memory>
#include <pwd.h>

class pwd_ifc {
    public:
        virtual int getpwnam_r(const char *user_name, passwd *out, char *buffer,
                size_t buffer_sz, passwd **result) {
            *result = 0;
            return 0;
        };
};

class pwd : public pwd_ifc {
    public:
        typedef std::shared_ptr<pwd_ifc> delegate;

    private:
        delegate delegate_;

    public:
        pwd(const delegate delegate) : delegate_(delegate) {}
        pwd() : delegate_(delegate(new pwd_ifc)) {}
        int getpwnam_r(const char *user_name, passwd *out, char *buffer,
                size_t buffer_sz, passwd **result) {
            return delegate_-> getpwnam_r(user_name, out, buffer, buffer_sz, result);
        }
        static pwd system();
};

#endif
