#include "phpcompletionassist.h"
#include "phpindenter.h"

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QTextBlock>

#include <texteditor/codeassist/assistinterface.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/codeassist/functionhintproposal.h>
#include <texteditor/codeassist/genericproposalmodel.h>
#include <texteditor/completionsettings.h>
#include <texteditor/texteditorsettings.h>
#include <texteditor/texteditor.h>

#include "../phpqt5classhelpers.h"
#include "../parser/scanner.h"

#include <libs/cplusplus/Icons.h>
#include <utils/algorithm.h>

namespace PHPQt5 {
using namespace Internal;
namespace PHPEditor {

using namespace TextEditor;

// --------------------------
// Keywords
// --------------------------
// Note: variables and functions must be sorted
Keywords::Keywords(const QStringList &variables,
                   const QStringList &functions,
                   const QMap<QString, QStringList> &functionArgs,
                   const DynVarsProcessor *dvp)
    : m_variables(variables),
      m_functions(functions),
      m_functionArgs(functionArgs),
      m_dvp(dvp)
{
    Utils::sort(m_variables);
    Utils::sort(m_functions);
}

bool Keywords::isVariable(const QString &word) const
{
    return std::binary_search(m_variables.constBegin(), m_variables.constEnd(), word);
}

bool Keywords::isFunction(const QString &word) const
{
    return std::binary_search(m_functions.constBegin(), m_functions.constEnd(), word);
}

bool Keywords::isDynVar(const QString &word) const
{
    const QStringList list = m_dvp->list();

    return std::binary_search(list.constBegin(),
                              list.constEnd(),
                              word);
}

QStringList Keywords::variables() const
{
    return m_variables;
}

QStringList Keywords::functions() const
{
    return m_functions;
}

QStringList Keywords::argsForFunction(const QString &function) const
{
    return m_functionArgs.value(function);
}

const DynVarsProcessor *Keywords::dynVarsProcessor()
{
    return m_dvp;
}


// --------------------------
// KeywordsAssistProposalItem
// --------------------------
PHPAssistProposalItem::PHPAssistProposalItem(bool isFunction)
    : m_isFunction(isFunction)
{
}

bool PHPAssistProposalItem::prematurelyApplies(const QChar &c) const
{
    // only '(' in case of a function
    return c == QLatin1Char('(') && m_isFunction;
}

void PHPAssistProposalItem::applyContextualContent(TextDocumentManipulatorInterface &manipulator,
                                                   int basePosition) const
{
    const CompletionSettings &settings = TextEditorSettings::completionSettings();

    int replaceLength = manipulator.currentPosition() - basePosition;
    QString toInsert = text();
    int cursorOffset = 0;
    const QChar characterAtCurrentPosition = manipulator.characterAt(manipulator.currentPosition());
    bool setAutoCompleteSkipPosition = false;

    if (m_isFunction && settings.m_autoInsertBrackets) {
        if (settings.m_spaceAfterFunctionName) {
            if (manipulator.textAt(manipulator.currentPosition(), 2) == QLatin1String(" (")) {
                cursorOffset = 2;
            } else if ( characterAtCurrentPosition == QLatin1Char('(')
                        || characterAtCurrentPosition == QLatin1Char(' ')) {
                replaceLength += 1;
                toInsert += QLatin1String(" (");
            } else {
                toInsert += QLatin1String(" ()");
                cursorOffset = -1;
                setAutoCompleteSkipPosition = true;
            }
        } else {
            if (characterAtCurrentPosition == QLatin1Char('(')) {
                cursorOffset = 1;
            } else {
                toInsert += QLatin1String("()");
                cursorOffset = -1;
                setAutoCompleteSkipPosition = true;
            }
        }
    }

    manipulator.replace(basePosition, replaceLength, toInsert);
    if (cursorOffset)
        manipulator.setCursorPosition(manipulator.currentPosition() + cursorOffset);
    if (setAutoCompleteSkipPosition)
        manipulator.setAutoCompleteSkipPosition(manipulator.currentPosition());
}

// -------------------------
// KeywordsFunctionHintModel
// -------------------------
PHPFunctionHintModel::PHPFunctionHintModel(const QStringList &functionSymbols)
    : m_functionSymbols(functionSymbols)
{}

void PHPFunctionHintModel::reset()
{}

int PHPFunctionHintModel::size() const
{
    return m_functionSymbols.size();
}

QString PHPFunctionHintModel::text(int index) const
{
    return m_functionSymbols.at(index);
}

int PHPFunctionHintModel::activeArgument(const QString &prefix) const
{
    Q_UNUSED(prefix);
    return 1;
}

// ---------------------------------
// KeywordsCompletionAssistProcessor
// ---------------------------------
PHPCompletionAssistProcessor::PHPCompletionAssistProcessor(Keywords keywords, DynVarsProcessor *dvp)
    : m_snippetCollector(QString(), QIcon(":/texteditor/images/snippet.png")),
      // m_variableIcon(QLatin1String(":/codemodel/images/keyword.png")),
      // m_functionIcon(QLatin1String(":/codemodel/images/member.png")),
      m_keywords(keywords),
      m_dvp(dvp)
{
    m_variableIcon = CPlusPlus::Icons::iconForType(CPlusPlus::Icons::VarPublicIconType);
    m_functionIcon = CPlusPlus::Icons::iconForType(CPlusPlus::Icons::FuncPublicIconType);
    m_onPropertyIcon = CPlusPlus::Icons::iconForType(CPlusPlus::Icons::SlotPublicIconType);
    m_constantIcon = CPlusPlus::Icons::iconForType(CPlusPlus::Icons::EnumIconType);
}

IAssistProposal *PHPCompletionAssistProcessor::perform(const TextEditor::AssistInterface *interface)
{
    m_interface.reset(interface);

    if (isInComment())
        return nullptr;

    if (interface->reason() == IdleEditor && !acceptsIdleEditor())
        return nullptr;

    if (m_startPosition == -1)
        m_startPosition = findStartOfName();

    int nextCharPos = m_startPosition + m_word.length();
    if (m_keywords.isFunction(m_word)
            && m_interface->characterAt(nextCharPos) == QLatin1Char('(')) {
        QStringList functionSymbols = m_keywords.argsForFunction(m_word);
        IFunctionHintProposalModel *model = new PHPFunctionHintModel(functionSymbols);
        return new FunctionHintProposal(m_startPosition, model);
    }

    else if (m_word.length() >= 2
             && m_interface->characterAt(nextCharPos) == QLatin1Char(':')
             && m_interface->characterAt(nextCharPos+1) == QLatin1Char(':')
             && isClassName(m_word, 0)) {
        PHPQt5::Internal::Scanner scanner(interface->textDocument()->toPlainText(),
                                          interface->textDocument()->toPlainText().length(),
                                          PHPQt5::Internal::Scanner::State_Default,
                                          false);
        PHPQt5::Internal::Token tk;

        while ((tk = scanner.read()).type() != PHPQt5::Internal::Token::T_EOF
               && scanner.position() < interface->position()) {
        }

        QList<AssistProposalItemInterface *> items = m_snippetCollector.collect();
        items.append(generateProposalListForClass(m_word, QStringLiteral("PlastiQMethod::Static")));

        return new GenericProposal(nextCharPos+2, items);
    }

    else if (m_word.length() >= 2
             && m_interface->characterAt(nextCharPos) == QLatin1Char(':')
             && m_interface->characterAt(nextCharPos+1) == QLatin1Char(':')
             && m_word == QStringLiteral("parent")) {
        PHPQt5::Internal::Scanner scanner(interface->textDocument()->toPlainText(),
                                          interface->textDocument()->toPlainText().length(),
                                          PHPQt5::Internal::Scanner::State_Default,
                                          false);
        PHPQt5::Internal::Token tk;

        while ((tk = scanner.read()).type() != PHPQt5::Internal::Token::T_EOF
               && scanner.position() < interface->position()) {
        }

        QString className = scanner.getCurrentClassName();

        if (!className.isEmpty()) {
            QList<AssistProposalItemInterface *> items = m_snippetCollector.collect();
            items.append(generateProposalListForClass(className));

            return new GenericProposal(nextCharPos+2, items);
        }
        else return nullptr;
    }

    else if (m_word == "@override" || m_word == "@override ") {
        Scanner scanner(interface->textDocument()->toPlainText(),
                        interface->position(),
                        Scanner::State_Default, false);

        Token tk;
        while ((tk = scanner.read()).type() != Token::T_EOF) {
        }

        qDebug() << m_word << scanner.state() << scanner.getCurrentClassName();

        if (scanner.state() != Scanner::State_MultilineDComment)
            return nullptr;

        QString className = scanner.getCurrentClassName();
        if (className.isEmpty())
            return nullptr;

        QList<AssistProposalItemInterface *> items = m_snippetCollector.collect();
        items.append(generateOverrideListForClass(className, "::Virtual"));

        return new GenericProposal(nextCharPos, items);
    }

    else if (m_word.startsWith(QLatin1Char('$'))
             && m_interface->characterAt(nextCharPos) == QLatin1Char('-')
             && m_interface->characterAt(nextCharPos+1) == QLatin1Char('>')) {

        PHPQt5::Internal::Scanner scanner(interface->textDocument()->toPlainText(),
                                          interface->textDocument()->toPlainText().length(),
                                          PHPQt5::Internal::Scanner::State_Default,
                                          false);
        PHPQt5::Internal::Token tk;

        while ((tk = scanner.read()).type() != PHPQt5::Internal::Token::T_EOF
               && scanner.position() < interface->position()) {
        }

        QString varType = scanner.getVarType(m_word);

        if (!varType.isEmpty()) {
            QList<AssistProposalItemInterface *> items = m_snippetCollector.collect();
            items.append(generateProposalListForClass(varType));

            //return generateProposalListForVar(varType, nextCharPos+2);
            return new GenericProposal(nextCharPos+2, items);
        }
        else if (m_word == QStringLiteral("$this")) {

            PHPQt5::Internal::ASTNode *node = scanner.currentNode();
            while (node = node->parent()) {
                if (node->token().is(PHPQt5::Internal::Token::T_EXTENDS)) {
                    QString className = node->extToken().value();
                    QList<AssistProposalItemInterface *> items = m_snippetCollector.collect();
                    items.append(generateProposalListForClass(className));
                    //return generateProposalListForVar(className, nextCharPos+2);
                    return new GenericProposal(nextCharPos+2, items);
                }
            }

            return nullptr;
        }
    }

    else if (m_interface->characterAt(m_startPosition-1) != QLatin1Char('>')) {
        QList<AssistProposalItemInterface *> items = m_snippetCollector.collect();
        // items.append(generateProposalList(m_keywords.variables(), m_variableIcon));
        // items.append(generateProposalList(m_keywords.variables(), m_variableIcon)); // duplicate lines BUG!??
        items.append(generateProposalFnList(m_keywords.functions(), m_functionIcon));
        items.append(generateProposalVarList(m_keywords.variables(), m_variableIcon));
        items.append(generateProposalDynVarList(m_dvp->list(), m_variableIcon));

        return new GenericProposal(m_startPosition, items);
    }

    return nullptr;
}

QChar PHPCompletionAssistProcessor::startOfCommentChar() const
{
    return QLatin1Char('#');
}

void PHPCompletionAssistProcessor::setSnippetGroup(const QString &id)
{
    m_snippetCollector.setGroupId(id);
}

void PHPCompletionAssistProcessor::setKeywords(Keywords keywords)
{
    m_keywords = keywords;
}

bool PHPCompletionAssistProcessor::isClassName(const QString &className, QString *path)
{
    return PHPQt5::ClassHelpers::isClass(className, path);

    /*
    if (m_classCompletions.contains(className)) {
        return true;
    }

    if (className.startsWith(QLatin1Char('Q')) && className.at(1).isUpper()) {
        QDir dir(qApp->applicationDirPath());
        dir.cd("../../PHPQt5/plastiq/include/");
        dir.setFilter(QDir::NoDotAndDotDot);

        if (dir.exists()) {
            QDirIterator it(dir.path());

            while (it.hasNext()) {
                QDir lPath(QString("%1/%2").arg(it.next()).arg(QString("PlastiQ%1").arg(className)));

                if (lPath.exists()) {
                    QFile cFile(QString("%1/plastiq%2.cpp").arg(lPath.path()).arg(className.toLower()));

                    if (cFile.exists()) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
    */
}

bool PHPCompletionAssistProcessor::acceptsIdleEditor()
{
    const int pos = m_interface->position();
    const QChar characterUnderCursor = m_interface->characterAt(pos);

    if (!characterUnderCursor.isLetterOrNumber()) {
        m_startPosition = findStartOfName();
        if (pos - m_startPosition >= 3 && !isInComment())
            return true;
    }

    return false;
}

int PHPCompletionAssistProcessor::findStartOfName(int pos)
{
    if (pos == -1)
        pos = m_interface->position();

    QChar chr = m_interface->characterAt(pos-1);
    if (chr == QLatin1Char('('))
        --pos;
    else if (chr == QLatin1Char('>') && m_interface->characterAt(pos-2) == QLatin1Char('-')) {
        // trigger for $var->
        pos -= 2;
    }
    else if (chr == QLatin1Char(':') && m_interface->characterAt(pos-2) == QLatin1Char(':')) {
        // trigger for Class::
        pos -= 2;
    }

    // Skip to the start of a name
    do {
        chr = m_interface->characterAt(--pos);
    } while (chr.isLetterOrNumber()
             || chr == QLatin1Char('_')
             || chr == QLatin1Char('$')
             || chr == QLatin1Char('@'));

    const int start = ++pos;
    m_word.clear();
    do {
        m_word += m_interface->characterAt(pos);
        chr = m_interface->characterAt(++pos);
    } while ((chr.isLetterOrNumber() || chr == QLatin1Char('_')) && chr != QLatin1Char('('));

    return start;
}

bool PHPCompletionAssistProcessor::isInComment() const
{
    QTextCursor tc(m_interface->textDocument());
    tc.setPosition(m_interface->position());
    tc.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    const QString &lineBeginning = tc.selectedText();
    return lineBeginning.contains(startOfCommentChar());
}

QList<AssistProposalItemInterface *>
PHPCompletionAssistProcessor::generateProposalList(const QStringList &words, const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        AssistProposalItem *item = new PHPAssistProposalItem(m_keywords.isFunction(word));
        item->setText(word);
        item->setIcon(icon);
        return item;
    });
}

