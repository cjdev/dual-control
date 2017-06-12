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
#include "base32.h"
#include "sys_fstream.h"

namespace
{
class tokens_impl : public tokens_ifc
{
private:
    fstreams fstreams_;
    totp_generator generator_;
public:
    tokens_impl (const fstreams &fstreams, const totp_generator generator) :
        fstreams_ (fstreams), generator_(generator) {}
    std::string token (const user &user) const override
    {
        // Get key
        const std::string file_path (user.home_directory() + "/.dual_control");
        fstreams::pstream stream (fstreams_.open_fstream (file_path));

        // TODO: ignore newlines
        std::vector<char> line_v(16);
        stream->read(line_v.data(), line_v.size());

        if (stream->fail()) {
            return "";
        }

        base32 codec;
        std::string line(line_v.begin(), line_v.end());
        std::vector<uint8_t> key = codec.decode(line);

        // TODO: generate the token
        return generator_.generate_token (std::string (key.begin(), key.end()));
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

tokens tokens::create (const fstreams &fstreams, const totp_generator &generator)
{
    return tokens (tokens::delegate
                   (new tokens_impl (fstreams, generator)));
}
