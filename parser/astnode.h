#ifndef EXPRAST_H
#define EXPRAST_H

#include <QStack>
#include <qobjectdefs.h>
#include "token.h"

namespace PHPQt5 {
namespace Internal {

class ASTNode;
class NeatNode;
typedef QVector<ASTNode*> NodeList;
typedef QVector<NeatNode*> NeatNodeList;

class NeatNode {
    NeatNode *_parent;
    ASTNode *_node;
    NeatNodeList _nodes;
    bool _dtor;

public:
    NeatNode(NeatNode *parent, ASTNode *node)
        : _parent(parent), _node(node), _dtor(false) {
        if (parent) parent->addNode(this);
    }

    ~NeatNode() {
        _dtor = true;
        while (_nodes.size()) {
            NeatNode *node = _nodes.takeLast();
            delete node;
        }
    }

    void addNode(NeatNode *node) { _nodes.append(node); }
    NeatNode *parent() const { return _parent; }
    ASTNode *node() const { return _node; }
    NeatNodeList nodes() const { return _nodes; }
    bool dtor() const { return _dtor; }
};

class Token;
class ASTNode
{
    Q_GADGET
    Q_ENUMS(Type)

public:
    enum Type {
        None,
        Error,
        Root,
        PHPDocument,
        ClassDeclaration,
        ClassExtension,
        ClassBody,
        FunctionDeclaration,
        FunctionBody,
        RegionBody,
        IfStatement,
        IfExpression,
        IfRegion
    };
    Q_ENUM(Type)

    enum DType {
        Unknown     = 0x0,
        Private     = 0x1,
        Public      = 0x2,
        Protected   = 0x4,
        Static      = 0x8
    };
    Q_DECLARE_FLAGS(DTypes, DType)
    Q_FLAG(DTypes)

    ASTNode(ASTNode *parent = Q_NULLPTR, Type type = None);
    ASTNode(ASTNode *parent, const Token &token, Type type = None);
    ~ASTNode();

    Type type() const { return _type; }
    void setType(Type type) { _type = type; }

    Token token() const;
    void setToken(const Token &token);

    Token nameToken() const { return _nameToken; }
    void setNameToken(Token nameToken) { _nameToken = nameToken; _nlock = true; }

    Token extToken() const { return _extToken; }
    void setExtToken(Token extToken) { _extToken = extToken; _elock = true; }

    ASTNode *parent() const;
    void setParent(ASTNode *parent);

    void addNode(ASTNode *node);
    NodeList nodes() const;

    bool nameLock() const { return _nlock; }
    bool extLock() const { return _elock; }

    // аргументы (только для функций)
    void setArgs(const QString &args);
    QString args() const { return _args; }

    // тип доступа (для функций и переменных)
    void setDTypeFlags(DTypes d) { _dType = d; }
    DTypes dTypeFlags() const { return _dType; }

private:
    Type _type;
    Token _token;
    Token _nameToken;
    Token _extToken;
    ASTNode *_parent = Q_NULLPTR;
    NodeList _nodes;

    bool _nlock = false;
    bool _elock = false;
    QString _args;
    DTypes _dType = Unknown;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ASTNode::DTypes)

}
} // namespace PHPQt5

#include <QDebug>
#include <QMetaEnum>
QDebug inline operator<< (QDebug d, const PHPQt5::Internal::ASTNode *node){
    QMetaEnum metaEnum = QMetaEnum::fromType<PHPQt5::Internal::ASTNode::Type>();

    if (node) {
        d.space() << QString("Node::%1[`%2`::`%3`::`%4`]")
                      .arg(QLatin1String(metaEnum.valueToKey(node->type())))
                      .arg(node->token().value())
                      .arg(node->nameToken().value())
                      .arg(node->extToken().value());
    }
    else {
        d.space() << QString("Node::Q_NULLPTR");
    }

    return d;
}

#endif // EXPRAST_H