QList<AssistProposalItemInterface *>
PHPCompletionAssistProcessor::generateProposalDynVarList(const QStringList &words, const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        AssistProposalItem *item = new PHPAssistProposalItem(false);
        item->setText(word);
        item->setIcon(icon);
        return item;
    });
}

QList<AssistProposalItemInterface *>
PHPCompletionAssistProcessor::generateProposalVarList(const QStringList &words, const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        AssistProposalItem *item = new PHPAssistProposalItem(false);
        item->setText(word);
        item->setIcon(icon);
        return item;
    });
}

QList<AssistProposalItemInterface *>
PHPCompletionAssistProcessor::generateProposalFnList(const QStringList &words, const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        AssistProposalItem *item = new PHPAssistProposalItem(true);
        item->setText(word);
        item->setIcon(icon);

        return item;
    });
}

QList<AssistProposalItemInterface *>
PHPCompletionAssistProcessor::generateProposalMethodsList(const QStringList &words,
                                                          const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        AssistProposalItem *item = new PHPAssistProposalItem(true);
        item->setText(word);
        item->setIcon(icon);

        if (m_details.contains(word)) item->setDetail(m_details.value(word));

        return item;
    });
}


QList<AssistProposalItemInterface *> PHPCompletionAssistProcessor::generateOverrideMethodsList(const QStringList &words, const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        QStringList data;

        if (m_odetails.contains(word)) {
            data.append(m_odetails.value(word));
        }

        AssistProposalItem *item = new PHPAssistOverrideMethodItem();
        item->setText(word);
        item->setIcon(icon);
        item->setData(data);

        if (m_details.contains(word)) item->setDetail(m_details.value(word));

        return item;
    });
}


