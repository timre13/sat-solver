#pragma once

#include "Token.h"
#include <memory>
#include <vector>
#include <cassert>

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

    TreeNode(const Token::tokenVec_t& tokens)
    {
        loadTokens(tokens.begin(), tokens.end());
    }

    TreeNode(std::shared_ptr<Token> token)
    {
        m_value = token;
    }

    void loadTokens(Token::tokenConstIt_t it, Token::tokenConstIt_t end);

    static Token::tokenConstIt_t findClosingBracket(
            Token::tokenConstIt_t it, Token::tokenConstIt_t end);

    std::string str(int indent=-1) const;
    operator std::string() const { return str(); }
    std::ostream& operator <<(std::ostream& s) { s << str(); return s; }
};
