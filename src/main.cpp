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

    return 0;
};
