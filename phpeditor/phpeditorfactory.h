#ifndef PHPEDITORFACTORY_H
#define PHPEDITORFACTORY_H


#include "dynvarsprocessor.h"

#include <texteditor/texteditor.h>

#include <QPlainTextEdit>

namespace PHPQt5 {
namespace Internal {

using namespace PHPEditor;

struct FontFormat;
enum Format;

class PHPEditorFactory : public TextEditor::TextEditorFactory
{
public:
    PHPEditorFactory();

private:
    void loadSyntaxRules();

    // <word, formatName>
    QMap<QString, QString> m_words;

    // <Format, formatName>
    QMap<QString, QStringList> m_functions;

    // <formatName, FontFormat>
    QMap<QString, FontFormat> m_fontFormats;

    // <Format, formatName>
    QMap<Format, QString> m_formatKeys;

    // <Format, QTextCharFormat>
    QMap<Format, QTextCharFormat> m_formats;

    QHash<int,QString> m_savedState;
    //QHash<Format,FontFormat> m_fontFormats;

    DynVarsProcessor m_dvp;
};

} // namespace Internal
} // namespace PHPQt5

#endif // PHPEDITORFACTORY_H