QList<AssistProposalItemInterface *> PHPCompletionAssistProcessor::generateProposalPropsList(const QStringList &words, const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        AssistProposalItem *item = new PHPAssistProposalPropItem();
        item->setText(QString("%1 ").arg(word));
        item->setIcon(icon);
        item->setData(word);

        if (m_pdetails.contains(word)) item->setDetail(m_pdetails.value(word));
        else item->setDetail("property");

        return item;
    });
}

QList<AssistProposalItemInterface *> PHPCompletionAssistProcessor::generateProposalOnSlotsList(const QStringList &words, const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        QStringList data;

        data.append(word);
        if (m_details.contains(word)) {
            data.append(m_details.value(word).split(QLatin1Char('+')));
        }

        AssistProposalItem *item = new PHPAssistProposalOnSlotItem();
        item->setText(word);
        item->setIcon(icon);
        item->setData(data);

        if (data.size() > 1) item->setDetail(data.at(1));

        return item;
    });
}

QList<AssistProposalItemInterface *> PHPCompletionAssistProcessor::generateProposalConstList(const QStringList &words, const QIcon &icon)
{
    return Utils::transform(words, [this, &icon](const QString &word) -> AssistProposalItemInterface * {
        AssistProposalItem *item = new PHPAssistProposalConstItem();
        item->setText(word);
        item->setIcon(icon);
        item->setData(word.trimmed());

        return item;
    });
}

