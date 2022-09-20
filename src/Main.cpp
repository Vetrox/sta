#include <fstream>
#include <iostream>

#include "Parser.h"
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

    for (auto const& token : tokens) {
        std::cout << token.to_string() << std::endl;
    }

    auto parsed = parse(tokens);

    for (auto const& item : parsed)
        std::cout << item->to_string() << std::endl;

    return 0;
}
