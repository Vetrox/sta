#include <fstream>
#include <iostream>

#include "Tokenizer.h"

int main() {
    std::cout << "Hello from main" << std::endl;

    std::ifstream f("tests/basic.sta");
    std::string program;
    std::string line;
    while (std::getline(f, line)) {
        program += line + " ";
    }
    auto tokens = tokenize(program);


    return 0;
}
