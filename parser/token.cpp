#include "token.h"

#include <QString>
#include "scanner.h"
#include "sourcecodestream.h"

namespace PHPQt5 {
namespace Internal {

Token Token::getIdentifierToken(const SourceCodeStream &stream)
{
    const QString identifier = stream.value();
    if (C_PHP_TYPES.contains(identifier))
        return Token(stream.anchor(), stream.length(), C_PHP_TYPES.value(identifier), identifier);

    if (C_PHP_KEYWORDS.contains(identifier))
        return Token(stream.anchor(), stream.length(), C_PHP_KEYWORDS.value(identifier), identifier);

    if (C_PHP_CONSTROLS.contains(identifier))
        return Token(stream.anchor(), stream.length(), C_PHP_CONSTROLS.value(identifier), identifier);

    return Token(stream.anchor(), stream.length(), Token::T_IDENTIFIER, identifier);
}

Token Token::getOperatorToken(const SourceCodeStream &stream)
{
    static const QString OPERATORS = "|~@!<>()+-=*/%.,;:[]&{}?^";
    const int pos = stream.anchor();
    const QChar op = stream.peek();

    if (OPERATORS.contains(op)) {
        if (op == '=') {
            if (stream.peek(1) == '=') {
                if (stream.peek(2) == '=')
                    return Token(pos, 3, T_IDENTICAL, QLatin1Literal("==="));
                else
                    return Token(pos, 2, T_EQUALS, QLatin1Literal("=="));
            }
            else if (stream.peek(1) == '>')
                return Token(pos, 2, T_DOUBLEARROW, QLatin1Literal("=>"));
            return Token(pos, 1, T_ASSIGN, QLatin1Literal("="));
        }

        if (op == '>') {
            if (stream.peek(1) == '>')
                return Token(pos, 2, T_BITWISE_SHIFTRIGHT, QLatin1Literal(">>"));
            else if (stream.peek(1) == '=')
                return Token(pos, 2, T_GREATEREQUAL, QLatin1Literal(">="));
            return Token(pos, 1, T_GREATER, QLatin1Literal(">"));
        }

        if (op == '<') {
            if (stream.peek(1) == '<')
                return Token(pos, 2, T_BITWISE_SHIFTLEFT, QLatin1Literal("<<"));
            else if (stream.peek(1) == '=')
                return Token(pos, 2, T_LESSEQUAL, QLatin1Literal("<="));
            else if (stream.peek(1) == '?' && stream.peek(2) == 'p' && stream.peek(3) == 'h' && stream.peek(4) == 'p')
                return Token(pos, 5, T_OPEN_TAG, QLatin1Literal("<?php"));
            return Token(pos, 1, T_LESS, QLatin1Literal("<"));
        }

        if (op == '!') {
            if (stream.peek(1) == '=') {
                if (stream.peek(2) == '=')
                    return Token(pos, 3, T_NOTIDENTICAL, QLatin1Literal("!=="));
                else
                    return Token(pos, 2, T_NOTEQUALS, QLatin1Literal("!="));
            }
            return Token(pos, 1, T_NOT, QLatin1Literal("!"));
        }

        if (op == '-') {
            if (stream.peek(1) == '=')
                return Token(pos, 2, T_SUBASSIGN, QLatin1Literal("-="));
            else if (stream.peek(1) == '>')
                return Token(pos, 2, T_ARROW, QLatin1Literal("->"));
            else if (stream.peek(1) == '-')
                return Token(pos, 2, T_DECR, QLatin1Literal("--"));
            return Token(pos, 1, T_SUB, QLatin1Literal("-"));
        }

        if (op == '+') {
            if (stream.peek(1) == '=')
                return Token(pos, 2, T_ADDASSIGN, QLatin1Literal("+="));
            else if (stream.peek(1) == '+')
                return Token(pos, 2, T_INCR, QLatin1Literal("++"));
            return Token(pos, 1, T_ADD, QLatin1Literal("+"));
        }

        if (op == '*') {
            if (stream.peek(1) == '=')
                return Token(pos, 2, T_MULASSIGN, QLatin1Literal("*="));
            return Token(pos, 1, T_MUL, QLatin1Literal("*"));
        }

        if (op == '/') {
            if (stream.peek(1) == '=')
                return Token(pos, 2, T_DIVASSIGN, QLatin1Literal("/="));
            return Token(pos, 1, T_DIV, QLatin1Literal("/"));
        }

        if (op == ':') {
            if (stream.peek() == ':')
                return Token(pos, 2, T_DOUBLECOLON, QLatin1Literal("::"));
            return Token(pos, 1, T_COLON, QLatin1Literal(":"));
        }

        if (op == '.') {
            if (stream.peek(1) == '.') {
                if (stream.peek(2) == '.')
                    return Token(pos, 3, T_EXCLUSIVE_RANGE, QLatin1Literal("..."));
                else
                    return Token(pos, 2, T_INCLUSIVE_RANGE, QLatin1Literal(".."));
            }
            else if (stream.peek(1) == '=')
                return Token(pos, 2, T_CONCATASSIGN, QLatin1Literal(".="));
            return Token(pos, 1, T_CONCAT, QLatin1Literal("."));
        }

        if (op == '&') {
            if (stream.peek(1) == '&')
                return Token(pos, 2, T_AND, QLatin1Literal("&&"));
            return Token(pos, 1, T_BITWISE_AND, QLatin1Literal("&"));
        }

        if (op == '|') {
            if (stream.peek(1) == '|')
                return Token(pos, 2, T_OR, QLatin1Literal("||"));
            return Token(pos, 1, T_BITWISE_OR, QLatin1Literal("|"));
        }

        if (op == '%') {
            if (stream.peek(1) == '=')
                return Token(pos, 2, T_MODASSIGN, QLatin1Literal("%="));
            return Token(pos, 1, T_MOD, QLatin1Literal("%"));
        }

        if (!C_PHP_OPERATORS.contains(op)) return Token(pos, 0, T_NONE, "");
        else return Token(pos, 1, C_PHP_OPERATORS.value(op), op);
    }
    else {
        return Token(pos, 1, T_ERROR, op);
    }
}


}
}
