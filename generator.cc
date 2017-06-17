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

#include <openssl/hmac.h>
#include <openssl/evp.h>

#include "base32.h"
namespace
{
class hmac_failed_exception : public std::exception
{};

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

unsigned char *timeToBytes (unsigned long long time, unsigned char *data,
                            size_t data_size)
{
    for (int idx = data_size - 1; idx > -1; idx--) {
        unsigned char next_digit = time & 0xff;
        data[idx] = next_digit;
        time >>= 8;
    }

    return data;
}

template <typename S>
unsigned long bytesToInt (const S &bytes)
{
    unsigned long result = 0;
    auto          byteCount = bytes.size() - 1;

    for (auto byte = bytes.cbegin(); byte < bytes.cend(); byte++, byteCount--) {
        const uint8_t val = static_cast<uint8_t> (*byte);
        result |= val << (byteCount * 8);
    }

    return result;
}

class token_generator_impl : public token_generator_ifc
{
private:
    const sys_time clock;
    const unsigned int code_digits;
    const base32 codec;

private:
    std::string zero_fill (unsigned long result, int digits) const
    {
        std::ostringstream result_stream;
        result_stream << std::setw (digits) << std::setfill ('0') << result;
        return result_stream.str();
    }

    unsigned long truncate (const octet_vector &mac) const
    {
        uint8_t offset = static_cast<uint8_t > (mac[19]) & static_cast<uint8_t>
                         (0x0f);

        auto mac_begin = mac.begin();
        auto subseq_begin = mac_begin + offset;
        octet_vector  offsetBytes (subseq_begin, subseq_begin + 4);

        return bytesToInt (offsetBytes) & 0x7fffffff;
    }

    std::string hotp (const std::vector<uint8_t> &key,
                      const unsigned char *data,
                      size_t data_size, const int digits=6) const
    {
        // TODO: see if I can use sha256/etc. with google auth...
        const unsigned char *digest = HMAC (EVP_sha1(), key.data(), key.size(),
                                            data,
                                            data_size, NULL, NULL);

        if (digest == nullptr) {
            throw hmac_failed_exception();
        }

        size_t sha1_output_size = 20;
        // const_cast should be safe here because we generated the pointer
        octet_vector digest_s = octet_vector (digest, digest+sha1_output_size);

        unsigned long result = truncate (digest_s) % ipow (10,digits);

        return zero_fill (result, digits);
    }

public:
    token_generator_impl (const sys_time clock,
                          const int code_digits) :
        clock (clock), code_digits (code_digits)
    {}

    std::string generate_token (const octet_vector &key) const override
    {
        // Assuming time is > 0, integer division produces the result we want.
        const time_t &time_chunk = clock.time (nullptr) / 30;

        unsigned char data[8] = {0,0,0,0,0,0,0,0};
        timeToBytes (time_chunk, data, 8);

        return hotp (key, data, 8, code_digits);
    }
};
}

// Generator goes here....

totp_generator::totp_generator (
    const sys_time clock,
    const int code_digits) :
    delegate_ (std::make_shared<token_generator_impl> (clock, code_digits))
{}

