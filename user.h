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

#ifndef _USER_H
#define _USER_H
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "sys_unistd.h"
#include "sys_pwd.h"

class user_ifc
{
public:
    virtual ~user_ifc() {}
    virtual std::string home_directory() const
    {
        return "";
    }
};

class user
{
public:
    typedef std::shared_ptr<user_ifc> delegate;
private:
    delegate delegate_;
public:
    user (delegate delegate) : delegate_ (delegate)
    {
    }
    user() : user (delegate (new user_ifc)) {}
    std::string home_directory() const
    {
        return delegate_-> home_directory();
    }
    static user create (const passwd &passwd);
};

class directory_ifc
{
public:
    virtual ~directory_ifc() {}
    virtual std::vector<user> find_user (const std::string &user_name)
    {
        return std::vector<user>();
    }
};

class directory : public directory_ifc
{
public:
    typedef std::shared_ptr<directory_ifc> delegate;
private:
    delegate delegate_;
public:
    directory (delegate delegate) : delegate_
        (delegate) {}
    directory() : directory (delegate (new directory_ifc)) {}
    std::vector<user> find_user (const std::string &user_name)
    {
        return delegate_->find_user (user_name);
    }

    static directory create (unistd &unistd, pwd &pwd);
};

#endif

