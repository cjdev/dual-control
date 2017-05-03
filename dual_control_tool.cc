#include <string>
#include <iostream>

#include "dual_control_tool.h"

int main(int argc, char *argv[]) {
    dual_control_tool tool;
    std::string generated_token = tool.install_token();
    std::cout << generated_token << std::endl;
    return 0;
}
