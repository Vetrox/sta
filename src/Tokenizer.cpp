#include "Tokenizer.h"

BigInt get_int(std::string_view& sv) {
    std::string buf;
    if (sv.starts_with("+") || sv.starts_with("-")) {
        buf += sv.at(0);
        sv = sv.substr(1);
    }
    while (sv.size() > 0 && sv.at(0) > '0' && sv.at(0) <= '9') {
        buf += sv.at(0);
        sv = sv.substr(1);
    }
    return BigInt(std::move(buf));
}

Token get_primitive_token(std::string_view& sv) {
    if (sv.starts_with("false")) {
        sv = sv.substr(5);
        return {.m_kind = TKind::Bool, .m_content = {.b = false}};
    } else if (sv.starts_with("true")) {
        sv = sv.substr(4);
        return {.m_kind = TKind::Bool, .m_content = {.b = true}};
    } else if (sv.starts_with("+")
            || sv.starts_with("-")
            || (sv.at(0) >= '0' && sv.at(0) <= '9')) {
        return {.m_kind = TKind::Int, .m_content = {.i = get_int(sv)}};
    }
    std::string buf;
    while (!(sv.starts_with(" ")
            || sv.starts_with("(")
            || sv.starts_with(")")
            || sv.starts_with("!")
            || sv.starts_with(">"))) {
        buf += sv.at(0);
        sv = sv.substr(1);
    }
    return {.m_kind = TKind::Symbol, .m_content = {.symbol = buf}};
}

Token get_token(std::string_view& sv) {
    for (size_t i = 0; i < TKind::__LENGTH_KEYWORDS__; i++) {
        auto const& str = tkind_to_str.at(i);
        if (sv.starts_with(str)) {
            sv = sv.substr(str.size());
            return {.m_kind = static_cast<TKind>(i)
            };
        }
    }
    return get_primitive_token(sv);
}


[[nodiscard]] std::deque<Token> tokenize(std::string_view sv) {
    std::deque<Token> tokens;
    while (true) {
        sv = sv.substr(sv.find_first_not_of(' ')); // FIXME: out of range error here
        if (sv.size() == 0) break;
        tokens.push_back(get_token(sv));
    }
    return tokens;
}