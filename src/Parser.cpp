#include "Parser.h"

#include <exception>
#include <source_location>
#include <stdexcept>

#define A(expr)  \
    if (!(expr)) \
    std::throw_with_nested(std::runtime_error(__FILE__ + std::string(" Line ") + std::to_string(__LINE__) + "\n> in function: " + __PRETTY_FUNCTION__ + "\n> ASSERTION FAILED: " + #expr + " wasn't met"))

#define A_(expr, msg) \
    if (!(expr))      \
    std::throw_with_nested(std::runtime_error(__FILE__ + std::string(" Line ") + std::to_string(__LINE__) + "\n> in function: " + __PRETTY_FUNCTION__ + "\n> " + msg))

std::shared_ptr<Literal> parse_literal(Token lit)
{
    if (lit.m_kind == TKind::Bool) {
        return std::make_shared<BooleanLiteral>(lit.m_content.b);
    } else if (lit.m_kind == TKind::Int) {
        return std::make_shared<IntegerLiteral>(lit.m_content.i);
    }
    A_(false, "Literal type unknown: " + lit.to_string());
}

std::shared_ptr<Expression> parse_expression(std::span<Token>& tokens)
{
    A(!tokens.empty());
    if (!is_intrinsic(tokens.front().m_kind)) {
        auto obj = tokens.front();
        tokens = tokens.subspan(1);
        if (obj.m_kind == TKind::Symbol)
            return std::make_shared<Variable>(obj.m_content.symbol);
        if (obj.m_kind > TKind::__LENGTH_KEYWORDS__)
            return parse_literal(obj);
        A_(false, "parsing non-intrinsic of following type not implemented: " + tokens.front().to_string());
    }
    A_(tokens.front().m_kind == TKind::Gt, "Not > " + tokens.front().to_string());
    auto op = Operator(OperatorKind::Greater);
    tokens = tokens.subspan(1);
    auto lhs = parse_expression(tokens);
    auto rhs = parse_expression(tokens);

    return std::make_shared<BinaryOp>(op, lhs, rhs);
}

std::shared_ptr<AbstractLanguageItem> parse_fn(std::span<Token>& tokens)
{
    A(!tokens.empty());
    A(tokens.front().m_kind == TKind::Fn);
    tokens = tokens.subspan(1);
    A(tokens.front().m_kind == TKind::Symbol);
    auto const name = tokens.front().m_content.symbol;
    tokens = tokens.subspan(1);
    A(tokens.front().m_kind == TKind::in);
    tokens = tokens.subspan(1);
    auto inputs = std::vector<std::string>();
    while (tokens.front().m_kind == TKind::Symbol) {
        inputs.push_back(tokens.front().m_content.symbol);
        tokens = tokens.subspan(1);
    }
    A(tokens.front().m_kind == TKind::out);
    tokens = tokens.subspan(1);
    auto outputs = std::vector<std::string>();
    while (tokens.front().m_kind == TKind::Symbol) {
        outputs.push_back(tokens.front().m_content.symbol);
        tokens = tokens.subspan(1);
    }
    A(tokens.front().m_kind == TKind::guard);
    tokens = tokens.subspan(1);
    auto guards = std::vector<std::shared_ptr<Expression>>();
    while (true) {
        if (tokens.empty())
            abort();
        if (tokens.front().m_kind == TKind::body)
            break;
        guards.push_back(parse_expression(tokens));
    }
    A(tokens.front().m_kind == TKind::body);
    tokens = tokens.subspan(1);
    auto body = std::vector<std::shared_ptr<Expression>>();
    while (true) {
        A(!tokens.empty());
        if (tokens.front().m_kind == TKind::End) {
            tokens = tokens.subspan(1);
            break;
        }
        body.push_back(parse_expression(tokens));
    }
    return std::make_shared<Function>(
        std::move(name),
        std::move(inputs),
        std::move(outputs),
        std::move(guards),
        std::move(body));
}

std::shared_ptr<AbstractLanguageItem> parse_next(std::span<Token>& tokens)
{
    A(!tokens.empty());
    auto const first_kind = tokens.begin()->m_kind;
    if (first_kind == TKind::Fn) {
        return parse_fn(tokens);
    }
    std::cout << "Token kind " << tkind_to_str[first_kind] << " not implemented" << std::endl;
    abort();
}

[[nodiscard]] std::vector<std::shared_ptr<AbstractLanguageItem>> parse(std::span<Token> tokens)
{
    auto items = std::vector<std::shared_ptr<AbstractLanguageItem>>();
    while (!tokens.empty())
        items.push_back(parse_next(tokens));
    return items;
}
