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
public:
    tokens_impl (const fstreams &fstreams) :
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
    void save (const user &user, const std::string &token) const override
    {
        std::string file_path (user.home_directory() + "/.dual_control");
        fstreams::postream stream (fstreams_.open_ofstream (file_path,
                                   std::ios_base::trunc));
        *stream << token << std::endl;
    }
};
}
tokens tokens::create (const fstreams &fstreams)
{
    return tokens (tokens::delegate
                   (new tokens_impl (fstreams)));
}

