#include "projecteditor.h"
#include "phpqt5constants.h"

#include <QToolBar>

#include <coreplugin/idocument.h>

#include <texteditor/textdocument.h>

namespace PHPQt5 {
namespace Internal {

ProjectEditor::ProjectEditor(ProjectEditorWidget *editorWidget)
    : Core::IEditor(editorWidget), m_toolBar(0)
{
    m_toolBar = new QToolBar(editorWidget);
    m_actionGroup = new QActionGroup(this);
    connect(m_actionGroup, &QActionGroup::triggered,
            this, &ProjectEditor::changeEditorPage);

    QAction *generalAction = m_toolBar->addAction(tr("General"));
    generalAction->setData(ProjectEditorWidget::General);
    generalAction->setCheckable(true);
    m_actionGroup->addAction(generalAction);

    QAction *sourceAction = m_toolBar->addAction(tr("Source"));
    sourceAction->setData(ProjectEditorWidget::Source);
    sourceAction->setCheckable(true);
    m_actionGroup->addAction(sourceAction);

    generalAction->setChecked(true);

    setContext(Core::Context(Constants::PROJECT_EDITOR_CONTEXT));
    setWidget(editorWidget);
}

QWidget *ProjectEditor::toolBar()
{
    return m_toolBar;
}

ProjectEditorWidget *ProjectEditor::widget() const
{
    return static_cast<ProjectEditorWidget *>(Core::IEditor::widget());
}

Core::IDocument *ProjectEditor::document()
{
    return textEditor()->textDocument();
}

TextEditor::TextEditorWidget *ProjectEditor::textEditor() const
{
    return widget()->textEditorWidget();
}

void ProjectEditor::changeEditorPage(QAction *action)
{
    if (!widget()->setActivePage(static_cast<ProjectEditorWidget::EditorPage>(action->data().toInt()))) {
        foreach (QAction *action, m_actionGroup->actions()) {
            if (action->data().toInt() == widget()->activePage()) {
                action->setChecked(true);
                break;
            }
        }
    }
}



}
}
