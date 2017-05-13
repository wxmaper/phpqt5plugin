/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "phpindenter.h"
#include "../parser/astnode.h"
#include "../parser/scanner.h"
//#include "phplexer.h"
//#include "phptoken.h"
//#include "phptokeninfo.h"

#include <texteditor/tabsettings.h>
#include <texteditor/textdocumentlayout.h>

#include <cpptools/cppcodeformatter.h>

#include <QMessageBox>
#include <QTextDocument>

#include "../debugger/phpqt5debugger.h"


namespace PHPQt5 {
namespace Internal {

using namespace CppTools;

/**
 * @brief Does given character change indentation level?
 * @param ch Any value
 * @return True if character increases indentation level at the next line
 */
PHPIndenter::PHPIndenter(DynVarsProcessor *dvp)
    : m_dvp(dvp) {}

bool PHPIndenter::isElectricCharacter(const QChar &ch) const
{
    //qDebug() << "isElectricCharacter" << ch;
    if (ch == QLatin1Char('{')
            || ch == QLatin1Char('}')) {
        return true;
    }
    return false;
}

void PHPIndenter::indentBlock(QTextDocument *doc,
                              const QTextBlock &block,
                              const QChar &typedChar,
                              const TextEditor::TabSettings &tabSettings)
{
    //qDebug() << "indentBlock";
    //  Q_UNUSED(doc);
    /*
    int indent = indentFor(block, tabSettings);

    if (typedChar == QLatin1Char('}')
            || (block.text().right(1) == "}"))
        indent -= tabSettings.m_indentSize;


    tabSettings.indentLine(block, qMax(0, indent));
    */
}

PHPCodeStyleSettings PHPIndenter::codeStyleSettings() const
{
    return PHPCodeStyleSettings();
}

void PHPIndenter::indent(QTextDocument *doc,
                         const QTextCursor &cursor,
                         const QChar &typedChar,
                         const TextEditor::TabSettings &tabSettings)
{
    const QString sourceData = doc->toPlainText();//.mid(0, cursor.position());


    //    Lexer lexer(sourceData.constData(), sourceData.size());

    //    QStack<Lexer::StateInfo> stateStack;
    //    TokenInfo tki;

    //    while ((tki = lexer.read()).token() != Token::T_EOF) {
    //        if (lexer.pos() <= cursor.position()+1) {
    //            stateStack = lexer.stateStack;
    //        }
    //    }

    //    {   // почему-то это здесь... да :-(
    //        m_dvp->clear();
    //        m_dvp->append(lexer.variables());
    //    }

    QtStyleCodeFormatter codeFormatter(tabSettings, codeStyleSettings());

    QTextBlock block = cursor.block();
    codeFormatter.updateStateUntil(block);

    Scanner scanner(sourceData, sourceData.size(), Scanner::State_Default, false);
    Token tk;
    QStack<RegionData> regionStack;
    Scanner::State state;
    while ((tk = scanner.read()).type() != Token::T_EOF) {
        if (scanner.position() <= cursor.position()) {
            regionStack = scanner.regionStack();
            state = scanner.state();
        }
    }

    if (state == Scanner::State_MultilineDComment) {
        int indent, padding;
        codeFormatter.indentFor(block, &indent, &padding);
        tabSettings.indentLine(block, indent + padding, padding);
    }
    else {
        int previousIndent = 0;

        QTextBlock previous = cursor.block().previous();
        QString previousText;
        if (previous.isValid()) {
            previousText = previous.text();
            while (previousText.trimmed().isEmpty()) {
                previous = previous.previous();
                if (!previous.isValid())
                    break;

                previousText = previous.text();
            }

            previousIndent = tabSettings.indentationColumn(previousText);
        }

        int regionSize = regionStack.size();
        foreach (RegionData regionData, regionStack) {
            switch (regionData.nodeType) {
            case ASTNode::FunctionDeclaration:
            case ASTNode::IfExpression:
                regionSize--;
                break;

            default: //nothing
                break;
            }
        }

        if (regionStack.size()) {
            switch (regionStack.last().nodeType) {
            case ASTNode::FunctionDeclaration:
            case ASTNode::IfExpression: {
                tabSettings.indentLine(cursor.block(), qMax(0, regionStack.last().startPosition));
                return; // return! all right
            }

            default: // nothing
                break;
            }
        }

        if (regionSize < 0)
            regionSize = 0;

        int indent = tabSettings.m_indentSize * regionSize;

        //qDebug() << Q_FUNC_INFO << "1" << typedChar << previousIndent<< indent;

        if (typedChar == QLatin1Char('{'))
            indent -= cursor.block().text().count('{') * tabSettings.m_indentSize;
        //qDebug() << Q_FUNC_INFO << "2" << typedChar << indent << cursor.block().text();

        if (typedChar == QLatin1Char('\x0')
                && cursor.block().text().trimmed() == "}"
                && ((cursor.block().text().count('{') - cursor.block().text().count('}')) != 0)) {
            indent -= tabSettings.m_indentSize;
        }
        //qDebug() << Q_FUNC_INFO << "3" << typedChar << indent << cursor.block().text();

        if (typedChar == '}'
                && cursor.block().text().trimmed().at(0) != '}'
                && ((cursor.block().text().count('{') - cursor.block().text().count('}')) != 0))
            indent += tabSettings.m_indentSize;

        if (indent < previousIndent && typedChar != '}'
                && cursor.block().text().trimmed().at(0) != '}')
            indent = previousIndent;

        if (indent < previousIndent
                && typedChar == '}' && cursor.block().text().trimmed() != "}") {
            indent = previousIndent;
        }

        //qDebug() << Q_FUNC_INFO << "4" << typedChar << indent << cursor.block().text();

        tabSettings.indentLine(cursor.block(), qMax(0, indent));
    }


    /*
    Scanner scanner(sourceData,
                    sourceData.size(),
                    Scanner::State_Default,
                    false);
    Token tk;

    while ((tk = scanner.read()).type() != Token::T_EOF) {
        if (scanner.position() <= cursor.position()+1) {
            regionStack = scanner.regionStack();
        }
    }

    //qDebug() << "ident" << regionStack.size();
    //TextEditor::TextDocumentLayout::setBraceDepth(cursor.document()->lastBlock(),
    //                                              regionStack.size());

    int previousIndent = 0;

    QTextBlock previous = cursor.block().previous();
    QString previousText;
    if (previous.isValid()) {
        previousText = previous.text();
        while (previousText.trimmed().isEmpty()) {
            previous = previous.previous();
            if (!previous.isValid())
                break;

            previousText = previous.text();
        }

        previousIndent = tabSettings.indentationColumn(previousText);
    }

    int indent = tabSettings.m_indentSize * (regionStack.size());
    if (typedChar == QLatin1Char('{'))
        indent -= cursor.block().text().count('{') * tabSettings.m_indentSize;

    if (typedChar == '}'
            && cursor.block().text().trimmed().at(0) != '}'
            && ((cursor.block().text().count('{') - cursor.block().text().count('}')) != 0))
        indent += tabSettings.m_indentSize;

    if (indent < previousIndent && typedChar != '}' && cursor.block().text().trimmed().at(0) != '}')
        indent = previousIndent;

    tabSettings.indentLine(cursor.block(), qMax(0, indent));
    */
}

void PHPIndenter::reindent(QTextDocument *doc, const QTextCursor &cursor, const TextEditor::TabSettings &tabSettings)
{
    //qDebug() << "reindent";
    PHPQt5OutputWindow::instance()->appendText(QString("reindent > "));
    if (cursor.hasSelection()) {
        QTextBlock block = doc->findBlock(cursor.selectionStart());
        const QTextBlock end = doc->findBlock(cursor.selectionEnd()).next();

        // skip empty blocks
        while (block.isValid() && block != end) {
            QString bt = block.text();
            if (tabSettings.firstNonSpace(bt) < bt.size())
                break;
            indentBlock(doc, block, QChar::Null, tabSettings);
            block = block.next();
        }

        int previousIndentation = tabSettings.indentationColumn(block.text());
        indentBlock(doc, block, QChar::Null, tabSettings);
        int currentIndentation = tabSettings.indentationColumn(block.text());
        int delta = currentIndentation - previousIndentation;

        block = block.next();
        while (block.isValid() && block != end) {
            tabSettings.reindentLine(block, delta);
            block = block.next();
        }
    } else {
        indentBlock(doc, cursor.block(), QChar::Null, tabSettings);
    }
}

int PHPIndenter::indentFor(const QTextBlock &block, const TextEditor::TabSettings &tabSettings)
{
    //qDebug() << "indentFor";
    return NormalIndenter::indentFor(block, tabSettings);
    //return 0;

    /*
    QTextBlock previousBlock = block.previous();
    if (!previousBlock.isValid())
        return 0;

    QString previousLine = previousBlock.text();
    int indentation = tabSettings.indentationColumn(previousLine);

    if (isElectricLine(previousLine))
        indentation += tabSettings.m_indentSize;
    else
        indentation = qMax<int>(0, indentation + getIndentDiff(previousLine, tabSettings));

    return indentation;
    */

    QTextBlock previous = block.previous();
    if (!previous.isValid())
        return 0;

    QString previousText = previous.text();
    while (previousText.trimmed().isEmpty()) {
        previous = previous.previous();
        if (!previous.isValid())
            return 0;
        previousText = previous.text();
    }

    int indent = tabSettings.indentationColumn(previousText);

    // qDebug() << "LAST stateStack" << lexer.stateStack.last();
    // if (lexer.stateStack.last() == Lexer::State_Condition)
    //     indent += tabSettings.m_indentSize;

    int adjust = previousText.count(QLatin1Char('{'));// - previousText.count(QLatin1Char('}'));


    adjust *= tabSettings.m_indentSize;

    PHPQt5OutputWindow::instance()->appendText(QString("indentFor > %1").arg(indent + adjust));

    return qMax(0, indent + adjust);
}

/// @return True if electric character is last non-space character at given string
bool PHPIndenter::isElectricLine(const QString &line) const
{
    if (line.isEmpty())
        return false;

    // trim spaces in 'if True:  ' ???
    int index = line.length() - 1;
    while (index > 0 && line[index].isSpace())
        --index;

    return isElectricCharacter(line[index]);
}

/// @return negative indent diff if previous line breaks control flow branch
int PHPIndenter::getIndentDiff(const QString &previousLine,
                               const TextEditor::TabSettings &tabSettings) const
{
    /*
    // trim spaces in 'if True:  ' ???
    int index = previousLine.length() - 1;
    while (index > 0 && previousLine[index].isSpace())
        --index;

    return previousLine[index] == '}' ? -4 : 0;
    */

    return 0;
}

}
} // namespace PHPQt5
