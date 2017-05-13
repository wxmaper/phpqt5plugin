#ifndef SCANNER_H
#define SCANNER_H

#include "astnode.h"
#include "token.h"
#include "tokeninfo.h"
#include "sourcecodestream.h"

#include <QChar>
#include <QStack>

#define MAX_READ_COUNT 10000000

namespace PHPQt5 {
namespace Internal {

struct RegionData {
    ASTNode::Type nodeType;
    int startPosition;
};

class Scanner
{
public:
    enum State {
        State_Default,
        State_Error,
        State_String,
        State_HardString,
        State_MultilineDComment,
        State_MultilineComment,
        State_Class,
        State_Function,
        State_If
    };

    enum Region {
        Region_None,
        Region_Class,
        Region_Function,
        Region_If
    };

    struct RegionInfo {
        Token::T regionToken;
        Token token;
    };

    Scanner(QString source, int length, State state = State_Default, bool noAst = true);
    ~Scanner();

    State state() { return _state; }
    QStack<RegionData> regionStack() const { return _regionStack; }
    QList<ASTNode*> functions() const { return _functions; }
    ASTNode* currentNode() const { return _node; }
    NeatNode* currentNeatNode() const { return _neatNode; }
    ASTNode* rootNode() const { return _rootNode; }
    NeatNode* neatNode() const { return _neatRootNode; }

    ASTNode *newErrorNode(ASTNode *parentNode);


    QMap<QString,QString> varTypes() const { return _varTypes; }
    QString getVarType(const QString &var) const {
        return _varTypes.value(var, QString());
    }

    QString getCurrentClassName() const {
        return _currentClassName;
    }



    void reset();
    Token read();
    TokenList tokenize();
    int position();
    int currentLinePos() {
        return _stream.linePos();
    }

    bool isWhitespace();
    bool isSkipChar();
    bool matchNumber();
    bool matchIdentifier();
    bool matchVariable();
    bool matchString();
    bool matchDCommentStart();
    bool matchCommentStart();
    bool matchMultilineDCommentStart();
    bool matchMultilineCommentStart();

private:
    Token onDefaultState();
    Token readWhitespace();
    Token readNumber();
    Token readIdentifier();
    Token readVariable();
    Token readOperator();
    Token readString();
    Token readDComment();
    Token readComment();
    Token readMultilineDComment();
    Token readMultilineComment();

    void clearState();
    void saveState(State state, const QChar &quoteChar = ' ');

    QStack<RegionData> _regionStack;
    SourceCodeStream _stream;
    State _state;
    bool _noAst;

    ASTNode *_node;
    ASTNode *_rootNode;
    NeatNode *_neatNode;
    NeatNode *_neatRootNode;

    Token _tkVariable;
    Token _prevTk;
    Token _prevPrevTk;
    Token _prevPrevPrevTk;
    Token _assignVariable;

    QList<ASTNode*> _functions;
    QString _argstr;
    bool _saveArgstr = false;
    ASTNode::DTypes _dType = ASTNode::Unknown;

    QHash<int, Token> _tokens; // position -> Token
    QMap<QString,QString> _varTypes;

    QString _currentClassName;

    int _rcount = 0;
};

}
} // namespace PHPQt5

#endif // SCANNER_H
