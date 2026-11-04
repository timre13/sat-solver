#pragma once

#include "Token.h"
#include <memory>
#include <vector>
#include <cassert>
#include <functional>

class TreeNode final
{
private:
    std::shared_ptr<Token> m_value;
    std::vector<std::shared_ptr<TreeNode>> m_children;

    void callRecursively(std::function<void(TreeNode&, int)> func, int level);

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

    std::shared_ptr<Token> getValue() { return m_value; }
    std::vector<std::shared_ptr<TreeNode>> getChildren() { return m_children; }

    void loadTokens(Token::tokenConstIt_t it, Token::tokenConstIt_t end);

    static Token::tokenConstIt_t findClosingBracket(
            Token::tokenConstIt_t it, Token::tokenConstIt_t end);

    void callRecursively(std::function<void(TreeNode&, int)> func);

    std::string str(bool tree=false, int indent=-1) const;
    operator std::string() const { return str(); }
    std::ostream& operator <<(std::ostream& s) { s << str(); return s; }
};
