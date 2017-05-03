#include <string>
#include <iostream>

#include "installer.h"

int main(int argc, char *argv[]) {
    installer tool;
    std::string generated_token = tool.install_token();
    std::cout << generated_token << std::endl;
    return 0;
}
