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

#pragma once

#include "dynvarsprocessor.h"

#include <texteditor/indenter.h>
#include <texteditor/normalindenter.h>

#include <cpptools/cppcodestylesettings.h>

#include <QTextBlockUserData>

namespace CppTools {
class CppCodeStyleSettings;
}

namespace PHPQt5 {
namespace Internal {

using namespace PHPEditor;
using namespace TextEditor;
using namespace CppTools;

class PHPCodeStyleSettings : public CppCodeStyleSettings
{
    bool indentBlockBraces = true;
    bool indentBlockBody = true;
    bool indentClassBraces = true;
    bool indentEnumBraces = true;
    bool indentNamespaceBraces = true;
    bool indentNamespaceBody = true;
    bool indentAccessSpecifiers = true;
    bool indentDeclarationsRelativeToAccessSpecifiers = true;
    bool indentFunctionBody = true;
    bool indentFunctionBraces = true;
    bool indentSwitchLabels = true;
    bool indentStatementsRelativeToSwitchLabels = true;
    bool indentBlocksRelativeToSwitchLabels = true;
    bool indentControlFlowRelativeToSwitchLabels = true;

    // Formatting of pointer and reference declarations, see Overview::StarBindFlag.
    bool bindStarToIdentifier = false;
    bool bindStarToTypeName = false;
    bool bindStarToLeftSpecifier = false;
    bool bindStarToRightSpecifier = false;

    // false: if (a &&
    //            b)
    //            c;
    // true:  if (a &&
    //                b)
    //            c;
    // but always: while (a &&
    //                    b)
    //                 foo;
    bool extraPaddingForConditionsIfConfusingAlign = false;

    // false: a = a +
    //                b;
    // true:  a = a +
    //            b
    bool alignAssignments = true;

    bool preferGetterNameWithoutGetPrefix = false;

    bool isInit = false;
};

class PHPIndenter : public NormalIndenter
{
public:
    PHPIndenter(DynVarsProcessor *dvp);

    PHPCodeStyleSettings codeStyleSettings() const;

private:

    bool isElectricCharacter(const QChar &ch) const override;
    int indentFor(const QTextBlock &block, const TextEditor::TabSettings &tabSettings) override;

    void indentBlock(QTextDocument *doc, const QTextBlock &block,
                     const QChar &typedChar, const TextEditor::TabSettings &tabSettings) override;

    // Indent at cursor. Calls indentBlock for selection or current line.
    void indent(QTextDocument *doc,
                        const QTextCursor &cursor,
                        const QChar &typedChar,
                        const TextEditor::TabSettings &tabSettings) override;

    // Reindent at cursor. Selection will be adjusted according to the indentation
    // change of the first block.
    void reindent(QTextDocument *doc, const QTextCursor &cursor, const TextEditor::TabSettings &tabSettings) override;

    bool isElectricLine(const QString &line) const;
    int getIndentDiff(const QString &previousLine,
                      const TextEditor::TabSettings &tabSettings) const;

    DynVarsProcessor *m_dvp;
};

}
} // namespace PHPQt5
