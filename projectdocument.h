#ifndef PROJECTDOCUMENT_H
#define PROJECTDOCUMENT_H

#include <texteditor/textdocument.h>
#include <qmljseditor/qmljseditordocument.h>

namespace PHPQt5 {
namespace Internal {

class ProjectEditorWidget;

class ProjectDocument : public QmlJSEditor::QmlJSEditorDocument
{
public:
    explicit ProjectDocument(ProjectEditorWidget *editorWidget);
    bool save(QString *errorString, const QString &fileName = QString(),
              bool autoSave = false) override;

    bool isModified() const override;
    bool isSaveAsAllowed() const override;

private:
    ProjectEditorWidget *m_editorWidget;
};

} // namespace Internal
} // namespace PHPQt5

#endif // PROJECTDOCUMENT_H
