#ifndef _VALIDATOR_H
#define _VALIDATOR_H

#include <string>

#include "user.h"

class validator {
    private:
        directory_p directory_;
    public:
        validator(const directory_p &directory): directory_(directory) {}
        bool validate(const std::string &user, const std::string &token);
};

#endif
