#ifndef _VALIDATOR_
#define _VALIDATOR_

#include <string>

class validator {
    public:
        bool validate(const std::string &user, const std::string &token);
};

#endif
