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

#include "token.h"
#include "test_util.h"
#include "user.h"
#include "sys_fstream.h"
#include "generator.h"
#include "base32.h"
#include "typealiases.h"

class fake_user : public user_ifc
{
private:
    std::string home_directory_;
public:
    fake_user() {}
    fake_user (const std::string &home_directory) :
        home_directory_ (home_directory)
    {
    }
    std::string home_directory() const override
    {
        return home_directory_;
    }
};

class mock_write_fstreams: public fstreams_ifc
{
private:
    mutable std::string captured_filename_;
    mutable std::shared_ptr<std::ostringstream> capture_stream_;
public:
    pstream open_fstream (const std::string &file_path) const override
    {
        pstream result = std::make_shared<std::istringstream> ("");
        std::string badinator (1, '\0');
        result->read (&badinator[0], badinator.size());
        return result;
    }
    postream open_ofstream (const std::string &file_path,
                            std::ios_base::openmode mode) const override
    {
        captured_filename_ = file_path;
        capture_stream_ = std::make_shared<std::ostringstream>();
        return capture_stream_;
    }
    std::string captured_filename()
    {
        return captured_filename_;
    }
    std::string captured_written()
    {
        return capture_stream_->str();
    }
};

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

class fake_rand : public random_source_ifc
{
public:
    fake_rand ()
    {}

    std::vector<uint8_t> get_random_bytes (int length) const override
    {
        return {};
    }
};

class fake_rand_with_specified_result : public random_source_ifc
{
private:
    octet_vector expected_bytes_;
public:
    fake_rand_with_specified_result (octet_vector expected_bytes)
        : expected_bytes_ (expected_bytes)
    {}

    std::vector<uint8_t> get_random_bytes (int length) const override
    {
        return expected_bytes_;
    }
};

class fake_totp_generator : public token_generator_ifc
{
private:
    std::string expected_token = "<unspecified>";
    std::string key = "<unset>";
public:
    fake_totp_generator (std::string expected_token = "<unspecified>") :
        expected_token (expected_token)
    {}
    std::string generate_token (const octet_vector &key) const override
    {
        return expected_token;
    }
};

int reads_from_the_right_file ()
{
    //given
    std::string home_directory = "/somedir";
    // hardcoded file name is .dual_control in the user's home directory
    std::string token_file = home_directory + "/.dual_control";
    std::string token ("AAAAAAAAAAAAAAAA");

    fstreams test_streams (fstreams::delegate (new fake_fstreams (token_file,
                           token)));
    totp_generator generator (totp_generator::delegate (new
                              fake_totp_generator (token)));
    random_source fake_rand (random_source::delegate (new class fake_rand));

    //file_reader test_file_reader (file_reader::delegate (new fake_file_reader));
    user test_user (user::delegate (new fake_user (home_directory)));
    tokens supplier (tokens::create (test_streams, generator, fake_rand));

    //when
    std::string actual = supplier.token (test_user);

    //then
    check (actual == token, "token does not match");
    succeed();
}

int returns_empty_string_if_file_open_fail()
{
    //given
    std::string home_directory = "/somedir";
    // hardcoded file name is .dual_control in the user's home directory
    std::string token_file = home_directory + "/.not_dual_control";
    fstreams test_streams (fstreams::delegate (new fake_fstreams (token_file,
                           "654321")));
    totp_generator generator (totp_generator::delegate (new
                              fake_totp_generator ()));
    random_source fake_rand (random_source::delegate (new class fake_rand));

    user test_user (user::delegate (new fake_user (home_directory)));
    tokens supplier (tokens::create (test_streams, generator, fake_rand));

    //when
    std::string actual = supplier.token (test_user);

    //then
    check (actual == "", "should have returned empty string");
    succeed();
}

int returns_empty_string_if_file_too_short()
{
    //given
    std::string home_directory = "/somedir";
    // hardcoded file name is .dual_control in the user's home directory
    std::string token_file = home_directory + "/.dual_control";
    // we want a 40-byte key, so we need a 64-byte base32-encoded file.
    std::string token ("AAAAAAAAAAAAAAA");

    fstreams test_streams (fstreams::delegate (new fake_fstreams (token_file,
                           token)));
    totp_generator generator (totp_generator::delegate (new
                              fake_totp_generator (token)));
    random_source fake_rand (random_source::delegate (new class fake_rand));

    //file_reader test_file_reader (file_reader::delegate (new fake_file_reader));
    user test_user (user::delegate (new fake_user (home_directory)));
    tokens supplier (tokens::create (test_streams, generator, fake_rand));

    //when
    std::string actual = supplier.token (test_user);

    //then
    check (actual == "", "should have returned empty string");
    succeed();
}

