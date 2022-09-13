#pragma once

#include <array>
#include <deque>
#include "Number/numutils.h"
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
    std::string symbol;
    BigInt i;
    bool b {false};
};

struct Token {
    TKind m_kind;
    TContent m_content;
};


[[nodiscard]] std::deque<Token> tokenize(std::string_view);

