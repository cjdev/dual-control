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
#include <iostream>
#include "user.h"

class user_token_supplier_ifc
{
public:
    virtual ~user_token_supplier_ifc() {}
    virtual std::string token (user &user)
    {
        return "virtual supplier";
    }
};

class file_reader_ifc
{
public:
    virtual std::string getline(std::ifstream &token_file, std::string &fetched_token) {
        return "";
    }
    virtual bool open(std::ifstream &token_file, const std::string &file_path) {
        return false;
    }
    virtual std::string read (std::string file_path)
    {
        return "";
    }
};

class file_reader : public file_reader_ifc
{
public:
    typedef std::shared_ptr<file_reader_ifc> delegate;
private:
    delegate delegate_;
public:
    file_reader (delegate delegate) :
        delegate_ (delegate) {}
    file_reader() : delegate_ (delegate (new file_reader_ifc)) {}
    bool open(std::ifstream &token_file, const std::string &file_path) {
        return delegate_->open(token_file, file_path);
    }
    std::string getline(std::ifstream &token_file, std::string &fetched_token) {
        return delegate_->getline(token_file, fetched_token);
    }
    std::string read (std::string file_path)
    {
        return delegate_->read (file_path);
    }
    static file_reader create ();
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

