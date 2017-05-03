#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <string>
#include <functional>

using generator = std::function<std::string()>;

inline generator make_generator() {
    return [] {return std::string("234567");};
}

#endif

