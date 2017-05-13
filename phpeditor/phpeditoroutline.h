#ifndef PHPEDITOROUTLINE_H
#define PHPEDITOROUTLINE_H

#include "phpoverviewmodel.h"

#include <QModelIndex>
#include <QObject>
#include <QSortFilterProxyModel>

class QAction;
class QTimer;

namespace TextEditor { class TextEditorWidget; }
namespace Utils { class TreeViewComboBox; }

namespace PHPQt5 {
namespace Internal {
class Scanner;

class OverviewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    OverviewProxyModel(PHPOverviewModel *sourceModel, QObject *parent)
        : QSortFilterProxyModel(parent)
        , m_sourceModel(sourceModel)
    {
        setSourceModel(m_sourceModel);
    }

    bool filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const
    {
        // Ignore generated symbols, e.g. by macro expansion (Q_OBJECT)
        /*
        const QModelIndex sourceIndex = m_sourceModel->index(sourceRow, 0, sourceParent);
        CPlusPlus::Symbol *symbol = m_sourceModel->symbolFromIndex(sourceIndex);
        if (symbol && symbol->isGenerated())
            return false;
        */

        return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }
private:
    PHPOverviewModel *m_sourceModel;
};

class PHPEditorOutline : public QObject
{
    Q_OBJECT

public:
    explicit PHPEditorOutline(TextEditor::TextEditorWidget *editorWidget);

    void update();

    PHPOverviewModel *model() const;
    QModelIndex modelIndex();

    QWidget *widget() const; // Must be deleted by client.

signals:
    void modelIndexChanged(const QModelIndex &index);

public slots:
    void updateIndex();
    void setSorted(bool sort);

private:
    void updateNow();
    void updateIndexNow();
    void updateToolTip();
    void gotoSymbolInEditor();

    PHPEditorOutline();

    bool isSorted() const;
    QModelIndex indexForPosition(int position,
                                 const QModelIndex &rootIndex = QModelIndex()) const;

private:
    TextEditor::TextEditorWidget *m_editorWidget;

    Utils::TreeViewComboBox *m_combo; // Not owned
    PHPOverviewModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    QModelIndex m_modelIndex;
    QAction *m_sortAction;
    QTimer *m_updateTimer;
    QTimer *m_updateIndexTimer;

    Scanner *_scanner = Q_NULLPTR;
};

} // namespace Internal
} // namespace PHPQt5

#endif // PHPEDITOROUTLINE_H
