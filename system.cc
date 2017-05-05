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

#include "system.h"
#include "sys_stdlib.h"
#include "sys_time.h"

system::system (const stdlib &stdlib, const sys_time &time)
{
    unsigned seed = static_cast<unsigned> (time.time (nullptr));
    stdlib.srand (seed);
    /* this makes it look like we're more random when the tool
     * is invoked. it isn't necessary and we shouldn't be using
     * rand() at all in the final version since it's kind of
     * dumb.
     */
    stdlib.rand();
}

