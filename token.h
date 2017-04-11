#ifndef _TOKEN_H
#define _TOKEN_H
#include <string>
#include <memory>

#include "user.h"

class token {
       std::string value_
    public:
       token(const std::string &value) : value_(value) {}
       std::string value() { return value_; }
}

shared_ptr<token> create_token(std::shared_ptr<user> user);

int validate_token(const char *user, const char *token);

#endif
