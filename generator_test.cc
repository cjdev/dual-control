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
#include <algorithm>
#include <initializer_list>
#include <vector>
#include <climits>
#include <ctime>
#include <iostream>

#include "generator.h"
#include "sys_stdlib.h"
#include "sys_time.h"
#include "test_util.h"
#include "typealiases.h"

class fake_stdlib : public stdlib_ifc
{

private:
    std::vector<int> samples_;
    mutable std::vector<int>::iterator current_;
public:
    fake_stdlib ( const std::initializer_list<int> &samples)
        : samples_ (samples.begin(), samples.end()),
          current_ (samples_.begin()) {}
    int rand() const override
    {
        if (current_ != samples_.end()) {
            auto rval = *current_;
            current_ += 1;
            return rval;
        }

        return 0;
    }
};

class fake_time : public sys_time_ifc
{

private:
    std::vector<time_t> samples_;
    mutable std::vector<time_t>::iterator current_;
public:
    fake_time ( const std::initializer_list<time_t> &samples)
        : samples_ (samples.begin(), samples.end()),
          current_ (samples_.begin()) {}
    time_t time (time_t *time_ptr) const override
    {
        if (current_ != samples_.end()) {
            auto rval = *current_;

            if (time_ptr != nullptr) {
                *time_ptr = rval;
            }

            current_ += 1;
            return rval;
        }

        return 0;
    }
};

int given_digits()
{
    // given
    std::initializer_list<time_t> samples { 1 };
    auto test_stdtime = std::make_shared<fake_time> (samples);

    sys_time stdtime (test_stdtime);
    // Fake the Key
    auto generator = totp_generator (stdtime, 6);

    // when
    octet_vector key { 0 };
    auto actual = generator.generate_token (key);

    // then
    check (actual.size() == 6, "size is wrong");
    check (std::all_of (actual.begin(), actual.end(), [] (char c) {
        return c >= '0' && c <= '9';
    }), "not just digits");
    succeed();
}

int modulated_source_modulates_tokens()
{
    // given
    std::initializer_list<time_t> samples { 1, 31 };
    auto test_stdtime = std::make_shared<fake_time> (samples);

    sys_time stdtime (test_stdtime);
    auto generator = totp_generator (stdtime, 6);

    // when
    octet_vector key { 0 };
    auto actual1 = generator.generate_token (key);
    auto actual2 = generator.generate_token (key);

    // then
    check (actual1 != actual2, "tokens should be different");
    succeed();
}

int int_max()
{
    // given
    std::initializer_list<time_t> samples { INT_MAX };
    auto test_stdtime = std::make_shared<fake_time> (samples);

    sys_time stdtime (test_stdtime);
    auto generator = totp_generator (stdtime, 6);
    octet_vector key { 0 };

    // when
    auto actual = generator.generate_token (key);

    // then
    check (actual.size() == 6, "size is wrong");
    check (std::all_of (actual.begin(), actual.end(), [] (char c) {
        return c >= '0' && c <= '9';
    }), "not just digits");
    succeed();
}

int int_min()
{
    // given
    std::initializer_list<time_t> samples { INT_MIN };
    auto test_stdtime = std::make_shared<fake_time> (samples);
    octet_vector key { 0 };

    sys_time stdtime (test_stdtime);
    auto generator = totp_generator (stdtime, 6);

    // when
    auto actual = generator.generate_token (key);

    // then
    check (actual.size() == 6, "size is wrong");
    check (std::all_of (actual.begin(), actual.end(), [] (char c) {
        return c >= '0' && c <= '9';
    }), "not just digits");
    succeed();
}

// totp test
int int_precomputed()
{
    // given
    // The token for key 76I6WTYEUTNCJUREMGKVM45PMA and time '2017/01/01 00:00:00' is 258675
    time_t theTime = 1483257600;
    std::initializer_list<time_t> samples { theTime }; //
    auto test_stdtime = std::make_shared<fake_time> (samples);

    sys_time stdtime (test_stdtime);
    // Fake the Key
    octet_vector key {255, 145, 235, 79, 4, 164, 218, 36, 210, 36, 97, 149, 86, 115, 175, 96};
    auto generator = totp_generator (stdtime, 6);
    std::string expected = "258675";

    // when
    auto actual = generator.generate_token (key);

    // then
    check (actual.size() == 6, "size is wrong");
    check (actual == expected, "precomputed value failed to match");
    succeed();
}

int run_tests()
{
    test (given_digits);
    test (modulated_source_modulates_tokens);
    test (int_max);
    test (int_min);
    test (int_precomputed);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}
