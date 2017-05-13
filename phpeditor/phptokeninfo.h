//#ifndef PHPTOKENINFO_H
//#define PHPTOKENINFO_H

//#include "phptoken.h"
//#include "phpformattoken.h"

//namespace PHPQt5 {
//namespace Internal {

//class FormatToken;
//class TokenInfo {

//public:
//    TokenInfo()
//        : m_token(Token::T_NONE), m_value("")
//    {}

//    TokenInfo(Token::T token, const QString &value, const FormatToken &format = FormatToken())
//        : m_token(token), m_value(value), m_format(format)
//    {}

//    TokenInfo(Token::T token, const QString &value,
//              const Format &format, int position, int length)
//        : m_token(token),
//          m_value(value),
//          m_format(FormatToken(format, position, length))
//    {}

//    Token::T token() const { return m_token; }
//    QString value() const { return m_value; }
//    int len() const { return m_value.length(); }

//    void setFormat(const FormatToken &format) { m_format = format; }
//    FormatToken format() const { return m_format; }

//private:
//    Token::T m_token = Token::T_NONE;
//    QString m_value;
//    FormatToken m_format;
//};

//} // namespace Internal
//} // namespace PHPQt5

//#include <QDebug>
//#include <QMetaEnum>
//QDebug inline operator<< (QDebug d, const PHPQt5::Internal::TokenInfo &t){
//    QMetaEnum metaEnum = QMetaEnum::fromType<PHPQt5::Internal::Token::T>();
//    d.nospace() << (QString("Token::%1('%2') ")
//                    .arg(QLatin1String(metaEnum.valueToKey(t.token())))
//                    .arg(t.value())
//                    .toUtf8().constData());

//    return d;
//}

//#endif // PHPTOKENINFO_H
