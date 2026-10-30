#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <vector>

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

std::string_view tokenTypeToStr(TokenType type);

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

