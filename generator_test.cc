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

#include "generator.h"
#include "sys_stdlib.h"
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

int six_digits()
{
    // given
    std::initializer_list<int> samples { 1 };
    auto test_stdlib = std::make_shared<fake_stdlib> (samples);
    stdlib stdlib (test_stdlib);
    generator generator = make_generator (stdlib);

    // when
    auto actual = generator();

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
    std::initializer_list<int> samples { 1, 2, 3 };
    auto test_stdlib = std::make_shared<fake_stdlib> (samples);
    stdlib stdlib (test_stdlib);
    generator generator = make_generator (stdlib);

    // when
    auto actual1 = generator();
    auto actual2 = generator();

    // then
    check (actual1 != actual2, "samples should be different");
    succeed();
}

int int_max()
{
    // given
    std::initializer_list<int> samples { INT_MAX };
    auto test_stdlib = std::make_shared<fake_stdlib> (samples);
    stdlib stdlib (test_stdlib);
    generator generator = make_generator (stdlib);

    // when
    auto actual = generator();

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
    std::initializer_list<int> samples { INT_MIN };
    auto test_stdlib = std::make_shared<fake_stdlib> (samples);
    stdlib stdlib (test_stdlib);
    generator generator = make_generator (stdlib);

    // when
    auto actual = generator();

    // then
    check (actual.size() == 6, "size is wrong");
    check (std::all_of (actual.begin(), actual.end(), [] (char c) {
        return c >= '0' && c <= '9';
    }), "not just digits");
    succeed();
}

int run_tests()
{
    test (six_digits);
    test (modulated_source_modulates_tokens);
    test (int_max);
    test (int_min);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}

