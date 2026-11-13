#include "TreeNode.h"
#include "Token.h"
#include <iostream>

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

void TreeNode::negate()
{
    for (size_t i{}; i < m_children.size(); ++i)
    {
        if (!m_children[i]->getValue()
            || m_children[i]->getValue()->getType() == TokenType::Variable)
        {
            std::cout << "Negating: "
                << (m_children[i]->getValue() ? m_children[i]->getValue()->getValue() : "()") << '\n';
            m_children.insert(m_children.begin()+i,
                std::make_shared<TreeNode>(std::make_shared<Token>(TokenType::Not)));
            ++i;
        }
    }

    for (auto& child : m_children)
    {
        if (!child->getValue()) continue;
        if (child->getValue()->getType() == TokenType::Disjunction)
        {
            child->setValue(std::make_shared<Token>(TokenType::Conjunction));
        }
        else if (child->getValue()->getType() == TokenType::Conjunction)
        {
            child->setValue(std::make_shared<Token>(TokenType::Disjunction));
        }
    }
}

void TreeNode::callRecursively(std::function<void(TreeNode&, int)> func, int level)
{
    for (const auto& child : m_children)
    {
        func(*child, level);
        child->callRecursively(func, level+1);
    }
}

void TreeNode::callRecursively(std::function<void(TreeNode&, int)> func)
{
    func(*this, 0);
    callRecursively(func, 1);
}

std::string TreeNode::str(bool tree/*=false*/, int indent/*=-1*/) const
{
    if (!tree)
        return m_value ? m_value->str() : "";

    std::string out;
    out += repeatStr("│   ", indent) + "├" + "──" + ' '
        + (m_value ? m_value->str() : "()") + '\n';
    for (const auto& c : m_children)
    {
        out += c->str(true, indent+1);
    }
    return out;
}
