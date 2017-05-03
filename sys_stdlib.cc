#include <cstdlib>
#include <memory>

#include "sys_stdlib.h"

namespace {
    class impl : public stdlib_ifc {
        public:
            int rand() const override {
                return ::rand();
            }

    };
}

stdlib const &stdlib::get() {
    static stdlib singleton {std::make_shared<impl>()};
    return singleton;
}
