#ifndef PHPCOMPLETIONASSIST_H
#define PHPCOMPLETIONASSIST_H

#include "texteditor/codeassist/iassistprocessor.h"
#include "texteditor/codeassist/assistproposalitem.h"
#include "texteditor/codeassist/ifunctionhintproposalmodel.h"
#include "texteditor/snippets/snippetassistcollector.h"
#include "dynvarsprocessor.h"

namespace PHPQt5 {
namespace PHPEditor {


using namespace TextEditor;

struct ClassCompletions {
    QStringList _onProps;
    QStringList _methods;
    QStringList _properties;
    QStringList _constants;
};

class Keywords
{
public:
    Keywords() = default;
    Keywords(const QStringList &variables, const QStringList &functions,
             const QMap<QString, QStringList> &functionArgs,
             const DynVarsProcessor *dvp);
    bool isVariable(const QString &word) const;
    bool isFunction(const QString &word) const;
    bool isDynVar(const QString &word) const;

    QStringList variables() const;
    QStringList functions() const;
    QStringList argsForFunction(const QString &function) const;
    const DynVarsProcessor *dynVarsProcessor();

private:
    QStringList m_variables;
    QStringList m_functions;
    QMap<QString, QStringList> m_functionArgs;
    const DynVarsProcessor *m_dvp;
};

class PHPAssistProposalItem : public AssistProposalItem
{
public:
    PHPAssistProposalItem(bool isFunction);

    bool prematurelyApplies(const QChar &c) const final;
    void applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const final;

private:
    bool m_isFunction;
};

class PHPAssistProposalPropItem : public AssistProposalItem
{
public:
    PHPAssistProposalPropItem();

    void applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const final;
};

class PHPAssistOverrideMethodItem : public AssistProposalItem
{
public:
    PHPAssistOverrideMethodItem();

    void applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const final;
};

class PHPAssistProposalOnSlotItem : public AssistProposalItem
{
public:
    PHPAssistProposalOnSlotItem();

    void applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const final;
};

class PHPAssistProposalConstItem : public AssistProposalItem
{
public:
    PHPAssistProposalConstItem();

    void applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const final;
};

class PHPFunctionHintModel : public IFunctionHintProposalModel
{
public:
    PHPFunctionHintModel(const QStringList &functionSymbols);
    ~PHPFunctionHintModel() final = default;

    void reset() final;
    int size() const final;
    QString text(int index) const final;
    int activeArgument(const QString &prefix) const final;

private:
    QStringList m_functionSymbols;
};

class PHPCompletionAssistProcessor : public IAssistProcessor
{
public:
    PHPCompletionAssistProcessor(Keywords keywords, DynVarsProcessor *dvp);
    ~PHPCompletionAssistProcessor() override = default;

    IAssistProposal *perform(const TextEditor::AssistInterface *interface) override;
    QChar startOfCommentChar() const;

    void setSnippetGroup(const QString &id);

protected:
    void setKeywords (Keywords keywords);

private:
    bool isClassName(const QString &word, QString *path);
    bool acceptsIdleEditor();
    int findStartOfName(int pos = -1);
    bool isInComment() const;
    QList<AssistProposalItemInterface *> generateProposalList(const QStringList &words, const QIcon &icon);
    QList<AssistProposalItemInterface *> generateProposalDynVarList(const QStringList &words, const QIcon &icon);
    QList<AssistProposalItemInterface *> generateProposalVarList(const QStringList &words, const QIcon &icon);
    QList<AssistProposalItemInterface *> generateProposalFnList(const QStringList &words, const QIcon &icon);
    QList<AssistProposalItemInterface *> generateProposalMethodsList(const QStringList &words, const QIcon &icon);
    QList<AssistProposalItemInterface *> generateOverrideMethodsList(const QStringList &words, const QIcon &icon);
    QList<AssistProposalItemInterface *> generateProposalPropsList(const QStringList &words, const QIcon &icon);
    QList<AssistProposalItemInterface *> generateProposalOnSlotsList(const QStringList &words, const QIcon &icon);
    QList<AssistProposalItemInterface *> generateProposalConstList(const QStringList &words, const QIcon &icon);


    QList<AssistProposalItemInterface *> generateProposalListForClass(const QString &className, const QString &filter = QString());
    QList<AssistProposalItemInterface *> generateOverrideListForClass(const QString &className, const QString &filter = QString());

    int m_startPosition = -1;
    TextEditor::SnippetAssistCollector m_snippetCollector;
    QString m_word;
    QMultiMap<QString,QString> m_details;
    QMultiMap<QString,QString> m_pdetails;
    QMultiMap<QString,QStringList> m_odetails;

    QMap<QString,ClassCompletions> m_classCompletions;
    QMap<QString,QStringList> m_classInherits;

    QScopedPointer<const TextEditor::AssistInterface> m_interface;
    QIcon m_variableIcon;
    QIcon m_functionIcon;
    QIcon m_onPropertyIcon;
    QIcon m_constantIcon;
    Keywords m_keywords;

    DynVarsProcessor *m_dvp;
    QList<AssistProposalItemInterface *> getItemsFromClass(const QString &className, const QString &filter);
    QStringList getClassInherits(const QString &className);
};

}
}

#endif // PHPCOMPLETIONASSIST_H
