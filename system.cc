
#include "system.h"
#include "sys_stdlib.h"
#include "sys_time.h"

system::system(const stdlib &stdlib, const sys_time &time) {
    unsigned seed = static_cast<unsigned>(time.time(nullptr));
    stdlib.srand(seed);
}

