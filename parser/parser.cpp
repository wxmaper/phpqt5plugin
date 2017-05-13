#include "ast.h"
#include "parser.h"
#include "scanner.h"

namespace PHPQt5 {
namespace Internal {

Parser::Parser(const QString &source)
{
    Scanner s(source, source.length(), Scanner::State_Default, false);

    Token tk;

    ast = new AST(Q_NULLPTR, AST::Root);
    regionNode = ast;

    while ((tk = s.read()).type() != Token::T_EOF) {
        currentNode = createASTNode(tk);
    }
}

AST *Parser::createASTNode(const Token &tk) {
    switch (tk.type()) {
    case Token::T_TYPE_VAR: // $var
        return new VariableAST(regionNode, tk.value());

    case Token::T_ASSIGN: // =
        if (currentNode->is(AST::Variable)) {
            return new AssignAST(regionNode, currentNode);
        }
        break;

    case Token::T_STRING: // "string"
    case Token::T_CHAR: // 'string'
        if (currentNode->is(AST::Assign)) {
            return new StringAST(currentNode, tk.value());
        }
        break;

    }
}


}
}
