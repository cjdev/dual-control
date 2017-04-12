#ifndef _VALIDATOR_H
#define _VALIDATOR_H

#include <string>

class validator {
    public:
        bool validate(const std::string &user, const std::string &token);
};

#endif
