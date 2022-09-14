#pragma once

#include <array>
#include <deque>
#include "numutils/numutils.h"
#include <string>
#include <string_view>
#include <unordered_map>


enum TKind {
    BracketOpen = 0,
    BracketClose,
    Fn,
    Pre,
    Post,
    Gt,
    Exclamation,
    __LENGTH_KEYWORDS__,
    Bool,
    Int,
    Symbol,
};

static std::array<std::string, 7> tkind_to_str {
    "(", ")", "fn", "pre", "post", ">", "!"
};

static std::unordered_map<std::string, TKind> str_to_tkind {
    {"(", TKind::BracketOpen},
    {")", TKind::BracketClose},
    {"fn", TKind::Fn},
    {"pre", TKind::Pre},
    {"post", TKind::Post},
    {">", TKind::Gt},
    {"!", TKind::Exclamation}
};


struct TContent {
    std::string symbol {};
    BigInt i {};
    bool b {false};
};

struct Token {
    TKind m_kind {TKind::Symbol};
    TContent m_content {};

    std::string to_string() const {
        if (m_kind < TKind::__LENGTH_KEYWORDS__)
            return "KEY: " + tkind_to_str.at(m_kind);
        if (m_kind == TKind::Bool)
            return "BOO: " + std::string(m_content.b ? "true" : "false");
        if (m_kind == TKind::Int)
            return "INT: " + m_content.i.as_decimal();
        return "SYM: " + m_content.symbol;
    }
};


[[nodiscard]] std::deque<Token> tokenize(std::string_view);

