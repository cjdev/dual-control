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
#include <cstring>
#include <pwd.h>
#include <cstdio>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

#include "sys_fstream.h"
#include "random_source.h"
#include "test_util.h"

class fake_fstreams : public fstreams_ifc
{
private:
    std::string expected_file_path_;
    std::string file_contents_;
public:
    fake_fstreams (const std::string &expected_file_path,
                   const std::string &file_contents)
        : expected_file_path_ (expected_file_path),
          file_contents_ (file_contents) {}
    pstream open_fstream (const std::string &file_path) const override
    {
        if (file_path == expected_file_path_) {
            return fstreams::pstream (new std::istringstream (file_contents_));
        } else {
            return fstreams_ifc::open_fstream (file_path);
        }

    }
};

int reads_from_the_right_file ()
{
    //given
    std::string random_source = random_source_ifc::file_path;
    std::string very_random_bytes { 4, 2 };
    // hardcoded file name is .dual_control in the user's home directory

    fstreams test_streams (fstreams::delegate (new fake_fstreams (random_source, very_random_bytes)));

    //file_reader test_file_reader (file_reader::delegate (new fake_file_reader));
    class random_source supplier (random_source::create (test_streams));

    //when
    std::vector<uint8_t> actual_v = supplier.get_random_bytes (2);
    std::string actual (actual_v.begin(), actual_v.end());

    //then
    check (actual == very_random_bytes, "didn't get the expected bytes");
    succeed();
}

int handles_missing_random_source_correctly ()
{
    //given
    std::string very_random_bytes { 4, 2 };
    // hardcoded file name is .dual_control in the user's home directory

    fstreams test_streams (fstreams::delegate (new fake_fstreams ("/nowhere", very_random_bytes)));

    //file_reader test_file_reader (file_reader::delegate (new fake_file_reader));
    class random_source supplier (random_source::create (test_streams));

    //when
    try {
        supplier.get_random_bytes (2);
        fail("get_random_bytes() should throw if it can't access /dev/urandom");
    } catch (std::exception e) {
        succeed();
    }

    //then
    succeed();
}

int run_tests()
{
    test (reads_from_the_right_file);
    test (handles_missing_random_source_correctly);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}
