/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the GNU Public License.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#include <cstring>
#include <pwd.h>
#include <cstdio>
#include <sys/stat.h>

#include "token.h"
#include "test_util.h"

const char *fake_user = "";
const char *fake_user_token = "";

// all the fake system calls
const char *fake_home_dir = "";
int fake_getpwnam_r (const char *nam, struct passwd *pwd, char *buffer,
                     size_t bufsize, struct passwd **result)
{
    strcpy (buffer, fake_home_dir);
    pwd->pw_dir = buffer;
    int ok = !strcmp (nam, fake_user);
    *result = ok ? pwd : 0;
    return !ok;
}

const char *fake_stat_path = "";
int fake_stat (const char *path, struct stat *stat)
{
    return (strcmp (fake_stat_path, path));
}

const char *fake_fopen_path = "";
const char *fake_fopen_mode = "";
FILE *_fhandle = 0;
FILE *fake_fopen (const char *path, const char *mode)
{
    static FILE handle;
    int path_matches = !strcmp (fake_fopen_path, path);
    int mode_matches = !strcmp (fake_fopen_mode, mode);

    if (path_matches && mode_matches) {
        _fhandle = &handle;
        return &handle;
    } else {
        _fhandle = 0;
        return 0;
    }
}

char *fake_fgets (char *buf, int n, FILE *fp)
{
    if (_fhandle == fp && fp != 0) {
        strncpy (buf, fake_user_token, n - 1);
        return buf;
    } else {
        return 0;
    }
}

int fake_fclose (FILE *fp)
{
    return 0;
}

// STDIO

RESET_VARS_START
fake_user = "msmith";
fake_user_token = "123456";
fake_home_dir = "/home/msmith";
fake_stat_path = "/home/msmith/.dual_control";
fake_fopen_path = fake_stat_path;
fake_fopen_mode = "r";
RESET_VARS_END

int validate_compares_to_user_token()
{

    // given

    // when
    int valid = validate_token ("msmith", "123456");

    // then
    check (valid, "expected result to be valid");

    succeed();

}

int validates_from_the_right_user()
{
    //given

    //when
    int valid = validate_token ("jbalcita", "12346");

    //then
    check (!valid, "expected result to be invalid");
    succeed();
}

int validates_user_specific_token()
{
    //given

    //when
    int valid = validate_token ("msmith", "654321");

    //then
    check (!valid, "expected result to be invalid");
    succeed();
}

int runtests()
{
    test (validate_compares_to_user_token);
    test (validates_from_the_right_user);
    test (validates_user_specific_token);
    succeed();
}

int main (int argc, char **argv)
{
    int rval = !runtests();
    return rval;
}

