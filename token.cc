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

#include "token.h"
#include "user.h"

namespace
{
class impl : public user_token_supplier_ifc
{
private:
    file_reader file_reader_;
public:
    impl (file_reader &file_reader) : file_reader_ (file_reader) {}
    std::string token (user &user)
    {
        std::string file_path (user.home_directory() + "/.dual_control");
        std::string fetched_token (file_reader_.read (file_path));
        return fetched_token;
    }
};

class file_reader_impl : public file_reader_ifc
{
public:
    std::string read (std::string file_path)
    {
        return file_path;
    }
};
}

file_reader file_reader::create ()
{
    return file_reader (std::shared_ptr<file_reader_ifc> (new
                        file_reader_impl ));
}

user_token_supplier user_token_supplier::create (file_reader &file_reader)
{
    return user_token_supplier (std::shared_ptr<user_token_supplier_ifc>
                                (new impl (file_reader)));
}

