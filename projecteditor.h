#ifndef PROJECTEDITOR_H
#define PROJECTEDITOR_H

#include "projecteditorwidget.h"
#include <coreplugin/editormanager/ieditor.h>
#include <texteditor/texteditor.h>

#include <QActionGroup>

namespace PHPQt5 {
namespace Internal {

class ProjectEditor : public Core::IEditor
{
    Q_OBJECT

public:
    explicit ProjectEditor(ProjectEditorWidget *editorWidget);

    QWidget *toolBar() override;
    ProjectEditorWidget *widget() const override;
    Core::IDocument *document() override;
    TextEditor::TextEditorWidget *textEditor() const;

    //int currentLine() const override;
    //int currentColumn() const override;
    //void gotoLine(int line, int column = 0, bool centerLine = true)  override;

private:
    void changeEditorPage(QAction *action);

    QString m_displayName;
    QToolBar *m_toolBar;
    QActionGroup *m_actionGroup;
};

}
}

#endif // PROJECTEDITOR_H
