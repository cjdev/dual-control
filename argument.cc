#include <vector>
#include <string>

#include "argument.h"

std::vector<const std::string> convert_arguments(int nargs, const char ** argv) {
    std::vector<std::string> temp;

    for (int i = 0; i < nargs; ++i) {
        temp.push_back(argv[i]);
    }

    return std::vector<const std::string>(temp.begin(), temp.end());
}
