#ifndef PHPEDITOR_H
#define PHPEDITOR_H

#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>

#include <QComboBox>

namespace PHPQt5 {
namespace Internal {

class PHPEditorDocument;
class PHPEditorOutline;
class PHPOutlineWidget;

class PHPEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    PHPEditor();
};

class PHPEditorWidget : public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    PHPEditorWidget();

    PHPEditorDocument *phpEditorDocument() const;
    PHPEditorOutline *outline() const;
    PHPOutlineWidget *outlineWidget() const;

    void setOutlineWidget(PHPOutlineWidget* outlineWidget);

    void finalizeInitialization() override;

private:
    PHPEditorDocument *m_phpEditorDocument;
    PHPEditorOutline *m_phpEditorOutline;
    PHPOutlineWidget *m_phpOutlineWidget;
};


}
}

#endif // PHPEDITOR_H
