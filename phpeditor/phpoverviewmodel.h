#ifndef PHPOVERVIEWMODEL_H
#define PHPOVERVIEWMODEL_H

#include <QAbstractItemModel>

namespace PHPQt5 {
namespace Internal {

class NeatNode;
class PHPOverviewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Columns
    {
        RamificationColumn, // столбец, по которому производится ветвление, всегда первый.
                            // Другого варианта не поддерживает QTreeView
        //NameColumn = RamificationColumn, // столбец с именем узла
        //ModificationDateColumn, // столбец с датой изменения узла
        //SizeColumn, // столбец с размером файла
        ColumnCount // число столбцов
    };

    PHPOverviewModel(QObject *parent = 0);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int findRow(const NeatNode *nodeInfo) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setRootNode(NeatNode *node);
    void clear();

signals:
    void updated();

private:
    NeatNode *_rootNode = Q_NULLPTR; // список корневых узлов
};

} // namespace Internal
} // namespace PHPQt5

#endif // PHPOVERVIEWMODEL_H
