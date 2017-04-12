#include <string>

#include "validator.h"

bool validator::validate(const std::string &user_name, const std::string &token) {
    return user_name == "user" && token == "token";
}

