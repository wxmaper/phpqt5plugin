#include "projecteditorfactory.h"

#include "phpqt5constants.h"
#include "projecteditorwidget.h"
#include "projecteditor.h"

#include <coreplugin/id.h>
#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditorsettings.h>

namespace PHPQt5 {
namespace Internal {

class ProjectTextEditorActionHandler : public TextEditor::TextEditorActionHandler
{
public:
    explicit ProjectTextEditorActionHandler(QObject *parent)
        : TextEditorActionHandler(parent, Constants::PROJECT_EDITOR_CONTEXT)
    {}

private:
    TextEditor::TextEditorWidget *resolveTextEditorWidget(Core::IEditor *editor) const
    {
        ProjectEditor *projectEditor = static_cast<ProjectEditor *>(editor);
        return projectEditor->textEditor();
    }
};

ProjectEditorFactory::ProjectEditorFactory(QObject *parent)
    : Core::IEditorFactory(parent)
{
    setId(Constants::PROJECT_EDITOR_ID);
    setDisplayName(tr("PHPQt5 Project editor"));
    addMimeType(Constants::C_PHPQT5_PROJECT_MIMETYPE);
    new ProjectTextEditorActionHandler(this);
}

Core::IEditor *ProjectEditorFactory::createEditor()
{
    ProjectEditorWidget *projectEditorWidget = new ProjectEditorWidget();
    return projectEditorWidget->editor();
}

} // namespace Internal
} // namespace PHPQt5
