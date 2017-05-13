#include "projectdocument.h"

#include "phpqt5constants.h"
#include "projecteditorwidget.h"
#include "qmljstools/qmljstoolsconstants.h"

namespace PHPQt5 {
namespace Internal {

ProjectDocument::ProjectDocument(ProjectEditorWidget *editorWidget)
    : QmlJSEditor::QmlJSEditorDocument(),
      m_editorWidget(editorWidget)
{
    setId(Constants::PROJECT_EDITOR_ID);
    setMimeType(QLatin1String(QmlJSTools::Constants::JSON_MIMETYPE));
    setSuspendAllowed(false);
    connect(editorWidget, &ProjectEditorWidget::guiChanged,
            this, &Core::IDocument::changed);
}

bool ProjectDocument::save(QString *errorString, const QString &fileName, bool autoSave)
{
    m_editorWidget->preSave();
    bool result = TextDocument::save(errorString, fileName, autoSave);
    m_editorWidget->postSave();

    return result;
}

bool ProjectDocument::isModified() const
{
    return TextDocument::isModified() ||  m_editorWidget->isModified();
}

bool ProjectDocument::isSaveAsAllowed() const
{
    return false;
}

} // namespace Internal
} // namespace PHPQt5