int writes_the_key ()
{
    // given
    std::string home_directory ("/somedir");
    user test_user (user::delegate (new fake_user (home_directory)));
    mock_write_fstreams *mockfs (new mock_write_fstreams);
    fstreams test_streams{fstreams::delegate (mockfs)};
    totp_generator generator (totp_generator::delegate (new
                              fake_totp_generator ()));
    std::string token ("token");
    random_source fake_rand (random_source::delegate (new class fake_rand));
    tokens tokens (tokens::create (test_streams, generator, fake_rand));

    //when
    tokens.save (test_user, token);

    // then
    std::ostringstream temp;
    temp << token << std::endl;
    std::string expected_written = temp.str();
    std::string expected_filename (home_directory + "/.dual_control");
    check (mockfs->captured_filename() == expected_filename,
           "filename does not match");
    check (mockfs->captured_written() == expected_written,
           "token does not match");
    succeed();
}

int ensure_key_creates_key_file_if_not_exists ()
{
    // given
    std::string home_directory ("/somedir");
    user test_user (user::delegate (new fake_user (home_directory)));
    mock_write_fstreams *mockfs (new mock_write_fstreams);

    fstreams test_streams{fstreams::delegate (mockfs)};

    totp_generator generator (totp_generator::delegate (new
                              fake_totp_generator ()));

    std::vector<uint8_t> random_bytes {4,2,4, 2,4,  2,4,2, 4,2};
    random_source fake_rand (random_source::delegate (new
                             fake_rand_with_specified_result (random_bytes)));
    tokens tokens (tokens::create (test_streams, generator, fake_rand));

    //when
    std::string actual = tokens.ensure_key (test_user);

    base32 codec;
    octet_vector actual_decoded = codec.decode (actual);
    // then
    std::string expected_filename (home_directory + "/.dual_control");
    check (mockfs->captured_filename() == expected_filename,
           "filename does not match");
    check (random_bytes == actual_decoded,
           "key does not match");
    succeed();
}

int ensure_key_reads_key_file_if_exists ()
{
    //given
    std::string home_directory ("/somedir");
    std::string key_file = home_directory + "/.dual_control";
    std::string key ("AAAAAAAAAAAAAAAA");
    user test_user (user::delegate (new fake_user (home_directory)));
    fstreams test_streams (fstreams::delegate (new fake_fstreams (key_file,
                           key)));
    totp_generator generator (totp_generator::delegate (new
                              fake_totp_generator ()));
    random_source fake_rand (random_source::delegate (new class fake_rand));
    tokens tokens (tokens::create (test_streams, generator, fake_rand));

    //when
    std::string actual = tokens.ensure_key (test_user);
    //then
    check (actual == key, "read key does not match given key");
    succeed();
}

int generate_key_uses_random_source ()
{
    // given
    user test_user (user::delegate (new fake_user ("/nowhere")));
    fstreams test_streams{fstreams::delegate (new fake_fstreams ("<>", "<>"))};
    totp_generator generator (totp_generator::delegate (new
                              fake_totp_generator ()));

    std::vector<uint8_t> random_bytes1 {4,2,4, 2,4,  2,4,2, 4,2};
    random_source fake_rand1 (random_source::delegate (new
                              fake_rand_with_specified_result (random_bytes1)));
    tokens tokens1 (tokens::create (test_streams, generator, fake_rand1));

    std::vector<uint8_t> random_bytes2 {1,2,1, 2,1,  1,2,1, 2,1};
    random_source fake_rand2 (random_source::delegate (new
                              fake_rand_with_specified_result (random_bytes2)));
    tokens tokens2 (tokens::create (test_streams, generator, fake_rand2));

    //when
    std::string first_key = tokens1.generate_key();
    std::string second_key = tokens2.generate_key();

    // then
    check (first_key != second_key,
           "keys generated from differing random data should not match");
    succeed();
}

int run_tests()
{
    test (reads_from_the_right_file);
    test (returns_empty_string_if_file_open_fail);
    test (returns_empty_string_if_file_too_short);
    test (ensure_key_creates_key_file_if_not_exists);
    test (ensure_key_reads_key_file_if_exists);
    test (generate_key_uses_random_source);
    succeed();
}

int main (int argc, char *argv[])
{
    return !run_tests();
}
