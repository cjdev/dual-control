#include <memory>
#include <ctime>

#include "sys_time.h"

namespace {
    class impl : public time_ifc {
        public:
            time_t time(time_t *timer) const override{
                return ::time(timer);
            }
    };
}

const time_t &time::get() {
    static time singleton(std::make_shared<impl>());
    return singleton;
}
