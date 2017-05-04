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
#include "token.h"
#include "sys_fstream.h"

namespace {
    installer init() {
        fstreams fstreams(fstreams::create());
        tokens tokens(tokens::create(fstreams));
        pwd pwd(pwd::create());
        unistd unistd(unistd::create());
        directory directory(directory::create(unistd, pwd));
        stdlib stdlib(stdlib::get());
        generator generator{make_generator(stdlib)};
        installer installer(installer::create(tokens, unistd,
                            directory, generator));

        return installer;
    }
}

int main (int argc, char *argv[])
{
    installer tool(init());
    std::string generated_token = tool.install_token();
    std::cout << generated_token << std::endl;
    return 0;
}

