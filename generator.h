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

#pragma once
#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>

#include <cryptopp/base32.h>
#include <cryptopp/hex.h>
#include <cryptopp/hmac.h>
#include <cryptopp/osrng.h>

#include "sys_stdlib.h"
#include "sys_time.h"

using generator = std::function<std::string()>;
int ipow (int base, int exp);
time_t time_step (const time_t time, const int step);

class token_generator_ifc
{
public:
    virtual std::string generate_token () const
    {
        return "<dummy string>";
    }
};

class totp_generator
{
public:
    using delegate = std::shared_ptr<token_generator_ifc>;
private:
    delegate delegate_;

public:
    std::string generate_token () const
    {
        return delegate_->generate_token();
    };

    totp_generator (const sys_time &clock,
                    const std::string &key_c,
                    const int code_digits);
};

#endif

