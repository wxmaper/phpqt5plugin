#include "astnode.h"
#include "token.h"

namespace PHPQt5 {
namespace Internal {

ASTNode::ASTNode(ASTNode *parent, ASTNode::Type type)
    : _parent(parent), _type(type)
{
    if (parent)
        parent->addNode(this);
}

ASTNode::ASTNode(ASTNode *parent, const Token &token, ASTNode::Type type)
    : _parent(parent), _token(token), _type(type)
{
    if (parent)
        parent->addNode(this);
}

ASTNode::~ASTNode()
{
    while (_nodes.size()) {
        ASTNode *node = _nodes.takeLast();
        delete node;
    }
}

Token ASTNode::token() const
{
    return _token;
}

void ASTNode::setToken(const Token &token)
{
    _token = token;
}

ASTNode *ASTNode::parent() const
{
    return _parent;
}

void ASTNode::setParent(ASTNode *parent)
{
    _parent = parent;
}

void ASTNode::addNode(ASTNode *node)
{
    _nodes.append(node);
}

NodeList ASTNode::nodes() const
{
    return _nodes;
}

void ASTNode::setArgs(const QString &args)
{
    _args = args;
}

}
}
