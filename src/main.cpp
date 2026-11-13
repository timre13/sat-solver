#include <iostream>
#include <string>
#include "Parser.h"
#include "Token.h"
#include "TreeNode.h"

int main(int argc, char** argv)
{
    std::string args;
    for (int i{1}; i < argc; ++i)
    {
        std::cout << argv[i] << '\n';
        args += argv[i];
    }
    std::cout << args << '\n';

    Token::tokenVec_t tokens = Parser::parse(args);
    for (const auto& token : tokens)
    {
         std::cout << token->getValue() << ": " << (int)token->getType() << '\n';
    }

    TreeNode tree{tokens};
    std::cout << tree.str() << '\n';
    std::cout << tree.str(true) << '\n';

    auto printFormula{[&tree](){
        int lastLevel=0;
        tree.callRecursively([&lastLevel](TreeNode& node, int level){
            //std::cout << "NODE [" << level << "]: " << node.str() << '\n';
            if (level > lastLevel)
                std::cout << "(";
            else if (level < lastLevel)
                std::cout << ")";
            lastLevel = level;
            std::cout << node.str();
        });
        std::cout << ")\n";
    }};

    auto removeDoubleNot{[&tree](){
        // Remove double negations
        tree.callRecursively([](TreeNode& node, int){
            auto& children = node.getChildren();
            for (size_t i{}; !children.empty() && i < children.size()-1; ++i)
            {
                if (children[i+0]->getValue() && children[i+0]->getValue()->getType() == TokenType::Not
                 && children[i+1]->getValue() && children[i+1]->getValue()->getType() == TokenType::Not)
                {
                    std::cout << "Removing double Not\n";
                    children.erase(children.begin()+i+1);
                    children.erase(children.begin()+i+0);
                }
            }
        });
    }};

    printFormula();
    removeDoubleNot();
    printFormula();

    // Replace Equivalence
    tree.callRecursively([](TreeNode& node, int){
        auto& children = node.getChildren();
        for (size_t i{}; i < children.size(); ++i)
        {
            auto& child = children[i];
            if (child->getValue() && child->getValue()->getType() == TokenType::Equivalence)
            {
                std::cout << "Replacing equivalence\n";
                auto left = children[i-1];
                auto right = children[i+1];
                std::cout << "Left: " << left->getValue()->getValue() << '\n';
                std::cout << "Right: " << right->getValue()->getValue() << '\n';
                child->setValue(std::make_shared<Token>(TokenType::Implication));
                children.insert(children.begin()+i+2, std::make_shared<TreeNode>(std::make_shared<Token>(TokenType::Conjunction)));
                children.insert(children.begin()+i+3, std::make_shared<TreeNode>(std::make_shared<Token>(*right->getValue())));
                children.insert(children.begin()+i+4, std::make_shared<TreeNode>(std::make_shared<Token>(TokenType::Implication)));
                children.insert(children.begin()+i+5, std::make_shared<TreeNode>(std::make_shared<Token>(*left->getValue())));
            }
        }
    });
    printFormula();
    removeDoubleNot();
    printFormula();

    // Replace Implication
    tree.callRecursively([](TreeNode& node, int){
        auto& children = node.getChildren();
        for (size_t i{}; i < children.size(); ++i)
        {
            auto& child = children[i];
            if (child->getValue() && child->getValue()->getType() == TokenType::Implication)
            {
                std::cout << "Replacing implication\n";
                child->setValue(std::make_shared<Token>(TokenType::Disjunction));
                children.insert(children.begin()+i-1, std::make_shared<TreeNode>(std::make_shared<Token>(TokenType::Not)));
            }
        }
    });
    printFormula();
    removeDoubleNot();
    printFormula();

    // Apply De Morgan's Law
    tree.callRecursively([](TreeNode& node, int){
        auto& children = node.getChildren();
        for (size_t i{}; !children.empty() && i < children.size()-1; ++i)
        {
            auto& child = children[i];
            if (child->getValue() && child->getValue()->getType() == TokenType::Not
             && !children[i+1]->getChildren().empty())
            {
                std::cout << "Applying not\n";
                children[i+1]->negate();
                children.erase(children.begin()+i);
            }
        }
    });
    printFormula();
    removeDoubleNot();
    printFormula();

    return 0;
};
