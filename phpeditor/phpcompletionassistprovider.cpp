#include "phpcompletionassistprovider.h"
#include "phpeditorconstants.h"

#include "phpcompletionassist.h"

#include <texteditor/codeassist/assistinterface.h>
#include <coreplugin/id.h>

#include <QMessageBox>

namespace PHPQt5 {
namespace Internal {

PHPCompletionAssistProvider::PHPCompletionAssistProvider(const QMap<QString, QString> *words,
                                                         const QMap<QString, QStringList> *functions,
                                                         DynVarsProcessor *dvp)
    : m_words(words), m_functions(functions), m_dvp(dvp)
{

}

PHPCompletionAssistProvider::~PHPCompletionAssistProvider()
{

}

bool PHPCompletionAssistProvider::supportsEditor(Core::Id editorId) const
{
    return editorId == Constants::C_PHPEDITOR_ID;
}

TextEditor::IAssistProcessor *PHPCompletionAssistProvider::createProcessor() const
{
    PHPEditor::Keywords keywords = PHPEditor::Keywords(m_words->keys(),
                                                       m_functions->keys(),
                                                       *m_functions,
                                                       m_dvp);

    return new PHPEditor::PHPCompletionAssistProcessor(keywords, m_dvp);
}

bool PHPCompletionAssistProvider::isContinuationChar(const QChar &c) const
{
    //qDebug() << "isContinuationChar" << c;
    return c.isLetterOrNumber() || c == '_';// || c == '$';
}

bool PHPCompletionAssistProvider::isActivationCharSequence(const QString &sequence) const
{
    //qDebug() << "isActivationCharSequence" << sequence;
    const QChar &ch  = sequence.at(2);
    const QChar &ch2 = sequence.at(1);
    const QChar &ch3 = sequence.at(0);

    if (activationSequenceChar(ch, ch2, ch3, Q_NULLPTR, true) != 0)
        return true;

    return false;
}

int PHPCompletionAssistProvider::activationSequenceChar(const QChar &ch,
                                                        const QChar &ch2,
                                                        const QChar &ch3,
                                                        Token::T *kind,
                                                        bool wantFunctionCall)
{
    int referencePosition = 0;
    Token::T completionKind = Token::T_IGNORE;

    switch (ch.toLatin1()) {
    case ',':
        completionKind = Token::T_COMMA;
        referencePosition = 1;
        break;

    case '(':
        if (wantFunctionCall) {
            completionKind = Token::T_PARENTHESES_OPEN;
            referencePosition = 1;
        }
        break;

    case ':':
        if (ch3 != QLatin1Char(':') && ch2 == QLatin1Char(':')) {
            completionKind = Token::T_DOUBLECOLON;
            referencePosition = 2;
        }
        break;

    case '>':
        if (ch2 == QLatin1Char('-')) {
            completionKind = Token::T_ARROW;
            referencePosition = 2;
        }
        break;

    case '$':
        if (ch2.isLetter() || ch2 == '_') {
            completionKind = Token::T_TYPE_VAR;
            referencePosition = 1;
        }
        break;
    }

    if (kind)
        *kind = completionKind;

    return referencePosition;
}

}
}
