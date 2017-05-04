#include <memory>
#include <ctime>

#include "sys_time.h"

namespace {
    class impl : public sys_time_ifc {
        public:
            time_t time(time_t *timer) const override{
                return ::time(timer);
            }
    };
}

const sys_time &sys_time::get() {
    static sys_time singleton(std::make_shared<impl>());
    return singleton;
}
