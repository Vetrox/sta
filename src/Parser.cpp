#include "Parser.h"

std::unique_ptr<Expression> parse_expression(std::span<Token>& tokens)
{
    std::cout << "Parse expression is not implemented. Tokens size: " << tokens.size() << std::endl;
    abort();
}

std::shared_ptr<AbstractLanguageItem> parse_fn(std::span<Token>& tokens)
{
    if (tokens.empty())
        abort();
    if (tokens.front().m_kind != TKind::Fn)
        abort();
    tokens = tokens.subspan(1);
    if (tokens.front().m_kind != TKind::Symbol)
        abort();
    auto const name = tokens.front().m_content.symbol;
    tokens = tokens.subspan(1);
    if (tokens.front().m_kind != TKind::in)
        abort();
    tokens = tokens.subspan(1);
    auto inputs = std::vector<std::string>();
    while (tokens.front().m_kind == TKind::Symbol) {
        inputs.push_back(tokens.front().m_content.symbol);
        tokens = tokens.subspan(1);
    }
    if (tokens.front().m_kind != TKind::out)
        abort();
    tokens = tokens.subspan(1);
    auto outputs = std::vector<std::string>();
    while (tokens.front().m_kind == TKind::Symbol) {
        outputs.push_back(tokens.front().m_content.symbol);
        tokens = tokens.subspan(1);
    }
    if (tokens.front().m_kind != TKind::guard)
        abort();
    tokens = tokens.subspan(1);
    auto guards = std::vector<Expression>();
    while (true) {
        if (tokens.empty())
            abort();
        if (tokens.front().m_kind == TKind::body)
            break;
        guards.push_back(parse_expression(span));
    }
    if (tokens.front().m_kind != TKind::body)
        abort();
    tokens = tokens.subspan(1);
    auto body = std::vector<Expression>();
    while (true) {
        if (tokens.empty())
            abort();
        if (tokens.front().m_kind == TKind::End)
            break;
        body.push_back(parse_expression(span));
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
    if (tokens.empty())
        abort();
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
