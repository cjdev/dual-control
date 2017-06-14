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
#include "random_source.h"

namespace
{
class tokens_impl : public tokens_ifc
{
private:
    fstreams fstreams_;
    totp_generator generator_;
    random_source rand_;
public:
    tokens_impl (const fstreams &fstreams, const totp_generator generator, const random_source rand) :
        fstreams_ (fstreams), generator_ (generator), rand_ (rand) {}
    std::string token (const user &user) const override
    {
        // Get key
        std::string line = read_key (user);

        if (line == "") {
            return "";
        }

        base32 codec;
        std::vector<uint8_t> key = codec.decode (line);

        return generator_.generate_token (std::string (key.begin(), key.end()));
    }

private:
    std::string get_key_path (const user &user) const
    {
        return user.home_directory() + "/.dual_control";
    }

    bool key_exists (const user &user) const
    {
        // check if file exists
        std::string file_path = get_key_path (user);
        fstreams::pstream stream (fstreams_.open_fstream (file_path));
        return stream->good();
    }

    std::string read_key (const user &user) const
    {
        std::string file_path = get_key_path (user);
        fstreams::pstream stream (fstreams_.open_fstream (file_path));

        // TODO: ignore newlines
        std::vector<char> line_v (16);
        stream->read (line_v.data(), line_v.size());

        if (stream->fail()) {
            return "";
        }

        std::string line (line_v.begin(), line_v.end());
        return line;
    }
public:
    std::string generate_key() const override
    {
        base32 codec;
        // get randomness
        int length = 10;
        std::vector<uint8_t> random_bytes (rand_.get_random_bytes(length));
        // base32encode it
        std::string key = codec.encode (random_bytes);
        return key;
    }

    std::string ensure_key (const user &user) const override
    {
        if (!key_exists (user)) {
            std::string key = generate_key();
            save (user, key);
            return key;
        } else {
            return read_key (user);
        }
    }

    void save (const user &user, const std::string &key) const override
    {
        std::string file_path = get_key_path (user);
        fstreams::postream stream (fstreams_.open_ofstream (file_path,
                                   std::ios_base::trunc));
        *stream << key<< std::endl;
    }
};
}

tokens tokens::create (const fstreams &fstreams,
                       const totp_generator &generator,
                       const random_source &rand)
{
    return tokens (tokens::delegate
                   (new tokens_impl (fstreams, generator, rand)));
}
