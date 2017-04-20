#include <string>
#include <iostream>

#include "validator.h"

namespace
{
    class impl : public validator_ifc {
        private:
            directory_p directory_;
            user_token_supplier_p user_token_supplier_;
        public:
        impl(const directory_p &directory, const user_token_supplier_p user_token_supplier) :
            directory_(directory),
            user_token_supplier_(user_token_supplier) {}
        bool validate (const std::string &user_name,
                              const std::string &token)
        {
            user_p found_user = directory_->find_user (user_name);

            if (!found_user) {
                return false;
            }

            std::string user_token = user_token_supplier_->token (found_user);
            return user_token == token;
        }
    };
}

validator create_validator(const directory_p &directory, const user_token_supplier_p &user_token_supplier) {
    std::shared_ptr<validator_ifc> delegate(new impl(directory, user_token_supplier));
    return validator(delegate);
}

/*
bool old_validator::validate (const std::string &user_name,
                              const std::string &token)
{
    user_p found_user = directory_->find_user (user_name);

    if (!found_user) {
        return false;
    }

    std::string user_token = user_token_supplier_->token (found_user);
    return user_token == token;
}
*/

