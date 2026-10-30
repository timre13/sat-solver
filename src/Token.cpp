#include "Token.h"

#include <array>

std::string_view tokenTypeToStr(TokenType type)
{
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

