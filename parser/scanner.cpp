#include "scanner.h"
#include "astnode.h"
#include <QString>
#include <QDebug>

namespace PHPQt5 {
namespace Internal {


Scanner::Scanner(QString source, int length, Scanner::State state, bool noAst)
    : _state(state), _noAst(noAst), _stream(source.constData(), length)
{
    //qDebug() << source;
    if (!noAst) {
        _rootNode = new ASTNode(Q_NULLPTR, ASTNode::Root);
        _node = _rootNode;

        _neatRootNode = new NeatNode(Q_NULLPTR, _rootNode);
        _neatNode = _neatRootNode;
    }
}

Scanner::~Scanner()
{
    if (!_noAst) {
        delete _neatRootNode;
        delete _rootNode;
    }
}

ASTNode *Scanner::newErrorNode(ASTNode *parentNode)
{
    Token tk(_stream.anchor(), 1, Token::T_ERROR, parentNode->token().value());
    qDebug() << "ERRORNODE:" << parentNode;
    saveState(State_Error);

    return new ASTNode(parentNode, tk, ASTNode::Error);
}

void Scanner::reset()
{
    _tokens.clear();

    if (!_noAst) {
        delete _neatRootNode;
        delete _rootNode;

        _rootNode = new ASTNode(Q_NULLPTR, ASTNode::Root);
        _node = _rootNode;

        _neatRootNode = new NeatNode(Q_NULLPTR, _rootNode);
        _neatNode = _neatRootNode;
    }
}

Token Scanner::read()
{
    _rcount++;

    if (_rcount > MAX_READ_COUNT) {
        return Token(_stream.anchor(), _stream.length(), Token::T_ERROR, QStringLiteral(""));
    }

    Token token;

    if (_stream.isEnd()) {
        token = Token(_stream.sourceLength(),
                      0, Token::T_EOF, QStringLiteral(""));
    }
    else {
        switch (_state) {
        case State_String:
            _stream.setAnchor();
            if (matchVariable()) token = readVariable();
            else token = readString();
            break;

        case State_HardString:
            token = readString();
            break;

            //    case State_Heredoc:
            //    case State_HardHeredoc:
            //        token = readHeredoc(m_savedData);
            //        break;

        case State_MultilineComment:
            token = readMultilineComment();
            break;

        case State_MultilineDComment:
            token = readMultilineDComment();
            break;

        default:
            token = onDefaultState();
        }


        if (_saveArgstr) {
            if (token.type() != Token::T_PARENTHESES_OPEN)
                _argstr.append(token.value());
        }

        //if (!_noAst)
        //    qDebug() << Q_FUNC_INFO << token;
    }

    switch (token.type()) {
    case Token::T_WHITESPACE: // nothing
        break;

    default:
        _prevPrevPrevTk = _prevPrevTk;
        _prevPrevTk = _prevTk;
        _prevTk = token;
        break;
    }

    // qDebug() << token;
    return token;
}

TokenList Scanner::tokenize()
{
    TokenList tokens;
    Token tk;

    while ((tk = read()).type() != Token::T_EOF) {
        tokens.append(tk);
    }

    return tokens;
}

int Scanner::position()
{
    return _stream.pos();
}

bool Scanner::isWhitespace()
{
    return (_stream.peek().isSpace() || _stream.peek() == '\t');
}

bool Scanner::isSkipChar()
{
    switch (_stream.peek().toLatin1()) {
    case '\r': case '\n':
        return true;

    default:
        return false;
    }
}

bool Scanner::matchNumber()
{
    return _stream.peek().isNumber();
}

bool Scanner::matchIdentifier() {
    return (_stream.peek().isLetter() || _stream.peek() == '_');
}

bool Scanner::matchVariable()
{
    if (_state == State_String) {
        return (_stream.peek() == '$' &&
                (_stream.peek(1).isLetter() || _stream.peek(1) == '_'
                 || _stream.peek(1) == '$' || _stream.peek(1) == '{'));
    }

    return (_stream.peek() == '$' &&
            (_stream.peek(1).isLetter() || _stream.peek(1) == '_' || _stream.peek(1) == '$'));
}

bool Scanner::matchString()
{
    return (_stream.peek() == '\'' || _stream.peek() == '\"');
}

bool Scanner::matchDCommentStart()
{
    return (_stream.peek() == '/' && _stream.peek(1) == '/' && _stream.peek(2) == '/');
}

bool Scanner::matchCommentStart()
{
    return ((_stream.peek() == '/' && _stream.peek(1) == '/')
            || _stream.peek() == '#');
}

bool Scanner::matchMultilineDCommentStart()
{
    return (_stream.peek() == '/' && _stream.peek(1) == '*' && _stream.peek(2) == '*');
}

bool Scanner::matchMultilineCommentStart()
{
    return (_stream.peek() == '/' && _stream.peek(1) == '*');
}

Token Scanner::onDefaultState()
{
    while (!_stream.isEnd()) {
        if (isSkipChar()) {
            _stream.move();
            continue;
        }

        _stream.setAnchor();

        if (isWhitespace())
            return readWhitespace();
        if (matchNumber())
            return readNumber();
        if (matchVariable())
            return readVariable();
        if (matchString())
            return readString();
        if (matchIdentifier())
            return readIdentifier();
        if (matchDCommentStart())
            return readDComment();
        if (matchCommentStart())
            return readComment();
        if (matchMultilineDCommentStart())
            return readMultilineDComment();
        if (matchMultilineCommentStart())
            return readMultilineComment();

        return readOperator();

        _stream.move();
    }

    return Token(0, 0, Token::T_EOF, QStringLiteral(""));
}

Token Scanner::readWhitespace()
{
    // qDebug() << Q_FUNC_INFO;
    while (_stream.peek().isSpace() || _stream.peek() == '\t') {
        _stream.move();
    }

    return Token(_stream.anchor(), _stream.length(), Token::T_WHITESPACE, _stream.value());
}

Token Scanner::readNumber()
{
    // qDebug() << Q_FUNC_INFO;
    static const QString HEX_CHARS = "abcdef";

    //QChar first = peek();
    QChar ch = _stream.peek();
    bool dot = false;
    bool x = false;

    bool e = false;
    int epos = -1;

    bool eminus = false;
    int eminuspos = -1;

    int chars = 0;
    int lOffset = 0;

    while (!_stream.isEnd()) {
        if (!ch.isNumber()
                && !(ch == '.' && !x && !dot && !e)
                && !(ch == 'x' && /*first == '0' &&*/ !x && chars == 1 && !e)
                && !((ch == 'e' || ch == 'E') && !e && !x)
                && !(ch == '-' && e && !eminus)
                && !(x && HEX_CHARS.contains(ch)))
            break;

        chars++;
        _stream.move();
        ch = _stream.peek();
    }

    if (x && chars == 2)
        lOffset--;
    if (e && epos == chars-1)
        lOffset--;
    if (eminus && eminuspos == chars-1)
        lOffset=-2;

    return Token(_stream.anchor(),
                 _stream.length() + lOffset,
                 dot ? Token::T_DOUBLE : Token::T_INTEGER,
                 _stream.value());
}

Token Scanner::readVariable()
{
    _stream.move();
    QChar ch = _stream.peek();

    int chars = 0;
    bool openBrace = false;
    bool preOpenBrace = false;
    bool closeBrace = false;

    int aOffset = 0;
    int lOffset = 0;

    if (_state == State_String) {
        QChar pch = _stream.peek(-2);

        while (ch.isLetterOrNumber() || ch == '_' || ch == '$'
               || (ch == '{' && !openBrace)
               || (pch == '{' && !preOpenBrace)
               || (ch == '}' && (openBrace || preOpenBrace)) && !closeBrace) {
            if (ch == '{') openBrace = true;
            else if (pch == '{' && ch != '}') {
                preOpenBrace = true;
                aOffset--;
                lOffset++;
                chars+=2;
                _stream.setAnchor(-2); // <--- !!! ПРОВЕРИТЬ !!!
            }
            else if (ch == '}') closeBrace = true;
            else chars++;

            _stream.move();
            ch = _stream.peek();
            pch = QChar::Null;

            if ((openBrace || preOpenBrace) && closeBrace) break;
        }

        if (preOpenBrace && !closeBrace) {
            aOffset++; lOffset--; chars--;
        }
    }
    else {
        while (ch.isLetterOrNumber() || ch == '_' || ch == '$') {
            if (ch != '$') chars++;
            _stream.move();
            ch = _stream.peek();
        }
    }

    if (chars < 1 || (openBrace && !closeBrace)) {
        return Token(_stream.anchor(),
                     _stream.length(),
                     _state == State_String ? Token::T_STRING : Token::T_ERROR,
                     _stream.value());
    }

    _tkVariable = Token(_stream.anchor() + aOffset,
                        _stream.length() + lOffset,
                        Token::T_TYPE_VAR,
                        _stream.value());

    return _tkVariable;
}

Token Scanner::readString()
{
    QChar quoteChar;

    switch (_state) {
    case State_HardString:
        quoteChar = '\'';
        break;

    case State_String:
        quoteChar = '"';
        break;

    default:
        quoteChar = _stream.peek();
        _stream.move();
        saveState(quoteChar == '\'' ? State_HardString : State_String, quoteChar);
    }

    QChar ch = _stream.peek();
    QChar pch = quoteChar;

    while (!_stream.isEnd()) {
        if (ch == quoteChar && pch != '\\') {
            clearState();
            _stream.move();
            break;
        }
        else if (_state == State_String && matchVariable() && pch != '\\') {
            break;
        }

        _stream.move();
        pch = ch;
        ch = _stream.peek();
    }

    return Token(_stream.anchor(),
                 _stream.length(),
                 quoteChar == '\'' ? Token::T_CHAR : Token::T_STRING,
                 _stream.value());
}

Token Scanner::readComment()
{
    QChar ch = _stream.peek();

    while (ch != '\n' && !ch.isNull() && !_stream.isEnd()) {
        _stream.move();
        ch = _stream.peek();
    }

    return Token(_stream.anchor(),
                 _stream.length(),
                 Token::T_COMMENT,
                 _stream.value());
}

Token Scanner::readDComment()
{
    QChar ch = _stream.peek();

    while (ch != '\n' && !ch.isNull() && !_stream.isEnd()) {
        _stream.move();
        ch = _stream.peek();
    }

    return Token(_stream.anchor(),
                 _stream.length(),
                 Token::T_DCOMMENT,
                 _stream.value());
}

Token Scanner::readMultilineComment()
{
    saveState(State_MultilineComment, ' ');

    while (!_stream.isEnd()) {
        if (_stream.peek() == '*' && _stream.peek(1) == '/') {
            _stream.move(2);
            _state = State_Default;
            break;
        }

        _stream.move();
    }

    return Token(_stream.anchor(),
                 _stream.length(),
                 Token::T_COMMENT,
                 _stream.value());
}

Token Scanner::readMultilineDComment()
{
    saveState(State_MultilineDComment, ' ');

    while (!_stream.isEnd()) {
        if (_stream.peek() == '*' && _stream.peek(1) == '/') {
            _stream.move(2);
            _state = State_Default;
            break;
        }

        _stream.move();
    }

    return Token(_stream.anchor(),
                 _stream.length(),
                 Token::T_DCOMMENT,
                 _stream.value());
}

void Scanner::clearState()
{
    _state = State_Default;
}

void Scanner::saveState(Scanner::State state, const QChar &quoteChar)
{
    _state = state;
}









Token Scanner::readIdentifier()
{
    QChar ch = _stream.peek();
    while (ch.isLetterOrNumber() || ch == '_') {
        _stream.move();
        ch = _stream.peek();
    }

    Token token = Token::getIdentifierToken(_stream);

    if (_noAst)
        return token;

    switch (token.type()) {
    case Token::T_CLASS:
        _node = new ASTNode(_node, token, ASTNode::ClassDeclaration);
        break;

    case Token::T_EXTENDS:
        _node = new ASTNode(_node, token, ASTNode::ClassExtension);
        if (_node->parent()->type() == ASTNode::ClassDeclaration) {
            _node->setNameToken(_node->parent()->nameToken());
        }
        else {
            // +T_ERROR
        }
        break;

    case Token::T_FUNCTION:
        _node = new ASTNode(_node, token, ASTNode::FunctionDeclaration);
        break;

    case Token::T_IF:
    case Token::T_ELSEIF:
        _node = new ASTNode(_node, token, ASTNode::IfStatement);
        break;

    case Token::T_PUBLIC:
        _dType |= ASTNode::Public;
        break;

    case Token::T_PRIVATE:
        _dType |= ASTNode::Private;
        break;

    case Token::T_PROTECTED:
        _dType |= ASTNode::Protected;
        break;

    case Token::T_STATIC:
        _dType |= ASTNode::Static;
        break;

    case Token::T_NEW:

        break;
    }

    if (token.type() == Token::T_IDENTIFIER) {
        switch (_node->type()) {
        case ASTNode::ClassDeclaration:
        case ASTNode::FunctionDeclaration:
            if(!_node->nameLock()) {
                _node->setNameToken(token);
                _neatNode = new NeatNode(_neatNode, _node);
                _node->setDTypeFlags(_dType);
                _dType = ASTNode::Unknown;
            }
            break;

        case ASTNode::ClassExtension:
            if(!_node->extLock()) {
                _node->setExtToken(token);
                //_neatNode = new NeatNode(_neatNode, _node);
                //_node->setDTypeFlags(_dType);
                //_dType = ASTNode::Unknown;
            }
            break;

        default:
            if (_prevTk.is(Token::T_NEW)) {
                _varTypes.insert(_assignVariable.value(), token.value());
            }
        }
    }

    return token;
}

Token Scanner::readOperator()
{
    Token token = Token::getOperatorToken(_stream);
    _stream.move(token.length());

    if (_noAst)
        return token;

    // qDebug() << token << "readOperator:" << _node;

    /* <?php */
    if (token.type() == Token::T_OPEN_TAG) {
        _node = new ASTNode(_node, token, ASTNode::PHPDocument);
        _node->setNameToken(Token(token.begin(), token.length(), token.type(), QStringLiteral("PHPDocument")));
        _neatNode = new NeatNode(_neatNode, _node);
    }

    /* { */
    else if (token.type() == Token::T_BRACKET_OPEN) {
        switch (_node->type()) {
        case ASTNode::ClassDeclaration:
            // qDebug() << "case ASTNode::ClassDeclaration:" << _node;
            _node = new ASTNode(_node, token, ASTNode::ClassBody);
            _node->setNameToken(_node->parent()->nameToken());
            _regionStack.append({ ASTNode::ClassBody, currentLinePos() });
            _currentClassName = _node->parent()->nameToken().value();
            break;

        case ASTNode::ClassExtension:
            if (_node->parent()->type() == ASTNode::ClassDeclaration) {
                _node = new ASTNode(_node, token, ASTNode::ClassBody);
                _node->setNameToken(_node->parent()->nameToken());
                _node->setExtToken(_node->parent()->extToken());
                _regionStack.append({ASTNode::ClassDeclaration, currentLinePos()});
                _currentClassName = _node->parent()->extToken().value();
            }
            else {
                // +T_ERROR
            }
            break;

        case ASTNode::FunctionDeclaration:
            _functions.append(_node);
            _node = new ASTNode(_node, token, ASTNode::FunctionBody);
            _node->setNameToken(_node->parent()->nameToken());
            _regionStack.append({ASTNode::FunctionBody, currentLinePos()});
            break;

        case ASTNode::IfStatement:
            _node = new ASTNode(_node, token, ASTNode::IfRegion);
            _regionStack.append({ASTNode::IfRegion, currentLinePos()});
            break;

        default:
            _node = new ASTNode(_node, token, ASTNode::RegionBody);
            _regionStack.append({ASTNode::RegionBody, currentLinePos()});
        }

    }

    /* } */
    else if (token.type() == Token::T_BRACKET_CLOSE) {
        //qDebug() << "T_BRACKET_CLOSE" << _node;
        //qDebug() << "T_BRACKET_CLOSE" << _node->parent();
        switch (_node->type()) {
        case ASTNode::ClassBody:
            if (_node->parent()) {
                if (_node->parent()->type() == ASTNode::ClassExtension) {
                    if (_node->parent()->parent() &&
                            _node->parent()->parent()->parent()) {
                        _node = _node->parent()->parent()->parent();
                        _neatNode = _neatNode->parent();
                    }
                    else _node = newErrorNode(_node);
                }
                else {
                    if (_node->parent()->parent()) {
                        _node = _node->parent()->parent();
                        _neatNode = _neatNode->parent();
                    }
                    else _node = newErrorNode(_node);
                }
            }
            else _node = newErrorNode(_node);

            _currentClassName.clear();
            break;

        case ASTNode::FunctionBody:
            if (_node->parent() && _node->parent()->parent()) {
                _node = _node->parent()->parent();
                _neatNode = _neatNode->parent();
            }
            else _node = newErrorNode(_node);

            break;

        case ASTNode::IfRegion:
            if (_node->parent() && _node->parent()->parent())
                _node = _node->parent()->parent(); // многострочное условие
            else _node = newErrorNode(_node);
            break;

        default:
            if (_node->parent())
                _node = _node->parent();
            else _node = newErrorNode(_node);
        }

        if (_regionStack.size())
            _regionStack.pop();
    }

    /* ( */
    else if (token.type() == Token::T_PARENTHESES_OPEN) {
        switch (_node->type()) {
        case ASTNode::IfStatement:
            _node = new ASTNode(_node, token, ASTNode::IfExpression);
            _regionStack.append({ASTNode::IfExpression, currentLinePos()});
            break;

        case ASTNode::FunctionDeclaration:
            _saveArgstr = true;
            _regionStack.append({ASTNode::FunctionDeclaration, currentLinePos()});
            break;
        }
    }

    /* ) */
    else if (token.type() == Token::T_PARENTHESES_CLOSE) {
        switch (_node->type()) {
        case ASTNode::IfExpression:
            _node = _node->parent();

            if (_regionStack.size())
                _regionStack.pop();

            break;

        case ASTNode::FunctionDeclaration:
            _node->setArgs(_argstr);
            _saveArgstr = false;
            _argstr.clear();

            if (_regionStack.size())
                _regionStack.pop();

            break;

        }
    }

    /* ; */
    else if (token.type() == Token::T_DOTCOMMA) {
        switch (_node->type()) {
        case ASTNode::IfStatement:
            _node = _node->parent(); // однострочное условие
            break;
        }
    }

    /* = */
    else if (token.is(Token::T_ASSIGN)) {
        if (_prevTk.is(Token::T_TYPE_VAR)) {
            _assignVariable = _prevTk;
        }
    }

    //qDebug() << "NODE: " << _node;

    return token;
}


}
}  // namespace PHPQt5

