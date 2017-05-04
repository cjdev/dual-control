#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <memory>

#include "sys_time.h"
#include "sys_stdlib.h"

class system {
    public:
        system(const stdlib &stdlib, const sys_time &time);
};


#endif
