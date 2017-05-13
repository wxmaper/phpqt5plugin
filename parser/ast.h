#ifndef AST_H
#define AST_H

#include <QString>



namespace PHPQt5 {
namespace Internal {


class AST
{
public:
    enum ASTType {
        Root,
        Assign,
        Number,
        Variable,
        String,

        Error,
    };

    AST(AST *parent, ASTType type)
        : _parent(parent), _asttype(type)
    {}

    AST* setParent(AST *parent) {
        AST* p = _parent;
        _parent = parent;
        return p;
    }

    AST* parent() const { return _parent; }
    ASTType type() const { return _asttype; }
    bool is(ASTType type) const { return _asttype == type; }

private:
    AST* _parent;
    ASTType _asttype;
};

class VariableAST : public AST {
public:
    QString _name;
    QString _className;
    AST *_value;

    VariableAST(AST *p, const QString &varName) : AST(p, Variable),
        _name(varName)
    {}
};

class AssignAST : public AST {
public:
    AST *_left;
    AST *_right;

    AssignAST(AST *p, AST *left) : AST(p, Assign),
        _left(left)
    {
        _left->setParent(this);
    }
};

class StringAST : public AST {
public:
    QString _value;

    StringAST(AST *p, const QString &value) : AST(p, String),
        _value(value)
    {}
};



}
}

#endif // AST_H
