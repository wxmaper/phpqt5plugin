#include "phpoutline.h"
#include "phpeditor.h"
#include "phpeditoroutline.h"

#include <coreplugin/find/itemviewfind.h>
#include <coreplugin/editormanager/editormanager.h>
#include <utils/qtcassert.h>

#include <QDebug>
#include <QVBoxLayout>
#include <QMenu>

namespace PHPQt5 {
namespace Internal {

PHPOutlineTreeView::PHPOutlineTreeView(QWidget *parent) :
    Utils::NavigationTreeView(parent)
{
    ////qDebug() << Q_FUNC_INFO;
    setExpandsOnDoubleClick(false);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
}

void PHPOutlineTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    ////qDebug() << Q_FUNC_INFO;
    if (!event)
        return;

    QMenu contextMenu;

    QAction *action = contextMenu.addAction(tr("Expand All"));
    connect(action, &QAction::triggered, this, &QTreeView::expandAll);
    action = contextMenu.addAction(tr("Collapse All"));
    connect(action, &QAction::triggered, this, &QTreeView::collapseAll);

    contextMenu.exec(event->globalPos());

    event->accept();
}

PHPOutlineFilterModel::PHPOutlineFilterModel(PHPOverviewModel *sourceModel, QObject *parent) :
    QSortFilterProxyModel(parent),
    m_sourceModel(sourceModel)
{
    ////qDebug() << Q_FUNC_INFO;
    setSourceModel(m_sourceModel);
}

bool PHPOutlineFilterModel::filterAcceptsRow(int sourceRow,
                                             const QModelIndex &sourceParent) const
{
    ////qDebug() << Q_FUNC_INFO;
    // ignore artifical "<Select Symbol>" entry
    if (!sourceParent.isValid() && sourceRow == 0)
        return false;
    // ignore generated symbols, e.g. by macro expansion (Q_OBJECT)
    /*
    const QModelIndex sourceIndex = m_sourceModel->index(sourceRow, 0, sourceParent);
    CPlusPlus::Symbol *symbol = m_sourceModel->symbolFromIndex(sourceIndex);
    if (symbol && symbol->isGenerated())
        return false;
    */

    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

Qt::DropActions PHPOutlineFilterModel::supportedDragActions() const
{
    ////qDebug() << Q_FUNC_INFO;
    return sourceModel()->supportedDragActions();
}

PHPOutlineWidget::PHPOutlineWidget(PHPEditorWidget *editor) :
    TextEditor::IOutlineWidget(),
    m_editor(editor),
    m_treeView(new PHPOutlineTreeView(this)),
    m_model(m_editor->outline()->model()),
    m_proxyModel(new PHPOutlineFilterModel(m_model, this)),
    m_enableCursorSync(true),
    m_blockCursorSync(false)
{
    ////qDebug() << Q_FUNC_INFO;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(Core::ItemViewFind::createSearchableWrapper(m_treeView));
    setLayout(layout);

    m_editor->setOutlineWidget(this);

    m_treeView->setModel(m_proxyModel);
    setFocusProxy(m_treeView);

    connect(m_model, &QAbstractItemModel::modelReset,
            this, &PHPOutlineWidget::modelUpdated);
    modelUpdated();

    connect(m_editor->outline(), &PHPEditorOutline::modelIndexChanged,
            this, &PHPOutlineWidget::updateSelectionInTree);
    connect(m_treeView, &QAbstractItemView::activated,
            this, &PHPOutlineWidget::onItemActivated);
}

QList<QAction*> PHPOutlineWidget::filterMenuActions() const
{
    ////qDebug() << Q_FUNC_INFO;
    return QList<QAction*>();
}

void PHPOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
    ////qDebug() << Q_FUNC_INFO;
    m_enableCursorSync = syncWithCursor;
    if (m_enableCursorSync)
        updateSelectionInTree(m_editor->outline()->modelIndex());
}

PHPOutlineTreeView *PHPOutlineWidget::treeView()
{
    ////qDebug() << Q_FUNC_INFO;
    return m_treeView;
}

void PHPOutlineWidget::modelUpdated()
{
    ////qDebug() << Q_FUNC_INFO;
    m_treeView->expandAll();
}

void PHPOutlineWidget::updateSelectionInTree(const QModelIndex &index)
{
    //qDebug() << Q_FUNC_INFO;
    if (!syncCursor())
        return;

    if (index.isValid())
    {
        //qDebug() << "mapFromSource" << index;
        QModelIndex proxyIndex = m_proxyModel->mapFromSource(index);

        m_blockCursorSync = true;
        //qDebug() << "CppOutline - updating selection due to cursor move";

        if (proxyIndex.isValid()) {
            m_treeView->setCurrentIndex(proxyIndex);
            m_treeView->scrollTo(proxyIndex);
        }
        m_blockCursorSync = false;
    }
}

void PHPOutlineWidget::updateTextCursor(const QModelIndex &proxyIndex)
{
    ////qDebug() <<Q_FUNC_INFO;
    QModelIndex index = m_proxyModel->mapToSource(proxyIndex);

    /*
    CPlusPlus::Symbol *symbol = m_model->symbolFromIndex(index);
    if (symbol) {
        m_blockCursorSync = true;

        Core::EditorManager::cutForwardNavigationHistory();
        Core::EditorManager::addCurrentPositionToNavigationHistory();

        // line has to be 1 based, column 0 based!
        m_editor->gotoLine(symbol->line(), symbol->column() - 1);
        m_blockCursorSync = false;
    }
    */
}

void PHPOutlineWidget::onItemActivated(const QModelIndex &index)
{
    ////qDebug() <<Q_FUNC_INFO;
    if (!index.isValid())
        return;

    updateTextCursor(index);
    m_editor->setFocus();
}

bool PHPOutlineWidget::syncCursor()
{
    //qDebug() <<Q_FUNC_INFO;
    return m_enableCursorSync && !m_blockCursorSync;
}

bool PHPOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
    ////qDebug() <<Q_FUNC_INFO;
    if (qobject_cast<PHPEditor*>(editor))
        return true;
    return false;
}

TextEditor::IOutlineWidget *PHPOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
    ////qDebug() <<Q_FUNC_INFO;
    PHPEditor *phpEditor = qobject_cast<PHPEditor*>(editor);
    PHPEditorWidget *phpEditorWidget = qobject_cast<PHPEditorWidget*>(phpEditor->widget());
    QTC_ASSERT(phpEditorWidget, return 0);

    PHPOutlineWidget *widget = new PHPOutlineWidget(phpEditorWidget);

    return widget;
}

} // namespace Internal
} // namespace PHPQt5
