#include <array>
#include <endian.h>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <regex>
#include <cassert>

#define PARSER_REGEX R"((<->|<=>|↔)|(->|→)|(v|∨)|(\^|∧)|(¬)|([a-z])|(\()|(\)))"

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

static std::string_view tokenTypeToStr(TokenType type)
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

static std::string repeatStr(const std::string& str, int count)
{
    std::string out;
    for (int i{}; i < count; ++i) out += str;
    return out;
}

class Token final
{
private:
    std::string m_value;
    TokenType m_type;

public:
    Token(const std::string& val, TokenType type)
    {
        m_value = val;
        m_type = type;
    }

    std::string getValue() const { return m_value; }
    TokenType getType() const { return m_type; }

    std::string str() const
    {
        return (m_type == TokenType::Variable ? m_value : std::string{tokenTypeToStr(m_type)});
    }
    operator std::string() const { return str(); }
    std::ostream& operator <<(std::ostream& s) { s << str(); return s; }

    using tokenVec_t = std::vector<std::shared_ptr<Token>>;
    using tokenConstIt_t = decltype(std::declval<tokenVec_t>().cbegin());
};

class TreeNode final
{
private:
    std::shared_ptr<Token> m_value;
    std::vector<std::shared_ptr<TreeNode>> m_children;

public:
    TreeNode(Token::tokenConstIt_t begin, Token::tokenConstIt_t end)
    {
        loadTokens(begin, end);
    }

    TreeNode(std::shared_ptr<Token> token)
    {
        m_value = token;
    }

    TreeNode(const Token::tokenVec_t& tokens)
    {
        loadTokens(tokens.begin(), tokens.end());
    }

    static Token::tokenConstIt_t findClosingBracket(Token::tokenConstIt_t it, Token::tokenConstIt_t end)
    {
        assert((*it)->getType() == TokenType::BracketBegin);
        int level{};
        while (it != end)
        {
            const auto& token = *it;
            if (token->getType() == TokenType::BracketBegin)
                ++level;
            if (token->getType() == TokenType::BracketEnd)
                --level;

            if (level == 0)
            {
                return it;
            }
            ++it;
        }
        return end;
    }

    void loadTokens(Token::tokenConstIt_t it, Token::tokenConstIt_t end)
    {
        while (it < end)
        {
            if ((*it)->getType() == TokenType::BracketBegin)
            {
                const auto last = findClosingBracket(it, end);
                m_children.push_back(std::make_shared<TreeNode>(it+1, last));
                it = last;
            }
            else
            {
                m_children.push_back(std::make_shared<TreeNode>(*it));
            }
            ++it;
        }
    }

    std::string str(int indent=-1) const
    {
        std::string out;
        if (m_value)
            out += repeatStr("│   ", indent) + "├" + "──" + ' ' + m_value->str() + '\n';
        for (const auto& c : m_children)
        {
            out += c->str(indent+1);
        }
        return out;
    }
    operator std::string() const { return str(); }
    std::ostream& operator <<(std::ostream& s) { s << str(); return s; }
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

    Token::tokenVec_t tokens;
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
                tokens.push_back(std::make_shared<Token>(match.str(), (TokenType)i));
                break;
            }
        }
    }

    for (const auto& token : tokens)
    {
         std::cout << token->getValue() << ": " << (int)token->getType() << '\n';
    }

    TreeNode tree{tokens};

    std::cout << tree.str() << '\n';

    return 0;
};
