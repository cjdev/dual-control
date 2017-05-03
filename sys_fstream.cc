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

#include <memory>
#include <fstream>
#include <iostream>

#include "sys_fstream.h"

namespace
{
class impl : public fstreams_ifc
{
public:
    pstream open_fstream (const std::string &file_path) const override
    {
        return pstream (new std::ifstream (file_path));
    }
    postream open_ofstream (const std::string &file_path, std::ios_base::open_mode mode) const override {
        return postream(new std::ofstream(file_path, mode));
    }

};

}

fstreams fstreams::create()
{
    static fstreams singleton (delegate (new impl));
    return singleton;
}

