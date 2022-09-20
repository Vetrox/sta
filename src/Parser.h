#pragma once

#include <iostream>
#include <memory>
#include <span>
#include <vector>

#include "Tokenizer.h"

class AbstractLanguageItem {
public:
    virtual std::string to_string() const = 0;
};

class Expression : public AbstractLanguageItem {
public:
    virtual std::string to_string() const override = 0;

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
    virtual std::string to_string() const override
    {
        std::string out = "Fn(name: " + m_name + " in: ";
        for (auto const& s : m_inputs)
            out += s + " ";
        out += "out: ";
        for (auto const& s : m_outputs)
            out += s + " ";
        out += "guards: ";
        for (auto const& g : m_guards)
            out += g.get()->to_string() + " ";
        out += "body: ";
        for (auto const& e : m_body)
            out += e.get()->to_string() + " ";
        out += ")";
        return out;
    };

private:
    std::string m_name;
    std::vector<std::string> m_inputs;
    std::vector<std::string> m_outputs;
    std::vector<std::shared_ptr<Expression>> m_guards;
    std::vector<std::shared_ptr<Expression>> m_body;
};

class Call : public Expression {
public:
    virtual std::string to_string() const override {};

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
    Operator(OperatorKind kind)
        : m_kind(kind)
    {
    }
    virtual std::string to_string() const override
    {
        return ">"; // TODO: add more intrinsics
    };

private:
    OperatorKind m_kind;
};

enum PrimitiveType {
    Boolean,
    Integer
};

class Literal : public Expression {
public:
    Literal() = delete;
    virtual std::string to_string() const override = 0;

protected:
    explicit Literal(PrimitiveType type)
        : m_type(type)
    {
    }

private:
    PrimitiveType m_type;
};

class IntegerLiteral final : public Literal {
public:
    IntegerLiteral() = delete;
    explicit IntegerLiteral(BigInt value)
        : Literal(PrimitiveType::Integer)
        , m_value(value)
    {
    }
    virtual std::string to_string() const override
    {
        return "IntLit(" + m_value.as_decimal() + ")";
    }

private:
    BigInt m_value;
};

class BooleanLiteral final : public Literal {
public:
    BooleanLiteral() = delete;
    explicit BooleanLiteral(bool value)
        : Literal(PrimitiveType::Boolean)
        , m_value(value)
    {
    }
    virtual std::string to_string() const override
    {
        return std::string("BoolLit(") + (m_value ? "true" : "false") + ")";
    }

private:
    bool m_value { false };
};

class Variable : public Expression {
public:
    Variable() = delete;
    Variable(std::string name)
        : m_name(name)
    {
    }
    virtual std::string to_string() const override
    {
        return "Var(" + m_name + ")";
    }

private:
    std::string m_name;
};

class BinaryOp : public Expression {
public:
    BinaryOp(Operator op,
        std::shared_ptr<Expression> lhs,
        std::shared_ptr<Expression> rhs)
        : m_op(op)
        , m_lhs(lhs)
        , m_rhs(rhs)
    {
    }
    virtual std::string to_string() const override
    {
        return "Op(kind: " + m_op.to_string() + " lhs: " + m_lhs.get()->to_string() + " rhs: " + m_rhs.get()->to_string() + ")";
    };

private:
    Operator m_op;
    std::shared_ptr<Expression> m_lhs;
    std::shared_ptr<Expression> m_rhs;
};

[[nodiscard]] std::vector<std::shared_ptr<AbstractLanguageItem>> parse(std::span<Token> tokens);
