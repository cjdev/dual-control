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

#include <string>
#include <iostream>

#include "installer.h"
#include "generator.h"
#include "sys_stdlib.h"
#include "user.h"
#include "sys_unistd.h"
#include "sys_pwd.h"
#include "sys_time.h"
#include "token.h"
#include "system.h"
#include "sys_fstream.h"

namespace
{
class system init_system()
{
    stdlib stdlib (stdlib::get());
    sys_time time (sys_time::get());
    class system system (stdlib, time);
    return system;
}
installer init_installer()
{
    fstreams fstreams (fstreams::create());
    pwd pwd (pwd::create());
    unistd unistd (unistd::create());
    directory directory (directory::create (unistd, pwd));
    stdlib stdlib (stdlib::get());
    sys_time time (sys_time::get());
    int code_digits = 6;
    //TODO: have generator take the key after construction
    totp_generator generator = totp_generator (time, "\x00", code_digits);
    tokens tokens (tokens::create (fstreams, generator));
    installer installer (installer::create (tokens, unistd, directory, generator));

    return installer;
}
}

int main (int argc, char *argv[])
{
    class system system (init_system());
    installer tool (init_installer());
    std::string generated_token = tool.install_token();
    std::cout << generated_token << std::endl;
    return 0;
}
