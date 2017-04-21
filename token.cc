/* Copyright (C) CJ Affiliate
 *
 * You may use, distribute and modify this code under  the
 * terms of the GNU Public License.
 *
 * You should have received a copy of the license with this
 * file. If not, you will find a copy in the "LICENSE" file
 * at https://github.com/cjdev/dual-control.
 */

#include <cstdlib>
#include <cstring>
#include <security/pam_modules.h>
#include <pwd.h>
#include <unistd.h>
#include <cstdio>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "test_support.h"

