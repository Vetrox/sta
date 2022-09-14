#pragma once

#include <iostream>

class AbstractLanguageItem {
    virtual std::string to_string() = 0;
};

class Expression : AbstractLanguageItem {
    virtual std::string to_string() override = 0;
};

class Lambda : Expression {
    virtual std::string to_string() override;
};

class List : Expression {
    virtual std::string to_string() override;
};

class PreConditions : List { // list-of-function (inputs -> bool)

};

class PostConditions : List { // list-of-function (outputs -> bool)

};
