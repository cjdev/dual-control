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

#include "random_source.h"
#include "sys_fstream.h"

namespace
{
class invalid_random_source_exception : public std::exception
{};

class impl : public random_source_ifc
{
private:
    fstreams fstreams_;
public:
    impl (fstreams fstreams)
        : fstreams_ (fstreams)
    {}
    std::vector<uint8_t> get_random_bytes (int length) const override
    {
        fstreams::pstream random_source = fstreams_.open_fstream (file_path);

        std::vector<uint8_t> result (length);
        random_source->read (reinterpret_cast<char *> (result.data()), length);

        if (random_source->fail()) {
            throw invalid_random_source_exception();
        }

        return result;
    }
};
}

const std::string random_source_ifc::file_path = "/dev/urandom";

random_source random_source::create (fstreams &fstreams)
{
    return random_source (random_source::delegate (new impl (fstreams)));
};

