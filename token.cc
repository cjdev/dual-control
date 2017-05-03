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
#include <iostream>
#include <functional>

#include "token.h"
#include "user.h"
#include "sys_fstream.h"

namespace
{
class tokens_impl : public tokens_ifc
{
private:
    fstreams fstreams_;
    token_generator generate_token_;
public:
    tokens_impl (const fstreams &fstreams,
                 const token_generator &generate_token) :
        generate_token_ (generate_token),
        fstreams_ (fstreams) {}
    std::string token (const user &user) const override
    {
        const std::string file_path (user.home_directory() + "/.dual_control");
        std::vector<char> line (7);

        fstreams::pstream stream (fstreams_.open_fstream (file_path));

        if (!stream->good()) {
            return "";
        }

        stream->getline (line.data(), line.size());
        return std::string (line.data());
    }
    void create (const user &user) const override
    {
        std::string generated_token (generate_token_());
        std::string file_path (user.home_directory() + "/.dual_control");
        fstreams::postream stream (fstreams_.open_ofstream (file_path,
                                   std::ios_base::trunc));
        *stream << generated_token << std::endl;
    }
};
}
tokens tokens::create (const fstreams &fstreams,
                       const tokens_impl::token_generator &generate_token)
{
    return tokens (tokens::delegate
                   (new tokens_impl (fstreams, generate_token)));
}

