/*
#include "phptoken.h"
#include "phplexer.h"
#include "phptokeninfo.h"

namespace PHPQt5 {
namespace Internal {

TokenInfo Token::getOperatorToken(const QChar &op, const Lexer *lexer)
{
    return getOperatorToken(op, *lexer);
}

TokenInfo Token::getOperatorToken(const QChar &op, const Lexer &lexer)
{
    static const QString OPERATORS = "|~@!<>()+-=/*%.,;:[]&{}?^";

    if (OPERATORS.contains(op)) {
        if (op == '=') {
            if (lexer.peek() == '=') {
                if (lexer.peek(1) == '=')
                    return TokenInfo(T_IDENTICAL, QLatin1Literal("==="));
                else
                    return TokenInfo(T_EQUALS, QLatin1Literal("=="));
            }
            else if (lexer.peek() == '>')
                return TokenInfo(T_DOUBLEARROW, QLatin1Literal("=>"));
            return TokenInfo(T_ASSIGN, QLatin1Literal("="));
        }

        if (op == '>') {
            if (lexer.peek() == '>')
                return TokenInfo(T_BITWISE_SHIFTRIGHT, QLatin1Literal(">>"));
            else if (lexer.peek() == '=')
                return TokenInfo(T_GREATEREQUAL, QLatin1Literal(">="));
            return TokenInfo(T_GREATER, QLatin1Literal(">"));
        }

        if (op == '<') {
            if (lexer.peek() == '<')
                return TokenInfo(T_BITWISE_SHIFTLEFT, QLatin1Literal("<<"));
            else if (lexer.peek() == '=')
                return TokenInfo(T_LESSEQUAL, QLatin1Literal("<="));
            else if (lexer.peek() == '?' && lexer.peek(1) == 'p' && lexer.peek(2) == 'h' && lexer.peek(3) == 'p')
                return TokenInfo(T_OPEN_TAG, QLatin1Literal("<?php"));
            return TokenInfo(T_LESS, QLatin1Literal("<"));
        }

        if (op == '!') {
            if (lexer.peek() == '=') {
                if (lexer.peek(1) == '=')
                    return TokenInfo(T_NOTIDENTICAL, QLatin1Literal("!=="));
                else
                    return TokenInfo(T_NOTEQUALS, QLatin1Literal("!="));
            }
            return TokenInfo(T_NOT, QLatin1Literal("!"));
        }

        if (op == '-') {
            if (lexer.peek() == '=')
                return TokenInfo(T_SUBASSIGN, QLatin1Literal("-="));
            else if (lexer.peek() == '>')
                return TokenInfo(T_ARROW, QLatin1Literal("->"));
            else if (lexer.peek() == '-')
                return TokenInfo(T_DECR, QLatin1Literal("--"));
            return TokenInfo(T_SUB, QLatin1Literal("-"));
        }

        if (op == '+') {
            if (lexer.peek() == '=')
                return TokenInfo(T_ADDASSIGN, QLatin1Literal("+="));
            else if (lexer.peek() == '+')
                return TokenInfo(T_INCR, QLatin1Literal("++"));
            return TokenInfo(T_ADD, QLatin1Literal("+"));
        }

        if (op == '*') {
            if (lexer.peek() == '=')
                return TokenInfo(T_MULASSIGN, QLatin1Literal("*="));
            return TokenInfo(T_MUL, QLatin1Literal("*"));
        }

        if (op == '/') {
            if (lexer.peek() == '=')
                return TokenInfo(T_DIVASSIGN, QLatin1Literal("/="));
            return TokenInfo(T_DIV, QLatin1Literal("/"));
        }

        if (op == ':') {
            if (lexer.peek() == ':')
                return TokenInfo(T_DOUBLECOLON, QLatin1Literal("::"));
            return TokenInfo(T_COLON, QLatin1Literal(":"));
        }

        if (op == '.') {
            if (lexer.peek() == '.') {
                if (lexer.peek(1) == '.')
                    return TokenInfo(T_EXCLUSIVE_RANGE, QLatin1Literal("..."));
                else
                    return TokenInfo(T_INCLUSIVE_RANGE, QLatin1Literal(".."));
            }
            else if (lexer.peek() == '=')
                return TokenInfo(T_CONCATASSIGN, QLatin1Literal(".="));
            return TokenInfo(T_CONCAT, QLatin1Literal("."));
        }

        if (op == '&') {
            if (lexer.peek() == '&')
                return TokenInfo(T_AND, QLatin1Literal("&&"));
            return TokenInfo(T_BITWISE_AND, QLatin1Literal("&"));
        }

        if (op == '|') {
            if (lexer.peek() == '|')
                return TokenInfo(T_OR, QLatin1Literal("||"));
            return TokenInfo(T_BITWISE_OR, QLatin1Literal("|"));
        }

        if (op == '%') {
            if (lexer.peek() == '=')
                return TokenInfo(T_MODASSIGN, QLatin1Literal("%="));
            return TokenInfo(T_MOD, QLatin1Literal("%"));
        }

        if (!C_PHP_OPERATORS.contains(op)) return TokenInfo(T_NONE, "");
        else return TokenInfo(C_PHP_OPERATORS.value(op), op);
    }
    else {
        return TokenInfo(T_ERROR, op);
    }
}

TokenInfo Token::getIdentifierToken(const QString &identifier)
{
    if (C_PHP_TYPES.contains(identifier))
        return TokenInfo(C_PHP_TYPES.value(identifier), identifier);

    if (C_PHP_KEYWORDS.contains(identifier))
        return TokenInfo(C_PHP_KEYWORDS.value(identifier), identifier);

    if (C_PHP_CONSTROLS.contains(identifier))
        return TokenInfo(C_PHP_CONSTROLS.value(identifier), identifier);

    return TokenInfo(T_IDENTIFIER, identifier);
}

} // namespace Internal
} // namespace PHPQt5
*/
