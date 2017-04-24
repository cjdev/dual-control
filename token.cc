/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the  GNU General Public License version 2  or
 * later.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

#include "token.h"
#include "user.h"

namespace
{
class user_token_supplier_impl : public user_token_supplier_ifc
{
private:
    file_reader file_reader_;
public:
    user_token_supplier_impl (file_reader &file_reader) : file_reader_
        (file_reader) {}
    std::string token (user &user)
    {
        const std::string file_path (user.home_directory() + "/.dual_control");
        std::ifstream token_file;
        std::string fetched_token;

        bool token_file_opened = file_reader_.open (token_file, file_path);

        if (!token_file_opened) {
            return "";
        }

        return file_reader_.getline (token_file, fetched_token);
    }
};

class file_reader_impl : public file_reader_ifc
{
public:
    bool open (std::ifstream &token_file, const std::string &file_path)
    {
        token_file.open (file_path);
        return token_file.good();
    }
    std::string getline (std::ifstream &token_file, std::string &fetched_token)
    {
        std::getline (token_file, fetched_token);
        return fetched_token;
    }
};
}

file_reader file_reader::create ()
{
    return file_reader (file_reader::delegate (new
                        file_reader_impl ));
}

user_token_supplier user_token_supplier::create (file_reader &file_reader)
{
    return user_token_supplier (user_token_supplier::delegate
                                (new user_token_supplier_impl (file_reader)));
}

