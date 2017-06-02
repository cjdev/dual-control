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

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <ctime>

#include "sys_stdlib.h"
#include "sys_time.h"

int ipow (int base, int exp);
time_t time_step (const time_t time, const int step);

class token_generator_ifc
{
public:
    virtual std::string generate_token () const = 0;
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

    totp_generator (delegate delegate_) :
        delegate_ (delegate_)
    {}

    totp_generator (const sys_time &clock,
                    const std::string &key_c,
                    const int code_digits);
};

#endif
