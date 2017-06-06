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

#ifndef BASE32_H
#define BASE32_H

#include <memory>
#include <vector>
#include <string>

class base32_ifc
{
public:
    virtual std::string encode (std::vector<uint8_t> input) = 0;
    virtual std::vector<uint8_t> decode (std::vector<uint8_t> input) = 0;
};

class base32
{
public:
    using delegate = std::shared_ptr<base32_ifc>;
private:
    delegate delegate_;
public:
    base32 ();

    std::string encode (std::vector<uint8_t> input) {
        return delegate_->encode(input);
    };

    std::vector<uint8_t> decode (std::vector<uint8_t> input) {
        return delegate_->decode(input);
    };
};

#endif
