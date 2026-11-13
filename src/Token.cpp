#include "Token.h"

#include <array>
#include <cassert>

std::string_view tokenTypeToStr(TokenType type)
{
    assert(type != TokenType::Variable);
    static constexpr auto strTable = std::to_array<std::string_view>({
        "<->", // Equivalence
        "->",  // Implication
        "v",   // Disjunction
        "^",   // Conjunction
        "¬",   // Not
        "l",   // Variable
        "(",   // BracketBegin
        ")",   // BracketEnd
    });
    return strTable[(int)type-1];
}

