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
#include <ctime>

#include "test_util.h"
#include "system.h"

#include "sys_time.h"
#include "sys_stdlib.h"

class mock_stdlib : public stdlib_ifc
{
public:
    mutable unsigned captured_seed {0};
    void srand (unsigned int seed) const
    {
        captured_seed = seed;
    }
};

class fake_sys_time : public sys_time_ifc
{
private:
    unsigned value_;
public:
    fake_sys_time (unsigned value) : value_ (value) {}
    time_t time (time_t *timer) const override
    {
        return static_cast<time_t> (value_);
    }
};

int initializes_random ()
{
    // given
    unsigned seed (20392);
    auto test_stdlib = std::make_shared<mock_stdlib>();
    stdlib stdlib (test_stdlib);
    auto test_time = std::make_shared<fake_sys_time> (seed);
    sys_time time (test_time);

    // when
    system sys (stdlib, time);

    // then
    check (test_stdlib->captured_seed == seed, "seed not cpatured");
    succeed();
}
int run_tests()
{
    test (initializes_random);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}

