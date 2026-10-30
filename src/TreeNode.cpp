#include "TreeNode.h"

static std::string repeatStr(const std::string& str, int count)
{
    std::string out;
    for (int i{}; i < count; ++i) out += str;
    return out;
}

Token::tokenConstIt_t TreeNode::findClosingBracket(
    Token::tokenConstIt_t it, Token::tokenConstIt_t end)
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

void TreeNode::loadTokens(Token::tokenConstIt_t it, Token::tokenConstIt_t end)
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

std::string TreeNode::str(int indent/*=-1*/) const
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
