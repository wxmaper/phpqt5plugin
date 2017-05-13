#ifndef TOKENINFO_H
#define TOKENINFO_H

#include "token.h"

#include <QString>

namespace PHPQt5 {
namespace Internal {

class TokenInfo
{
public:
    TokenInfo(Token::T token, const QString &value = QString(), int pos = 0, int len = 0)
        : m_token(token), m_value(value), m_pos(pos), m_len(len)
    {}

    Token::T token() {
        return m_token;
    }

    QString value() const { return m_value; }
    int pos() const { return m_pos; }
    int len() const { return m_len; }

private:
    Token::T m_token = Token::T_NONE;
    QString m_value;
    int m_pos;
    int m_len;
};

}
} // namespace PHPQt5

#endif // TOKENINFO_H
