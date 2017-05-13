#include "phphighlighter.h"
#include <QDebug>

#include <texteditor/textdocumentlayout.h>

#include <QTextBlock>
#include <QXmlStreamReader>
#include <QMessageBox>
// #include "phptokeninfo.h"

#include "../parser/scanner.h"
#include "../parser/token.h"

#include <D:/qt-projects/phpqt5/phpqt5classhelpers.h>

namespace PHPQt5 {
namespace Internal {

using namespace TextEditor;

PHPHighlighter::PHPHighlighter(const QMap<QString, QString> &words,
                               const QMap<QString, FontFormat> &fontFormats,
                               const QMap<Format, QString> &formatKeys,
                               const QMap<Format, QTextCharFormat> &formats,
                               const QHash<int, QString> &savedState,
                               DynVarsProcessor *dvp)
    : m_words(words), m_fontFormats(fontFormats), m_formatKeys(formatKeys),
      m_formats(formats), m_savedState(savedState), m_dvp(dvp),
      TextEditor::SyntaxHighlighter()
{

}

PHPHighlighter::~PHPHighlighter()
{
}

struct BitField {
    union {
        int value;
        struct {
            int scannerState:8;
            int braceDepth:8;
            int parentheses:8;
        };
    };
};

void PHPHighlighter::highlightBlock(const QString &text)
{
    BitField previousBlockState_;
    previousBlockState_.value = previousBlockState();
    int scannerState = 0, initialBraceDepth = 0, parenthesesCount = 0;
    if (previousBlockState_.value != -1) {
        scannerState = previousBlockState_.scannerState;
        initialBraceDepth = previousBlockState_.braceDepth;
        parenthesesCount = previousBlockState_.parentheses;
    }

    int braceDepth = initialBraceDepth;

    Scanner s(text, text.length(), Scanner::State(scannerState));
    TokenList tokens = s.tokenize();
    scannerState = s.state();

    if (tokens.isEmpty()) {
        TextDocumentLayout::clearParentheses(currentBlock());
        setCurrentBlockState(previousBlockState_.value);
        return;
    }

    Parentheses parentheses;
    parentheses.reserve(5);

    foreach (const Token &tk, tokens) {
        formatToken(tk, text, s.state());

        if (tk.is(Token::T_BRACKET_OPEN)
                || tk.is(Token::T_PARENTHESES_OPEN)
                || tk.is(Token::T_SBRACKET_OPEN)) {
            const QChar c = text.at(tk.begin());
            parentheses.append(Parenthesis(Parenthesis::Opened, c, tk.begin()));
        }
        else if(tk.is(Token::T_BRACKET_CLOSE)
                || tk.is(Token::T_PARENTHESES_CLOSE)
                || tk.is(Token::T_SBRACKET_CLOSE)) {
            const QChar c = text.at(tk.begin());
            parentheses.append(Parenthesis(Parenthesis::Closed, c, tk.begin()));
        }

        if (tk.is(Token::T_BRACKET_OPEN)) {
            ++braceDepth;
        }
        else if (tk.is(Token::T_BRACKET_CLOSE)) {
            --braceDepth;

            if (braceDepth < 0) {
                QTextCharFormat format = currentBlock().charFormat();
                QFont font = format.font();
                format.setUnderlineColor(QColor(QLatin1String("red")));
                format.setBackground(QBrush(QColor(QLatin1String("#ffcccc"))));
                font.setUnderline(true);
                format.setFont(font);
                setFormat(tk.begin(), tk.length(), format);
            }
        }
        else if (tk.is(Token::T_PARENTHESES_OPEN)) {
            ++parenthesesCount;
        }
        else if (tk.is(Token::T_PARENTHESES_CLOSE)) {
            --parenthesesCount;

            if (parenthesesCount < 0) {
                QTextCharFormat format = currentBlock().charFormat();
                QFont font = format.font();
                format.setUnderlineColor(QColor(QLatin1String("red")));
                format.setBackground(QBrush(QColor(QLatin1String("#ffcccc"))));
                font.setUnderline(true);
                format.setFont(font);
                setFormat(tk.begin(), tk.length(), format);
            }
        }
    }

    BitField newState;
    newState.braceDepth = braceDepth;
    newState.parentheses = parenthesesCount;
    newState.scannerState = scannerState;

    TextDocumentLayout::setBraceDepth(currentBlock(), braceDepth);
    TextDocumentLayout::setParentheses(currentBlock(), parentheses);

    setCurrentBlockState(newState.value);
}

void PHPHighlighter::formatToken(const Token &tk, const QString &text, int state)
{
    switch (tk.type()) {

    case Token::T_TYPE_VAR:
        setFormat(tk.begin(), tk.length(), formatForCategory(Format_Variable));
        break;

    case Token::T_COMMENT:
        setFormat(tk.begin(), tk.length(), formatForCategory(Format_Comment));
        break;

    case Token::T_DCOMMENT: {
        setFormat(tk.begin(), tk.length(), formatForCategory(Format_Doxygen));

        QRegExp annotationRx(QStringLiteral("@[a-zA-Z0-9]*"));
        int index = annotationRx.indexIn(text);
        while (index >= 0) {
            int length = annotationRx.matchedLength();
            setFormat(index, length, formatForCategory(Format_Annotation));
            index = annotationRx.indexIn(text, index + length);
        }

        QRegExp typeRx(QStringLiteral("(@[a-zA-Z0-9]*) [a-zA-Z0-9_]*"));
        index = typeRx.indexIn(text);
        while (index >= 0) {
            int length = typeRx.matchedLength();
            setFormat(index + typeRx.cap(1).length(), length - typeRx.cap(1).length(), formatForCategory(Format_DoxyType));
            index = typeRx.indexIn(text, index + length);
        }

        QRegExp rx(QStringLiteral("\\$[a-zA-Z_\\x7f-\\xff][a-zA-Z0-9_\\x7f-\\xff]*(\\[[a-zA-Z0-9_]*\\])*"));
        index = rx.indexIn(text);
        while (index >= 0) {
            int length = rx.matchedLength();
            setFormat(index, length, formatForCategory(Format_DoxyVar));
            index = rx.indexIn(text, index + length);
        }

    } break;

    case Token::T_CHAR:
        setFormat(tk.begin(), tk.length(), formatForCategory(Format_String));
        break;

    case Token::T_STRING: {
        setFormat(tk.begin(), tk.length(), formatForCategory(Format_String));

        // подсветка переменных в строках
        QList<QRegExp> rxList;
        rxList.append(QRegExp(QStringLiteral("\\$[a-zA-Z_\\x7f-\\xff][a-zA-Z0-9_\\x7f-\\xff]*(\\[[a-zA-Z0-9_]*\\])*")));
        rxList.append(QRegExp(QStringLiteral("\\$\\{[a-zA-Z_\\x7f-\\xff][a-zA-Z0-9_\\x7f-\\xff]*(\\[[a-zA-Z0-9_]*\\])*\\}")));
        rxList.append(QRegExp(QStringLiteral("\\{\\$[a-zA-Z_\\x7f-\\xff][a-zA-Z0-9_\\x7f-\\xff]*(\\[([0-9]*|\";[a-zA-Z_]*\")|'[a-zA-Z_]*'|\\])*(->[a-zA-Z_\\x7f-\\xff][a-zA-Z0-9_\\x7f-\\xff]*(\\[[a-zA-Z0-9_]*\\])*(\\[([0-9]*|\";[a-zA-Z_]*\";)|'[a-zA-Z_]*'|\\])*)*\\}")));

        foreach (const QRegExp &rx, rxList) {
            int index = rx.indexIn(text);
            while (index >= 0) {
                int length = rx.matchedLength();
                setFormat(index, length, formatForCategory(Format_Variable));
                index = rx.indexIn(text, index + length);
            }
        }

        QRegExp rx(QStringLiteral("\\\\\""));
        int index = rx.indexIn(text);
        while (index >= 0) {
            int length = rx.matchedLength();
            setFormat(index, length, formatForCategory(Format_Shielded));
            index = rx.indexIn(text, index + length);
        }

    } break;

    case Token::T_INTEGER:
    case Token::T_DOUBLE:
        setFormat(tk.begin(), tk.length(), formatForCategory(Format_Number));
        break;

    case Token::T_IDENTIFIER:
    default: {
        const QString identifier = tk.value();
        //qDebug() << "T_IDENTIFIER" << tk;
        if (m_words.contains(identifier)) {
            setFormat(tk.begin(), tk.length(), formatForIdentifier(identifier));
        }
        else if (isClassName(identifier)) {
            setFormat(tk.begin(), tk.length(), formatForCategory(Format_PHPQt5Class));
        }
    } break;
    }

    return; // ИСПРАВИТЬ
    if (state == Scanner::State_Error) {
        QTextCharFormat format = currentBlock().charFormat();
        QFont font = format.font();
        format.setUnderlineColor(QColor("red"));
        font.setUnderline(true);
        format.setFont(font);

        setFormat(tk.begin(), tk.length(), format);
    }
}

QTextCharFormat PHPHighlighter::formatForCategory(Format format)
{
    const QString formatName = m_formatKeys.value(format, C_DEFAULT_FORMAT_NAME);

    if (m_fontFormats.contains(formatName))
        return getTextCharFormat(formatName, m_fontFormats.value(formatName));

    return getTextCharFormat(C_DEFAULT_FORMAT_NAME, m_fontFormats.value(C_DEFAULT_FORMAT_NAME));
}

QTextCharFormat PHPHighlighter::formatForIdentifier(const QString &identifier)
{
    const QString formatName = m_words.value(identifier, C_DEFAULT_FORMAT_NAME);

    if (m_fontFormats.contains(formatName))
        return getTextCharFormat(formatName, m_fontFormats.value(formatName));

    return getTextCharFormat(C_DEFAULT_FORMAT_NAME, m_fontFormats.value(C_DEFAULT_FORMAT_NAME));
}

QTextCharFormat PHPHighlighter::getTextCharFormat(const QString &formatName, const FontFormat &fontFormat)
{
    QTextCharFormat f = currentBlock().charFormat();
    QFont font = f.font();

    font.setBold(fontFormat.bold);
    font.setUnderline(fontFormat.underline);
    font.setItalic(fontFormat.italic);
    f.setForeground(fontFormat.foreground);
    f.setBackground(fontFormat.background);
    f.setFont(font);

    return f;
}

bool PHPHighlighter::isKeyword(const QString &s)
{
    return m_keywords.contains(s);
}

bool PHPHighlighter::isConstant(const QString &s)
{
    return m_constants.contains(s);
}

bool PHPHighlighter::isFunction(const QString &s)
{
    return m_functions.contains(s);
}

bool PHPHighlighter::isClass(const QString &s)
{
    return m_classes.contains(s);
}

bool PHPHighlighter::isType(const QString &s)
{
    return m_types.contains(s);
}

bool PHPHighlighter::isClassName(const QString &s)
{
    return PHPQt5::ClassHelpers::isClass(s, 0);
}

} // namespace Internal
} // namespace PHPQt5
