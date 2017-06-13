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

#ifndef _RANDOM_SOURCE_H
#define _RANDOM_SOURCE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "sys_unistd.h"
#include "sys_pwd.h"
#include "sys_fstream.h"

class random_source_ifc
{
 private:
    fstreams fstreams_;
 public:
    random_source_ifc (fstreams fstreams)
        : fstreams_ (fstreams)
    {}
    virtual std::vector<uint8_t> get_random_bytes (int length) const {
        std::string file_path = "/dev/urandom";
        fstreams::pstream random_source = fstreams_.open_fstream(file_path);

        std::vector<uint8_t> result(length);
        random_source->read(reinterpret_cast<char *>(result.data()), length);
        return result;
        /* return {}; */
    }
};

class random_source
{
 public:
    using delegate = std::shared_ptr<random_source_ifc>;
 private:
    delegate delegate_;
 public:
    random_source (delegate delegate)
        : delegate_ (delegate)
    {}
    std::vector<uint8_t> get_random_bytes (int length) const {
        return delegate_->get_random_bytes(length);
    }
};

#endif
