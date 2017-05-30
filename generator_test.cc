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

int six_digits()
{
    // given
    std::initializer_list<time_t> samples { 1 };
    auto test_stdtime = std::make_shared<fake_time> (samples);

    sys_time stdtime (test_stdtime);
    // Fake the Key
    std::string key = "\xff\x91\xebO\x04\xa4\xda$\xd2$a\x95Vs\xaf`";
    auto generator = totp_generator (stdtime, key, 6);

    // when
    auto actual = generator.generate_token();

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
    // Fake the Key
    std::string key = "\xff\x91\xebO\x04\xa4\xda$\xd2$a\x95Vs\xaf`";
    auto generator = totp_generator (stdtime, key, 6);

    // when
    auto actual1 = generator.generate_token();
    auto actual2 = generator.generate_token();

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
    // Fake the Key
    std::string key = "\xff\x91\xebO\x04\xa4\xda$\xd2$a\x95Vs\xaf`";
    auto generator = totp_generator (stdtime, key, 6);

    // when
    auto actual = generator.generate_token();

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

    sys_time stdtime (test_stdtime);
    // Fake the Key
    std::string key = "\xff\x91\xebO\x04\xa4\xda$\xd2$a\x95Vs\xaf`";
    auto generator = totp_generator (stdtime, key, 6);

    // when
    auto actual = generator.generate_token();

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
    /// TODO: int -> time_t
    std::initializer_list<time_t> samples { theTime }; //
    auto test_stdtime = std::make_shared<fake_time> (samples);

    sys_time stdtime (test_stdtime);
    // Fake the Key
    std::string key = "\xff\x91\xebO\x04\xa4\xda$\xd2$a\x95Vs\xaf`";
    auto generator = totp_generator (stdtime, key, 6);
    std::string expected = "258675";

    // when
    auto actual = generator.generate_token();

    // then
    check (actual.size() == 6, "size is wrong");
    check (actual == expected,
           "precomputed value failed to match"); // TODO: Does == work for std::string like I want it to?
    succeed();
}

int run_tests()
{
    test (six_digits);
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

