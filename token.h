/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the GNU Public License.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#ifndef _TOKEN_H
#define _TOKEN_H
#include <string>
#include <memory>
#include <iostream>
#include "user.h"

class user_token_supplier_ifc
{
public:
    virtual std::string token (const user &user)
    {
        return "";
    }
};

class user_token_supplier : public user_token_supplier_ifc
{
private:
    std::shared_ptr<user_token_supplier_ifc> delegate_;
public:
    user_token_supplier (std::shared_ptr<user_token_supplier_ifc> delegate) :
        delegate_ (delegate) {}
    user_token_supplier() : user_token_supplier (
            std::shared_ptr<user_token_supplier_ifc> (new user_token_supplier_ifc)) {}
    std::string token (const user &user)
    {
        return delegate_->token (user);
    }
};

#endif