QList<AssistProposalItemInterface *> PHPCompletionAssistProcessor::getItemsFromClass(const QString &className, const QString &filter)
{
    QList<AssistProposalItemInterface *> items;

    if (m_classCompletions.contains(className)) {
        const ClassCompletions &cc = m_classCompletions.value(className);
        items.append(generateProposalPropsList(cc._properties, m_variableIcon));
        items.append(generateProposalMethodsList(cc._methods, m_functionIcon));
        items.append(generateProposalOnSlotsList(cc._onProps, m_functionIcon));

        return items;
    }

    if ((className.startsWith(QLatin1Char('Q')) && className.at(1).isUpper())
            || className == "Qt") {
        QDir dir(qApp->applicationDirPath());
        dir.cd("../../PHPQt5/plastiq/include/");
        dir.setFilter(QDir::NoDotAndDotDot);

        if (dir.exists()) {
            QDirIterator it(dir.path());

            while (it.hasNext()) {
                QDir lPath(QString("%1/%2").arg(it.next()).arg(QString("PlastiQ%1").arg(className)));

                if (lPath.exists()) {
                    QFile cFile(QString("%1/plastiq%2.cpp").arg(lPath.path()).arg(className.toLower()));

                    if (cFile.exists() && cFile.open(QFile::ReadOnly)) {
                        QString cData = QString(QLatin1String(cFile.readAll().constData()));
                        ClassCompletions cc;

                        // МЕТОДЫ
                        int sidx = cData.indexOf(QString("QHash<QByteArray, PlastiQMethod> "
                                                         "PlastiQ%1::plastiqMethods = {").arg(className));
                        if (sidx < 0) return items;

                        int eidx = cData.indexOf("};", sidx);
                        if (eidx < 0) return items;

                        foreach (const QString &method, cData.mid(sidx,eidx-sidx).split('\n')) {
                            if (!filter.isEmpty() && !method.contains(filter))
                                continue;

                            int msidx = method.indexOf(", { \"");
                            if (msidx < 0) continue;

                            int meidx = method.indexOf("\"", msidx + 5);
                            QString word = method.mid(msidx + 5, meidx - msidx - 5);
                            cc._methods << word;

                            int dsidx = method.indexOf("{ \"");
                            if (dsidx < 0) continue;
                            int rsidx = method.lastIndexOf("\", \"");
                            if (rsidx < 0) continue;

                            QString details = method.mid(dsidx + 3, method.indexOf("\"", dsidx + 3) - dsidx - 3);
                            QString ret = method.mid(rsidx + 4, method.indexOf("\"", rsidx + 4) - rsidx - 4);
                            m_details.insert(word, QString("%1 %2::%3").arg(ret, className, details));
                            details.replace("&", "").replace("*", "");

                            if (!filter.isEmpty()) {
                                QString argsLine = details.mid(details.indexOf('(')+1, details.lastIndexOf(')')-details.indexOf('(')-1);
                                QStringList args;
                                args.append(ret);
                                args.append(argsLine.split(QLatin1Char(',')));

                                QStringList oargs;
                                foreach (const QString &arg, args) {
                                    if (arg.at(0) == QLatin1Char('Q') && arg.at(1).isUpper()) {
                                        oargs.append(QStringLiteral("%1+$%2%3")
                                                     .arg(arg).arg(arg.mid(1,1).toLower(), arg.mid(2)));
                                    }
                                    else if (arg.at(0).isUpper()){
                                        oargs.append(QStringLiteral("%1+$%2%3")
                                                     .arg(arg).arg(arg.mid(0,1).toLower(), arg.mid(1)));
                                    }
                                    else {
                                        oargs.append(QStringLiteral("%1+$%2")
                                                     .arg(arg).arg(arg));
                                    }
                                }

                                m_odetails.insert(word, oargs);
                            }
                        }

                        if (filter.isEmpty()) {
                            // СВОЙСТВА
                            sidx = cData.indexOf(QString("QHash<QByteArray, PlastiQProperty> "
                                                         "PlastiQ%1::plastiqProperties = {").arg(className));
                            if (sidx < 0) break;

                            eidx = cData.indexOf("};", sidx);
                            if (eidx < 0) break;

                            foreach (const QString &property, cData.mid(sidx,eidx-sidx).split('\n')) {
                                int psidx = property.indexOf("{ \"");
                                if (psidx < 0) continue;

                                int peidx = property.indexOf("\"", psidx + 3);
                                QString word = property.mid(psidx + 3, peidx - psidx - 3);
                                cc._properties << word;

                                int tsidx = property.indexOf("\", \"");
                                if (tsidx < 0) continue;

                                //QString details = property.mid(dsidx + 3, property.indexOf("\"", dsidx + 3) - dsidx - 3);
                                QString type = property.mid(tsidx + 4, property.indexOf("\"", tsidx + 4) - tsidx - 4);
                                m_pdetails.insert(word, QString("property: %1 %2::%3").arg(type, className, word));
                            }

                            // СИГНАЛЫ
                            sidx = cData.indexOf(QString("QHash<QByteArray, PlastiQMethod> "
                                                         "PlastiQ%1::plastiqSignals = {").arg(className));
                            if (sidx < 0) break;

                            eidx = cData.indexOf("};", sidx);
                            if (eidx < 0) break;

                            foreach (const QString &signal, cData.mid(sidx, eidx-sidx).split('\n')) {
                                int ssidx = signal.indexOf(", { \"");
                                if (ssidx < 0) continue;

                                int seidx = signal.indexOf("\"", ssidx + 5);
                                QString word = signal.mid(ssidx + 5, seidx - ssidx - 5);
                                word = QString("on%1%2").arg(word.mid(0,1).toUpper()).arg(word.mid(1));
                                cc._onProps << word;

                                int dsidx = signal.indexOf("{ \"");
                                if (dsidx < 0) continue;
                                int rsidx = signal.lastIndexOf("\", \"");
                                if (rsidx < 0) continue;

                                QString details = signal.mid(dsidx + 3, signal.indexOf("\"", dsidx + 3) - dsidx - 3);
                                details = details.replace("&", "").replace("*", "");

                                QString argLine = QStringLiteral("$sender");
                                QString argsLine = details.mid(details.indexOf('(')+1, details.lastIndexOf(')')-details.indexOf('(')-1);
                                QStringList args = argsLine.split(QLatin1Char(','));

                                if (!argsLine.isEmpty()) {
                                    foreach (const QString &arg, args) {
                                        if (arg.at(0) == QLatin1Char('Q') && arg.at(1).isUpper()) {
                                            argLine += QStringLiteral(", $%1%2").arg(arg.mid(1,1).toLower(), arg.mid(2));
                                        }
                                        else if (arg.at(0).isUpper()){
                                            argLine += QStringLiteral(", $%1%2").arg(arg.mid(0,1).toLower(), arg.mid(1));
                                        }
                                        else {
                                            argLine += QStringLiteral(", $") + arg;
                                        }
                                    }
                                }

                                qDebug() << "->>>>>>>" << details << argLine;

                                // QString ret = signal.mid(rsidx + 4, signal.indexOf("\"", rsidx + 4) - rsidx - 4);
                                m_details.insert(word, QString("signal: %1::%2").arg(className, details));
                            }

                            // КОНСТАНТЫ
                            QString constantStr = QString("QHash<QByteArray, long> "
                                                          "PlastiQ%1::plastiqConstants = {").arg(className);
                            sidx = cData.indexOf(constantStr);
                            if (sidx < 0) break;

                            eidx = cData.indexOf("};", sidx);
                            if (eidx < 0) break;

                            foreach (const QString &constant, cData.mid(sidx+constantStr.length(),eidx-sidx-constantStr.length()).split('\n')) {
                                int psidx = constant.indexOf("::");
                                if (psidx < 0) continue;

                                int peidx = constant.indexOf(" ", psidx + 2);
                                QString word = constant.mid(psidx + 2, peidx - psidx - 2).append(" ");
                                cc._constants << word;
                            }

                            items.append(generateProposalOnSlotsList(cc._onProps, m_onPropertyIcon));
                            items.append(generateProposalPropsList(cc._properties, m_variableIcon));
                            items.append(generateProposalConstList(cc._constants, m_constantIcon));
                        }

                        if (!filter.isEmpty())
                            items.append(generateOverrideMethodsList(cc._methods,
                                                                     CPlusPlus::Icons::iconForType(CPlusPlus::Icons::FuncPrivateIconType)));
                        else items.append(generateProposalMethodsList(cc._methods, m_functionIcon));

                        m_classCompletions.insert(className, cc);
                        return items;
                    }
                }
            }
        }
    }

    return items;
}

