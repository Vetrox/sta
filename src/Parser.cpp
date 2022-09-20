#include "Parser.h"

#include <exception>
#include <source_location>
#include <stdexcept>

#define A(expr)  \
    if (!(expr)) \
    std::throw_with_nested(std::runtime_error(__FILE__ + std::string(" Line ") + std::to_string(__LINE__) + "\n> in function: " + __PRETTY_FUNCTION__ + "\n> ASSERTION FAILED: " + #expr + " wasn't met"))

std::shared_ptr<Expression> parse_expression(std::span<Token>& tokens)
{
    A(false);
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
        if (tokens.front().m_kind == TKind::End)
            break;
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
