/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the  GNU General Public License  version 2  or
 * later.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>
#include <memory>

#include "user.h"
#include "file_reader.h"

class user_token_supplier_ifc
{
public:
    virtual ~user_token_supplier_ifc() {}
    virtual std::string token (user &user)
    {
        return "";
    }
};

class user_token_supplier : public user_token_supplier_ifc
{
public:
    typedef std::shared_ptr<user_token_supplier_ifc> delegate;
private:
    delegate delegate_;
public:
    user_token_supplier (delegate delegate) :
        delegate_ (delegate) {}
    user_token_supplier() : user_token_supplier (
            delegate (new user_token_supplier_ifc)) {}
    std::string token (user &user)
    {
        return delegate_->token (user);
    }
    static user_token_supplier create (file_reader &file_reader);
};

#endif