QStringList PHPCompletionAssistProcessor::getClassInherits(const QString &className)
{
    QStringList inherits;

    if (m_classInherits.contains(className)) {
        return m_classInherits.value(className);
    }
    else if (className.startsWith(QLatin1Char('Q')) && className.at(1).isUpper()) {
        QDir dir(qApp->applicationDirPath());
        dir.cd("../../PHPQt5/plastiq/include/");
        dir.setFilter(QDir::NoDotAndDotDot);

        if (dir.exists()) {
            QDirIterator it(dir.path());

            while (it.hasNext()) {
                QDir lPath(QString("%1/%2").arg(it.next()).arg(QString("PlastiQ%1").arg(className)));

                if (lPath.exists()) {
                    QFile cFile(QString("%1/plastiq%2.h").arg(lPath.path()).arg(className.toLower()));

                    if (cFile.exists() && cFile.open(QFile::ReadOnly)) {
                        QString cData = QString(QLatin1String(cFile.readAll().constData()));
                        QRegExp rx("PLASTIQ_INHERITS\\((.*)\\)");
                        rx.setMinimal(true);

                        if (rx.indexIn(cData, 0) != -1) {
                            inherits.append(rx.cap(1).split(QLatin1Char(',')));

                            foreach (const QString &cn, inherits) {
                                if (!cn.trimmed().isEmpty())
                                    inherits.append(getClassInherits(cn));
                            }

                            m_classInherits.insert(className, inherits);
                        }
                    }
                }
            }
        }
    }

    return inherits;
}

