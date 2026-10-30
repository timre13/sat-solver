#include "Parser.h"

#include <string>
#include <regex>
#include <cassert>

#define PARSER_REGEX R"((<->|<=>|↔)|(->|→)|(v|∨)|(\^|∧)|(¬)|([a-z])|(\()|(\)))"

namespace Parser
{

Token::tokenVec_t parse(const std::string& formula)
{
    Token::tokenVec_t tokens;
    static const std::regex regex{PARSER_REGEX};
    for (auto it = std::sregex_iterator(formula.begin(), formula.end(), regex);
            it != std::sregex_iterator{}; ++it)
    {
        const auto match = *it;
        for (size_t i{1}; i < match.size(); ++i)
        {
            if (match[i].matched)
            {
                if (match.str().empty()) continue;
                assert(i > 0);
                assert(i <= (size_t)TokenType::BracketEnd);
                tokens.push_back(std::make_shared<Token>(match.str(), (TokenType)i));
                break;
            }
        }
    }
    return tokens;
}

} // namespace Parser
