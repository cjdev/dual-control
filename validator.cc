#include <string>
#include <iostream>

#include "validator.h"

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

