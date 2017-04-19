#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <string>
#include <vector>
#include <security/pam_modules.h>

class pam_request {
        int argc_;
        const char **argv_;
        int flags_;
        pam_handle *handle_;
    public:
        pam_request(pam_handle *handle, int flags, int argc, const char **argv)
            : handle_(handle),
              flags_(flags),
              argc_(argc),
              argv_(argv) {}
        std::vector<std::string> arguments();
        int flags() { return flags_; }
        pam_handle *handle() {return handle_; }
};

#endif

