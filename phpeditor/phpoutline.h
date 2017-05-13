#ifndef PHPOUTLINE_H
#define PHPOUTLINE_H

#include "phpeditor.h"
#include "phpoverviewmodel.h"

#include <texteditor/ioutlinewidget.h>
#include <utils/navigationtreeview.h>
#include <QSortFilterProxyModel>

namespace PHPQt5 {
namespace Internal {

class PHPOutlineTreeView : public Utils::NavigationTreeView
{
    Q_OBJECT
public:
    PHPOutlineTreeView(QWidget *parent);

    void contextMenuEvent(QContextMenuEvent *event);
};

class PHPOutlineFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    PHPOutlineFilterModel(PHPOverviewModel *sourceModel, QObject *parent);
    // QSortFilterProxyModel
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const;
    Qt::DropActions supportedDragActions() const;
private:
    PHPOverviewModel *m_sourceModel;
};

class PHPOutlineWidget : public TextEditor::IOutlineWidget
{
    Q_OBJECT
public:
    PHPOutlineWidget(PHPEditorWidget *editor);

    // IOutlineWidget
    virtual QList<QAction*> filterMenuActions() const;
    virtual void setCursorSynchronization(bool syncWithCursor);

    PHPOutlineTreeView *treeView();

private:
    void modelUpdated();
    void updateSelectionInTree(const QModelIndex &index);
    void updateTextCursor(const QModelIndex &index);
    void onItemActivated(const QModelIndex &index);
    bool syncCursor();

private:
    PHPEditorWidget *m_editor;
    PHPOutlineTreeView *m_treeView;
    PHPOverviewModel *m_model;
    PHPOutlineFilterModel *m_proxyModel;

    bool m_enableCursorSync;
    bool m_blockCursorSync;
};

class PHPOutlineWidgetFactory : public TextEditor::IOutlineWidgetFactory
{
    Q_OBJECT

public:
    bool supportsEditor(Core::IEditor *editor) const;
    TextEditor::IOutlineWidget *createWidget(Core::IEditor *editor);
};

} // namespace Internal
} // namespace PHPQt5

#endif // PHPOUTLINE_H
