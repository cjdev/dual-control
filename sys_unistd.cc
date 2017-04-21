#include "sys_unistd.h"
#include <unistd.h>

namespace {
    class impl : public unistd_ifc {
        public:
            long int sysconf(int name) {
                return ::sysconf(name);
            }
    };
    static unistd sys_unistd(unistd::delegate(new impl));
}

unistd unistd::system() {
    return sys_unistd;
}
