#ifndef PARSER_H
#define PARSER_H

#include <QString>


namespace PHPQt5 {
namespace Internal {

class Token;
class AST;
class Parser
{
public:
    Parser(const QString &source);
    AST *createASTNode(const Token &tk);

    AST *ast;
    AST *regionNode;
    AST *currentNode;
};

}
}

#endif // PARSER_H
