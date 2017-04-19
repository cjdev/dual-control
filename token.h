#ifndef _TOKEN_H
#define _TOKEN_H
#include <string>
#include <memory>

#include "user.h"


class user_token_supplier
{
public:
    virtual std::string token(const user_p user) = 0;
};
typedef std::shared_ptr<user_token_supplier> user_token_supplier_p;


#endif
