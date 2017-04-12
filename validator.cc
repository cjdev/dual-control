#include <string>

#include "validator.h"

bool validator::validate(const std::string &user_name, const std::string &token) {
    return directory_->find_user(user_name) && token == "token";
}

