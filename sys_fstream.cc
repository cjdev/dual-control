#include <memory>
#include <fstream>

#include "sys_fstream.h"

namespace {
class impl : public fstreams_ifc {
    public:
        pstream open_fstream(const std::string &file_path) {
            return pstream(new std::ifstream(file_path));
        }
};


}


fstreams fstreams::create()  {
    static fstreams singleton(delegate(new impl));
    return singleton;
}


