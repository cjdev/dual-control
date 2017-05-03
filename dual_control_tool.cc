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

int main (int argc, char *argv[])
{
    installer tool;
    std::string generated_token = tool.install_token();
    std::cout << generated_token << std::endl;
    return 0;
}

