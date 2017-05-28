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

#include "generator.h"
#include <iostream>

namespace {
int ipow (int base, int exp)
{
    int result = 1;

    while (exp) {
        if (exp & 1) {
            result *= base;
        }

        exp >>= 1;
        base *= base;
    }

    return result;
}

unsigned long bytesToInt (const std::string &bytes)
{
    unsigned long result = 0;
    auto          byteCount = bytes.size() - 1;

    for (auto byte = bytes.cbegin(); byte < bytes.cend(); byte++, byteCount--) {
        const uint8_t val = static_cast<uint8_t> (*byte);
        result |= val << (byteCount * 8);
    }

    return result;
}

time_t time_step (const time_t time, const int step)
{
    // Time is > 0 so division produces the result we want.
    return time / step;
}

class token_generator_impl : public token_generator_ifc
{
private:
    unsigned long truncate (const std::string &mac) const
    {
        uint8_t offset = static_cast<uint8_t > (mac[19]) & static_cast<uint8_t>
                         (0x0f);
        std::string  offsetBytes = mac.substr (offset, 4);
        return bytesToInt (offsetBytes) & 0x7fffffff;
    }

    unsigned long hotp (const CryptoPP::SecByteBlock &key,
                        const CryptoPP::Integer &counter) const
    {
        std::string mac;

        CryptoPP::SecByteBlock counter_bytes (8);
        // Do I know that 8 is sufficient here? . . .
        counter.Encode (counter_bytes.BytePtr(), 8, CryptoPP::Integer::UNSIGNED);

        CryptoPP::HMAC<CryptoPP::SHA1> hmac (key, key.size());

        CryptoPP::StringSink  *stringSink = new CryptoPP::StringSink (mac);
        CryptoPP::HashFilter  *hashFilter = new CryptoPP::HashFilter (hmac,
                stringSink);
        CryptoPP::StringSource ss2 (counter_bytes, counter_bytes.size(), true,
                                    hashFilter);

        unsigned long result = truncate (mac);

        result = result % ipow (10, code_digits);
        return result;
    }

    CryptoPP::SecByteBlock generate_key (unsigned int size)
    const
    {
        CryptoPP::AutoSeededRandomPool prng;

        CryptoPP::SecByteBlock key (size);
        prng.GenerateBlock (key, key.size());
        return key;
    }

    const sys_time &clock;
    unsigned int code_digits;
    const std::shared_ptr<CryptoPP::SecByteBlock> key;
public:
    token_generator_impl (const sys_time &clock,
          const std::string &key_c,
          const int code_digits) :
        clock (clock), code_digits (code_digits),
        key (std::make_shared<CryptoPP::SecByteBlock> (CryptoPP::SecByteBlock (
                    reinterpret_cast<const unsigned char *> (key_c.c_str()), key_c.size())))
    {}

    std::string generate_token () const override
    {
        clock.time(nullptr);
        time_t foo = 111;
        const CryptoPP::Integer &time = clock.time (&foo);
        int time_step_size = 30;
        CryptoPP::Integer current_step = time_step (time.ConvertToLong(),
                                         time_step_size);
        long otp = hotp (*key, current_step);
        std::ostringstream is;
        is << std::setfill ('0') << std::setw (6)<< otp;
        return is.str();
    }
};
}

// Generator goes here....

totp_generator::totp_generator (
    const sys_time &clock,
    const std::string &key_c,
    const int code_digits) :
    delegate_ (std::make_shared<token_generator_impl> (clock, key_c, code_digits))
{}
