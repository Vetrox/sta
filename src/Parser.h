#pragma once

#include <iostream>
#include <memory>
#include <span>
#include <vector>

#include "Tokenizer.h"

class AbstractLanguageItem {
public:
    virtual std::string to_string() = 0;
};

class Expression : public AbstractLanguageItem {
public:
    virtual std::string to_string() override = 0;

    // virtual xxx eval() = 0;
};

class Function : public AbstractLanguageItem { // TODO: make function an expression
public:
    Function(std::string name,
        std::vector<std::string> inputs,
        std::vector<std::string> outputs,
        std::vector<std::shared_ptr<Expression>> guards,
        std::vector<std::shared_ptr<Expression>> body)
        : m_name(name)
        , m_inputs(inputs)
        , m_outputs(outputs)
        , m_guards(guards)
        , m_body(body)
    {
    }
    virtual std::string to_string() override {};

private:
    std::string m_name;
    std::vector<std::string> m_inputs;
    std::vector<std::string> m_outputs;
    std::vector<std::shared_ptr<Expression>> m_guards;
    std::vector<std::shared_ptr<Expression>> m_body;
};

class Call : public Expression {
public:
    virtual std::string to_string() override {};

private:
    std::shared_ptr<Function> m_fun;
    std::vector<Expression> m_arguments;
};

enum OperatorKind {
    Greater,
    Not
};

class Operator : public AbstractLanguageItem {
public:
    virtual std::string to_string() override {};

private:
    OperatorKind m_kind;
};
/*
enum VariableType {
    Unresolved,
    Boolean,
    Integer
};

class Symbol : Expression {
public:
    virtual std::string to_string() override;
private:
    VariableType m_type {VariableType::Unresolved};
    std::unique_ptr<Function> m_definition_scope;
    std::unique_ptr<Literal> m_value;
};*/

class BinaryOp : public Expression {
public:
    virtual std::string to_string() override {};

private:
    Operator m_op;
    std::unique_ptr<Expression> m_lhs;
    std::unique_ptr<Expression> m_rhs;
};

[[nodiscard]] std::vector<std::shared_ptr<AbstractLanguageItem>> parse(std::span<Token> tokens);
