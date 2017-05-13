#ifndef PHPTOKEN_H
#define PHPTOKEN_H

/*
#include <QString>
#include <qobjectdefs.h>
#include <QHash>

namespace PHPQt5 {
namespace Internal {

class TokenInfo;
class Lexer;
class Token
{
    Q_GADGET
    Q_ENUMS(T)

public:
    Token() {}

    static TokenInfo getOperatorToken(const QChar &op, const Lexer *lexer);
    static TokenInfo getOperatorToken(const QChar &op, const Lexer &lexer);
    static TokenInfo getIdentifierToken(const QString &identifier);

    enum T {
        T_INTERNAL,
        T_PUBLIC,
        T_PROTECTED,
        T_STATIC,
        T_PRIVATE,
        T_SCOPED,
        T_COMMA,
        T_REQUIRE,
        T_ENDIF,
        T_ENDWHILE,
        T_ENDFOR,
        T_ENDFOREACH,
        T_ENDSWITCH,
        T_DOUBLEARROW,
        T_QUESTION,
        T_LIKELY,
        T_UNLIKELY,
        T_OR,
        T_AND,
        T_INSTANCEOF,
        T_BITWISE_OR,
        T_BITWISE_XOR,
        T_BITWISE_SHIFTLEFT,
        T_BITWISE_SHIFTRIGHT,
        T_EQUALS,
        T_IDENTICAL,
        T_LESS,
        T_GREATER,
        T_LESSEQUAL,
        T_GREATEREQUAL,
        T_NOTIDENTICAL,
        T_NOTEQUALS,
        T_ADD,
        T_SUB,
        T_CONCAT,
        T_MUL,
        T_DIV,
        T_MOD,
        T_ISSET,
        T_FETCH,
        T_EMPTY,
        T_INCLUSIVE_RANGE,
        T_EXCLUSIVE_RANGE,
        T_TYPEOF,
        T_CLONE,
        T_NEW,
        T_SELF,
        T_NOT,
        T_BITWISE_NOT,
        T_BITWISE_AND,
        T_PARENTHESES_CLOSE,
        T_SBRACKET_OPEN,
        T_ARROW,
        T_NAMESPACE,
        T_IDENTIFIER,
        T_DOTCOMMA,
        T_USE,
        T_AS,
        T_FUNCTION,
        T_PARENTHESES_OPEN,
        T_BRACKET_OPEN,
        T_BRACKET_CLOSE,
        T_INTERFACE,
        T_EXTENDS,
        T_CLASS,
        T_IMPLEMENTS,
        T_ABSTRACT,
        T_FINAL,
        T_COMMENT,
        T_ASSIGN,
        T_CONST,
        T_CONSTANT,
        T_INLINE,
        T_DEPRECATED,
        T_VOID,
        T_NULL,
        T_THIS,
        T_SBRACKET_CLOSE,
        T_TYPE_INTEGER,
        T_TYPE_UINTEGER,
        T_TYPE_LONG,
        T_TYPE_ULONG,
        T_TYPE_CHAR,
        T_TYPE_UCHAR,
        T_TYPE_DOUBLE,
        T_TYPE_BOOL,
        T_TYPE_STRING,
        T_TYPE_ARRAY,
        T_TYPE_VAR,
        T_TYPE_CALLABLE,
        T_TYPE_RESOURCE,
        T_TYPE_OBJECT,
        T_BREAK,
        T_CONTINUE,
        T_IF,
        T_ELSE,
        T_ELSEIF,
        T_SWITCH,
        T_CASE,
        T_COLON,
        T_DEFAULT,
        T_LOOP,
        T_WHILE,
        T_DO,
        T_TRY,
        T_CATCH,
        T_FOR,
        T_FOREACH,
        T_IN,
        T_REVERSE,
        T_LET,
        T_ADDASSIGN,
        T_SUBASSIGN,
        T_MULASSIGN,
        T_DIVASSIGN,
        T_CONCATASSIGN,
        T_MODASSIGN,
        T_STRING,
        T_DOUBLECOLON,
        T_INCR,
        T_DECR,
        T_ECHO,
        T_RETURN,
        T_OPEN_TAG,
        T_UNSET,
        T_THROW,
        T_PLUS,
        T_INTEGER,
        T_ISTRING,
        T_CHAR,
        T_DOUBLE,
        T_TRUE,
        T_FALSE,
        T_CBLOCK,
        T_AT,
        T_SPACE,
        T_DCOMMENT,
        T_IGNORE,
        T_NONE,
        T_EOF,
        T_ERROR
    };

    Q_ENUM(T)
};

static const QHash<QChar,Token::T> C_PHP_OPERATORS = {
    { '(', Token::T_PARENTHESES_OPEN },
    { ')', Token::T_PARENTHESES_CLOSE },
    { '{', Token::T_BRACKET_OPEN },
    { '}', Token::T_BRACKET_CLOSE },
    { '[', Token::T_SBRACKET_OPEN },
    { ']', Token::T_SBRACKET_CLOSE },
    { '@', Token::T_AT },
    { '!', Token::T_NOT },
    { '~', Token::T_BITWISE_NOT },
    { '&', Token::T_BITWISE_AND },
    { '|', Token::T_BITWISE_OR },
    { '^', Token::T_BITWISE_XOR },
    { '=', Token::T_ASSIGN },
    { '>', Token::T_GREATER },
    { '.', Token::T_CONCAT },
    { '+', Token::T_ADD },
    { '-', Token::T_SUB },
    { '*', Token::T_MUL },
    { '/', Token::T_DIV },
    { '%', Token::T_MOD },
    { ':', Token::T_COLON },
    { ';', Token::T_DOTCOMMA },
    { ',', Token::T_COMMA },
    { '?', Token::T_QUESTION },
    { '\n', Token::T_IGNORE },
    { '\000', Token::T_EOF }
};

static const QHash<QString,Token::T> C_PHP_KEYWORDS = {
    { QLatin1Literal("class"), Token::T_CLASS },
    { QLatin1Literal("extends"), Token::T_EXTENDS },
    { QLatin1Literal("abstract"), Token::T_ABSTRACT },
    { QLatin1Literal("catch"), Token::T_CATCH },
    { QLatin1Literal("clone"), Token::T_CLONE },
    { QLatin1Literal("const"), Token::T_CONST },
    // { QLatin1Literal("exception"), Token:: },
    { QLatin1Literal("final"), Token::T_FINAL },
    { QLatin1Literal("function"), Token::T_FUNCTION },
    // { QLatin1Literal("global"), Token::T_GLOBAL },
    { QLatin1Literal("implements"), Token::T_IMPLEMENTS },
    { QLatin1Literal("instanceof"), Token::T_INSTANCEOF },
    { QLatin1Literal("interface"), Token::T_INTERFACE },
    { QLatin1Literal("new"), Token::T_NEW },
    { QLatin1Literal("self"), Token::T_SELF },
    { QLatin1Literal("static"), Token::T_STATIC },
    // { QLatin1Literal("parent"), Token::T_PARENT },
    { QLatin1Literal("private"), Token::T_PRIVATE },
    { QLatin1Literal("protected"), Token::T_PROTECTED },
    { QLatin1Literal("public"), Token::T_PUBLIC },
    { QLatin1Literal("throw"), Token::T_THROW },
    { QLatin1Literal("try"), Token::T_TRY },
    { QLatin1Literal("and"), Token::T_AND },
    { QLatin1Literal("or"), Token::T_OR },
    // { QLatin1Literal("xor"), Token::T_XOR },
    // { QLatin1Literal("var"), Token::T_VAR },
    { QLatin1Literal("namespace"), Token::T_NAMESPACE },
    { QLatin1Literal("use"), Token::T_USE }
};

static const QHash<QString,Token::T> C_PHP_TYPES = {
    { QLatin1Literal("int"), Token::T_TYPE_INTEGER },
    { QLatin1Literal("string"), Token::T_TYPE_STRING },
    { QLatin1Literal("bool"), Token::T_TYPE_BOOL },
    { QLatin1Literal("double"), Token::T_TYPE_DOUBLE },
    { QLatin1Literal("array"), Token::T_TYPE_ARRAY },
    { QLatin1Literal("float"), Token::T_TYPE_DOUBLE },
    { QLatin1Literal("object"), Token::T_TYPE_OBJECT },
    { QLatin1Literal("resource"), Token::T_TYPE_RESOURCE }
};

static const QHash<QString,Token::T> C_PHP_IDENTIFIERS = {

    { QLatin1Literal("typeof"), Token::T_TYPEOF },
    { QLatin1Literal("isset"), Token::T_ISSET },
    { QLatin1Literal("unset"), Token::T_UNSET },
    { QLatin1Literal("internal"), Token::T_INTERNAL },
    { QLatin1Literal("static"), Token::T_STATIC },
    { QLatin1Literal("inline"), Token::T_INLINE },
    { QLatin1Literal("deprecated"), Token::T_DEPRECATED },
    { QLatin1Literal("empty"), Token::T_EMPTY },
    { QLatin1Literal("fetch"), Token::T_FETCH },

    { QLatin1Literal("false"), Token::T_FALSE },
    { QLatin1Literal("true"), Token::T_TRUE },
    { QLatin1Literal("null"), Token::T_NULL },
    { QLatin1Literal("FALSE"), Token::T_FALSE },
    { QLatin1Literal("TRUE"), Token::T_TRUE },
    { QLatin1Literal("NULL"), Token::T_NULL },
};

static const QHash<QString,Token::T> C_PHP_CONSTROLS = {
    { QLatin1Literal("as"), Token::T_AS },
    { QLatin1Literal("case"), Token::T_CASE },
    { QLatin1Literal("default"), Token::T_DEFAULT },
    { QLatin1Literal("if"), Token::T_IF },
    { QLatin1Literal("else"), Token::T_ELSE },
    { QLatin1Literal("elseif"), Token::T_ELSEIF },
    { QLatin1Literal("while"), Token::T_WHILE },
    { QLatin1Literal("do"), Token::T_DO },
    { QLatin1Literal("for"), Token::T_FOR },
    { QLatin1Literal("foreach"), Token::T_FOREACH },
    { QLatin1Literal("break"), Token::T_BREAK },
    { QLatin1Literal("continue"), Token::T_CONTINUE },
    { QLatin1Literal("switch"), Token::T_SWITCH },
    // { QLatin1Literal("declare"), Token::T_DECLARE },
    { QLatin1Literal("return"), Token::T_RETURN },
    { QLatin1Literal("require"), Token::T_REQUIRE },
    // { QLatin1Literal("include"), Token::T_INCLUDE },
    { QLatin1Literal("endif"), Token::T_ENDIF },
    { QLatin1Literal("endwhile"), Token::T_ENDWHILE },
    { QLatin1Literal("endfor"), Token::T_ENDFOR },
    { QLatin1Literal("endforeach"), Token::T_ENDFOREACH },
    { QLatin1Literal("endswitch"), Token::T_ENDSWITCH }
};

} // namespace Internal
} // namespace PHPQt5
*/

#endif // PHPTOKEN_H
