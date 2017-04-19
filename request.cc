#include <vector>
#include <string>

#include "request.h"

std::vector<std::string> pam_request::arguments()
{
    std::vector<std::string> rval;

    for (int i= 0; i < argc_; i++) {
        rval.push_back (argv_[i]);
    }

    return rval;
}

