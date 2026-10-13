#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cassert>

#define PARSER_REGEX R"((<->|<=>|↔)|(->|→)|(v|∨)|(^|∧)|(¬)|([a-z])|(\()|(\)))"

enum class TokenType
{
    Equivalence = 1,
    Implication,
    Disjunction,
    Conjunction,
    Not,
    Variable,
    BracketBegin,
    BracketEnd,
};

struct Token
{
    std::string value;
    TokenType type;
};

int main(int argc, char** argv)
{
    std::string args;
    for (int i{1}; i < argc; ++i)
    {
        std::cout << argv[i] << '\n';
        args += argv[i];
    }
    std::cout << args << '\n';


    std::vector<Token> tokens;
    static const std::regex regex{PARSER_REGEX};
    for (auto it = std::sregex_iterator(args.begin(), args.end(), regex); it != std::sregex_iterator{}; ++it)
    {
        const auto match = *it;
        for (size_t i{1}; i < match.size(); ++i)
        {
            if (match[i].matched)
            {
                if (match.str().empty()) continue;
                assert(i > 0);
                assert(i <= (size_t)TokenType::BracketEnd);
                tokens.push_back({.value=match.str(), .type=(TokenType)i});
                break;
            }
        }
    }

    for (const auto& token : tokens)
    {
         std::cout << token.value << ": " << (int)token.type << '\n';
    }

    return 0;
};