QList<AssistProposalItemInterface *> PHPCompletionAssistProcessor::generateProposalListForClass(const QString &className,
                                                                                                const QString &filter)
{
    m_details.clear();
    QList<AssistProposalItemInterface *> items;

    QStringList inherits = getClassInherits(className);

    foreach (const QString &cn, inherits) {
        if (!cn.trimmed().isEmpty())
            items.append(getItemsFromClass(cn, filter));
    }

    items.append(getItemsFromClass(className, filter));

    return items;
}

QList<AssistProposalItemInterface *> PHPCompletionAssistProcessor::generateOverrideListForClass(const QString &className, const QString &filter)
{
    m_details.clear();
    QList<AssistProposalItemInterface *> items;

    QStringList inherits = getClassInherits(className);

    foreach (const QString &cn, inherits) {
        if (!cn.trimmed().isEmpty())
            items.append(getItemsFromClass(cn, filter));
    }

    items.append(getItemsFromClass(className, filter));

    return items;
}

PHPAssistProposalPropItem::PHPAssistProposalPropItem()
{

}

void PHPAssistProposalPropItem::applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const
{
    int replaceLength = manipulator.currentPosition() - basePosition;
    QString toInsert = data().toString();
    manipulator.replace(basePosition, replaceLength, toInsert);
}

