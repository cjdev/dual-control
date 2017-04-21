#include "sys_pwd.h"

#include <pwd.h>

namespace {
    class impl : public pwd_ifc {
        public:
        int getpwnam_r(const char *user_name, passwd *out, char *buffer,
                size_t buffer_sz, passwd **result) {
            return ::getpwnam_r(user_name, out, buffer, buffer_sz, result);
        }

    };
    static pwd system_pwd(pwd::delegate(new impl));
}

pwd pwd::system() {
    return system_pwd;
}

