#ifndef PHPCOMPLETIONASSISTPROVIDER_H
#define PHPCOMPLETIONASSISTPROVIDER_H

#include <texteditor/codeassist/completionassistprovider.h>

#include <QStringList>
#include "dynvarsprocessor.h"
#include "../parser/token.h"

namespace PHPQt5 {
namespace Internal {

using namespace PHPEditor;

class PHPCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
    Q_OBJECT

public:
    PHPCompletionAssistProvider(const QMap<QString, QString> *words,
                                const QMap<QString, QStringList> *functions,
                                DynVarsProcessor *dvp);
    ~PHPCompletionAssistProvider();

    bool supportsEditor(Core::Id editorId) const;
    TextEditor::IAssistProcessor *createProcessor() const;

    bool isContinuationChar(const QChar &c) const override;
    bool isActivationCharSequence(const QString &sequence) const override;
    static int activationSequenceChar(const QChar &ch, const QChar &ch2, const QChar &ch3,
                                      Token::T *kind, bool wantFunctionCall);

private:
    const QMap<QString, QString> *m_words;// <word, formatName>
    const QMap<QString, QStringList> *m_functions; // <name, arguments>
    DynVarsProcessor *m_dvp;
};

} // namespace Internal
} // namespace PHPQt5


#endif // PHPCOMPLETIONASSISTPROVIDER_H
