#include "phpeditor.h"
#include "phpeditordocument.h"
#include "phpeditoroutline.h"

#include "../parser/astnode.h"
#include <texteditor/textdocument.h>

#include <utils/annotateditemdelegate.h>

#include <QComboBox>
#include <QCoreApplication>
#include <QFileInfo>
#include <QHeaderView>
#include <QMenu>
#include <QMetaMethod>
#include <QPointer>
#include <QScopedPointer>
#include <QTextCodec>
#include <QTimer>
#include <QTreeView>

namespace PHPQt5 {
namespace Internal {

PHPEditor::PHPEditor()
    : TextEditor::BaseTextEditor()
{
    //qDebug() << Q_FUNC_INFO;
}

PHPEditorWidget::PHPEditorWidget()
    : m_phpEditorDocument(qobject_cast<PHPEditorDocument *>(textDocument())),
      m_phpEditorOutline(new PHPEditorOutline(this))
{
   // qDebug() << Q_FUNC_INFO;
}

PHPEditorDocument *PHPEditorWidget::phpEditorDocument() const
{
    return m_phpEditorDocument;
}

PHPEditorOutline *PHPEditorWidget::outline() const
{
    return m_phpEditorOutline;
}

PHPOutlineWidget *PHPEditorWidget::outlineWidget() const
{
    return m_phpOutlineWidget;
}

void PHPEditorWidget::setOutlineWidget(PHPOutlineWidget *outlineWidget)
{
    m_phpOutlineWidget = outlineWidget;
}

void PHPEditorWidget::finalizeInitialization()
{
    textDocument()->setCodec(QTextCodec::codecForName("UTF-8")); // php files are defined to be utf-8
    m_phpEditorDocument = qobject_cast<PHPEditorDocument *>(textDocument());

    connect(this, &QPlainTextEdit::cursorPositionChanged,
            m_phpEditorOutline, &PHPEditorOutline::updateIndex);
    connect(this, &QPlainTextEdit::textChanged,
            m_phpEditorOutline, &PHPEditorOutline::update);

    // не работает :-(
    insertExtraToolBarWidget(TextEditorWidget::Left, m_phpEditorOutline->widget());
    
}


}
}
