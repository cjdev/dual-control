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
    class impl : public user_token_supplier_ifc {
        private:
            file_reader file_reader_;
        public:
            impl(file_reader &file_reader) : file_reader_(file_reader) {}
            std::string token(user &user) {
                std::string filepath = user.home_directory() + "/" + ".dual_control";
                file_reader_.read(filepath);
            }
    };
}