PHPAssistProposalOnSlotItem::PHPAssistProposalOnSlotItem()
{

}

void PHPAssistProposalOnSlotItem::applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const
{
    int replaceLength = manipulator.currentPosition() - basePosition;
    QStringList d = data().toStringList();
    QString toInsert = d.at(0);

    if (d.size() > 1) {
        toInsert.append(" = function(");
        toInsert.append(d.at(2));
        toInsert.append(") {};");
    }

    manipulator.replace(basePosition, replaceLength, toInsert);
    manipulator.setCursorPosition(basePosition+toInsert.length()-2);
}

PHPAssistProposalConstItem::PHPAssistProposalConstItem()
{

}

void PHPAssistProposalConstItem::applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const
{
    int replaceLength = manipulator.currentPosition() - basePosition;
    QString toInsert = data().toString();
    manipulator.replace(basePosition, replaceLength, toInsert);
}

PHPAssistOverrideMethodItem::PHPAssistOverrideMethodItem()
{

}

void PHPAssistOverrideMethodItem::applyContextualContent(TextDocumentManipulatorInterface &manipulator, int basePosition) const
{
    QString indent("    ");

    int replaceLength = manipulator.currentPosition() - basePosition;
    QStringList d = data().toStringList();
    QString toInsert;
    toInsert.append(' ');
    toInsert.append(text());

    QStringList vars;
    for (int i = 1; i < d.size(); i++) {
        QStringList dd = d.at(i).split('+');
        toInsert.append('\n');
        toInsert.append(indent).append(QStringLiteral(" * @param %1").arg(dd.at(0)));
        vars.append(dd.at(1));
    }

    toInsert.append('\n');
    toInsert.append(indent).append(" *");
    toInsert.append('\n');
    toInsert.append(indent).append(QStringLiteral(" * @return %1").arg(d.at(0).split('+').at(0)));
    toInsert.append('\n');
    toInsert.append(indent).append(" */\n");
    toInsert.append(indent).append(QStringLiteral("protected function %1(%2) {\n").arg(text()).arg(vars.join(", ")));
    toInsert.append(indent).append(indent).append('\n');
    toInsert.append(indent).append('}');

    // indent.length()+2
    manipulator.replace(basePosition, replaceLength, toInsert);
    manipulator.setCursorPosition(basePosition+toInsert.length()-6);
}

}
}
