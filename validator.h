#ifndef _VALIDATOR_H
#define _VALIDATOR_H

#include <string>
#include <memory>

#include "user.h"
#include "token.h"

class validator_ifc {
    public:
        virtual ~validator_ifc() {}
        virtual bool validate(const std::string &user, const std::string &token) {
           return false;
        }
};

typedef std::shared_ptr<validator_ifc> validator;

class old_validator {
    private:
        directory_p directory_;
        user_token_supplier_p user_token_supplier_;
    public:
        old_validator(const directory_p &directory, const user_token_supplier_p &user_token_supplier):
            directory_(directory),
            user_token_supplier_(user_token_supplier) {}
        bool validate(const std::string &user, const std::string &token);
};

#endif
