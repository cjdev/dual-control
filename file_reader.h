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

#ifndef _FILE_READER_H
#define _FILE_READER_H

#include <string>
#include <memory>

#include "user.h"


class file_reader_ifc
{
public:
    virtual bool open (std::ifstream &token_file, const std::string &file_path)
    {
        return false;
    }
    virtual std::string getline (std::ifstream &token_file,
                                 std::string &line)
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
    bool open (std::ifstream &token_file, const std::string &file_path)
    {
        return delegate_->open (token_file, file_path);
    }
    std::string getline (std::ifstream &token_file, std::string &line)
    {
        return delegate_->getline (token_file, line);
    }

    static file_reader create ();
};

#endif

