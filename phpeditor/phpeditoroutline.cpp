#include "phpeditordocument.h"
#include "phpeditoroutline.h"
#include "phpoutline.h"
#include "phpoverviewmodel.h"

#include <texteditor/texteditor.h>
#include <texteditor/textdocument.h>
#include <coreplugin/editormanager/editormanager.h>

#include "../parser/scanner.h"
#include "../parser/astnode.h"

#include <cplusplus/OverviewModel.h>
#include <utils/treeviewcombobox.h>

#include <QAction>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QVBoxLayout>

enum { UpdateOutlineIntervalInMs = 500 };

namespace PHPQt5 {
namespace Internal {

QTimer *newSingleShotTimer(QObject *parent, int msInternal, const QString &objectName)
{
    //qDebug() <<Q_FUNC_INFO;
    QTimer *timer = new QTimer(parent);
    timer->setObjectName(objectName);
    timer->setSingleShot(true);
    timer->setInterval(msInternal);
    return timer;
}

PHPEditorOutline::PHPEditorOutline(TextEditor::TextEditorWidget *editorWidget)
    : QObject(editorWidget)
    , m_editorWidget(editorWidget)
    , m_combo(new Utils::TreeViewComboBox)
    , m_model(new PHPOverviewModel(this))
    , m_proxyModel(new OverviewProxyModel(m_model, this))
{
    //qDebug() <<Q_FUNC_INFO;
    // Set up proxy model
    /*
    if (CppTools::CppToolsSettings::instance()->sortedEditorDocumentOutline())
        m_proxyModel->sort(0, Qt::AscendingOrder);
    else
        m_proxyModel->sort(-1, Qt::AscendingOrder); // don't sort yet, but set column for sortedOutline()
    */
    m_proxyModel->setDynamicSortFilter(true);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    // Set up combo box
    m_combo->setModel(m_proxyModel);

    m_combo->setMinimumContentsLength(22);
    QSizePolicy policy = m_combo->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    m_combo->setSizePolicy(policy);
    m_combo->setMaxVisibleItems(40);

    m_combo->setContextMenuPolicy(Qt::ActionsContextMenu);
    /*
    m_sortAction = new QAction(tr("Sort Alphabetically"), m_combo);
    m_sortAction->setCheckable(true);
    m_sortAction->setChecked(isSorted());
    connect(m_sortAction, &QAction::toggled,
            CppTools::CppToolsSettings::instance(),
            &CppTools::CppToolsSettings::setSortedEditorDocumentOutline);

    m_combo->addAction(m_sortAction);
*/

    connect(m_combo, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this, &PHPEditorOutline::gotoSymbolInEditor);
    connect(m_combo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PHPEditorOutline::updateToolTip);

    // Set up timers
    m_updateTimer = newSingleShotTimer(this, UpdateOutlineIntervalInMs,
                                       QLatin1String("CppEditorOutline::m_updateTimer"));
    connect(m_updateTimer, &QTimer::timeout, this, &PHPEditorOutline::updateNow);

    m_updateIndexTimer = newSingleShotTimer(this, UpdateOutlineIntervalInMs,
                                            QLatin1String("CppEditorOutline::m_updateIndexTimer"));
    connect(m_updateIndexTimer, &QTimer::timeout, this, &PHPEditorOutline::updateIndexNow);
}

void PHPEditorOutline::update()
{
    //qDebug() <<Q_FUNC_INFO;
    m_updateTimer->start();
}

bool PHPEditorOutline::isSorted() const
{
    //qDebug() <<Q_FUNC_INFO;
    return m_proxyModel->sortColumn() == 0;
}

void PHPEditorOutline::setSorted(bool sort)
{
    //qDebug() <<Q_FUNC_INFO;
    if (sort != isSorted()) {
        if (sort)
            m_proxyModel->sort(0, Qt::AscendingOrder);
        else
            m_proxyModel->sort(-1, Qt::AscendingOrder);
        bool block = m_sortAction->blockSignals(true);
        m_sortAction->setChecked(m_proxyModel->sortColumn() == 0);
        m_sortAction->blockSignals(block);
        updateIndexNow();
    }
}

PHPOverviewModel *PHPEditorOutline::model() const
{
    //qDebug() <<Q_FUNC_INFO;
    return m_model;
}

QModelIndex PHPEditorOutline::modelIndex()
{
    //qDebug() << Q_FUNC_INFO;
    if (!m_modelIndex.isValid()) {
        //int line = 0, column = 0;
        //m_editorWidget->convertPosition(m_editorWidget->position(), &line, &column);
        m_modelIndex = indexForPosition(m_editorWidget->position());
        emit modelIndexChanged(m_modelIndex);
    }

    return m_modelIndex;
}

QWidget *PHPEditorOutline::widget() const
{
    //qDebug() <<Q_FUNC_INFO;
    return m_combo;
}

void PHPEditorOutline::updateNow()
{
    //qDebug() << Q_FUNC_INFO;

    PHPEditorDocument *doc = qobject_cast<PHPEditorDocument*>(m_editorWidget->textDocument());
    if (doc != Q_NULLPTR) {
        if (_scanner) {
            m_model->clear();
            delete _scanner;
            _scanner = 0;
        }

        _scanner = new Scanner(doc->document()->toPlainText(),
                               doc->document()->toPlainText().length(),
                               Scanner::State_Default,
                               false);
        Token tk;

        while ((tk = _scanner->read()).type() != Token::T_EOF) {
        }

        m_model->setRootNode(_scanner->neatNode());
    }

    m_combo->view()->expandAll();
    updateIndexNow();
}

void PHPEditorOutline::updateIndex()
{
    //qDebug() <<Q_FUNC_INFO;
    m_updateIndexTimer->start();
}

void PHPEditorOutline::updateIndexNow()
{
    //qDebug() <<Q_FUNC_INFO;
    /*
    if (!m_model->document())
        return;

    const unsigned revision = m_editorWidget->document()->revision();
    if (m_model->document()->editorRevision() != revision) {
        m_updateIndexTimer->start();
        return;
    }

    m_updateIndexTimer->stop();

    m_modelIndex = QModelIndex(); //invalidate
    QModelIndex comboIndex = modelIndex();

    if (comboIndex.isValid()) {
        bool blocked = m_combo->blockSignals(true);
        m_combo->setCurrentIndex(m_proxyModel->mapFromSource(comboIndex));
        updateToolTip();
        m_combo->blockSignals(blocked);
    }
    */

    //qDebug() << Q_FUNC_INFO;

    m_updateIndexTimer->stop();

    int pos = m_editorWidget->position();

    QModelIndex rootIndex = QModelIndex();
    QModelIndex index = indexForPosition(pos, rootIndex);

    PHPOutlineWidget *outlineWidget = qobject_cast<PHPEditorWidget*>(m_editorWidget)->outlineWidget();

    /*
    PHPOutlineTreeView *treeView = outlineWidget->treeView();
    bool blocked = treeView->blockSignals(true);
    treeView->setCurrentIndex(index);
    treeView->blockSignals(blocked);
   */

    bool blocked = m_combo->blockSignals(true);
    m_combo->setCurrentIndex(m_proxyModel->mapFromSource(index));
    m_combo->blockSignals(blocked);
    m_combo->view()->expandAll();
}

void PHPEditorOutline::updateToolTip()
{
    //qDebug() <<Q_FUNC_INFO;
    m_combo->setToolTip(m_combo->currentText());
}

void PHPEditorOutline::gotoSymbolInEditor()
{
    //qDebug() <<Q_FUNC_INFO;
    /*
    const QModelIndex modelIndex = m_combo->view()->currentIndex();
    const QModelIndex sourceIndex = m_proxyModel->mapToSource(modelIndex);

    CPlusPlus::Symbol *symbol = m_model->symbolFromIndex(sourceIndex);
    if (!symbol)
        return;

    const TextEditor::TextEditorWidget::Link &link = CppTools::linkToSymbol(symbol);
    if (!link.hasValidTarget())
        return;

    Core::EditorManager::cutForwardNavigationHistory();
    Core::EditorManager::addCurrentPositionToNavigationHistory();
    m_editorWidget->gotoLine(link.targetLine, link.targetColumn);
    m_editorWidget->activateEditor();
    */
    const QModelIndex modelIndex = m_combo->view()->currentIndex();
    const QModelIndex sourceIndex = m_proxyModel->mapToSource(modelIndex);
    NeatNode *node = static_cast<NeatNode*>(sourceIndex.internalPointer());

    if (!node)
        return;

    int line = 0;
    int column = 0;
    m_editorWidget->convertPosition(node->node()->nameToken().begin(), &line, &column);
    m_editorWidget->gotoLine(line, column);
    m_editorWidget->activateEditor();
}

QModelIndex PHPEditorOutline::indexForPosition(int position,
                                               const QModelIndex &rootIndex) const
{
    //qDebug() << Q_FUNC_INFO;
    QModelIndex lastIndex = rootIndex;

    const int rowCount = m_model->rowCount(rootIndex);
    for (int row = 0; row < rowCount; ++row) {
        const QModelIndex index = m_model->index(row, 0, rootIndex);
        NeatNode *node = static_cast<NeatNode*>(index.internalPointer());
        //qDebug() << "!!!!!!!!!!!!!!!" << node->node() << node->node()->token().begin() << position;
        if (node->node()->token().begin() > position)
            break;
        lastIndex = index;
    }

    if (lastIndex != rootIndex) {
        // recurse
        lastIndex = indexForPosition(position, lastIndex);
    }

    return lastIndex;
}

} // namespace Internal
} // namespace PHPQt5
