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

    tree.callRecursively([](TreeNode& node, int){
        for (auto child : node.getChildren())
        {
            if (child->getValue() && child->getValue()->getType() == TokenType::Implication)
            {
                std::cout << "Implication\n";
            }
        }
    });

    return 0;
};
