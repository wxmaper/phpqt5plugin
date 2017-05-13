#ifndef PHPHIGHLIGHTER_H
#define PHPHIGHLIGHTER_H

#include <texteditor/syntaxhighlighter.h>

#include <QMap>
#include <QObject>
#include <QTextDocument>

#include "dynvarsprocessor.h"
#include "fontsettings.h"
#include "phpformattoken.h"
//#include "phplexer.h"

namespace PHPQt5 {
namespace Internal {

using namespace PHPEditor;

static const char C_DEFAULT_FORMAT_NAME[] = "text";

class Scanner;
class Token;

class PHPHighlighter : public TextEditor::SyntaxHighlighter
{
public:
    //PHPHighlighter();
    PHPHighlighter(const QMap<QString, QString> &words,
                   const QMap<QString, FontFormat> &fontFormats,
                   const QMap<Format, QString> &formatKeys,
                   const QMap<Format, QTextCharFormat> &formats,
                   const QHash<int, QString> &savedState,
                   DynVarsProcessor *dvp);
    ~PHPHighlighter();

private:
    void highlightBlock(const QString &text) override;
    void formatToken(const Token &tk, const QString &text, int state);
    QTextCharFormat formatForCategory(Format format);
    QTextCharFormat formatForIdentifier(const QString &identifier);
    QTextCharFormat getTextCharFormat(const QString &formatName, const FontFormat &fontFormat);

    bool isKeyword(const QString &s);
    bool isConstant(const QString &s);
    bool isFunction(const QString &s);
    bool isClass(const QString &s);
    bool isType(const QString &s);
    bool isClassName(const QString &s);

    QSet<QString> m_keywords;
    QSet<QString> m_constants;
    QSet<QString> m_functions;
    QSet<QString> m_classes;
    QSet<QString> m_types;

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    // QVector<HighlightingRule> m_rules;


    // <word, formatName>
    QMap<QString, QString> m_words;

    // <formatName, FontFormat>
    QMap<QString, FontFormat> m_fontFormats;

    // <Format, formatName>
    QMap<Format, QString> m_formatKeys;

    // <Format, QTextCharFormat>
    QMap<Format, QTextCharFormat> m_formats;

    QHash<int,QString> m_savedState;
    //QHash<Format,FontFormat> m_fontFormats;

    DynVarsProcessor *m_dvp;
};


} // namespace Internal
} // namespace PHPQt5

#endif // PHPHIGHLIGHTER_H
