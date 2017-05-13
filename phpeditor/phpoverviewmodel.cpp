#include "phpoverviewmodel.h"
#include "../parser/astnode.h"

#include <utils/qtcassert.h>
#include <libs/cplusplus/Icons.h>

namespace PHPQt5 {
namespace Internal {

PHPOverviewModel::PHPOverviewModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QModelIndex PHPOverviewModel::index(int row, int column, const QModelIndex &parent) const
{
    ////qDebug() << Q_FUNC_INFO;
    if (!_rootNode || !hasIndex(row, column, parent))
        return QModelIndex();

    NeatNode *parentNode;

    if (!parent.isValid())
        parentNode = _rootNode;
    else
        parentNode = static_cast<NeatNode*>(parent.internalPointer());

    if (parentNode && !parentNode->dtor()) {
        NeatNode *childNode = parentNode->nodes().at(row);
        if (childNode && !childNode->dtor())
            return createIndex(row, column, childNode);
        else
            return QModelIndex();
    }
}

QModelIndex PHPOverviewModel::parent(const QModelIndex &index) const
{
    //qDebug() << Q_FUNC_INFO;
    if (!index.isValid())
        return QModelIndex();

    NeatNode *childNode = static_cast<NeatNode*>(index.internalPointer());

    if (childNode && !childNode->dtor()) {
        NeatNode *parentNode = childNode->parent();

        if (!parentNode || parentNode == _rootNode || parentNode->dtor())
            return QModelIndex();

        return createIndex(findRow(parentNode), 0, parentNode);
    }

    return QModelIndex();
}

int PHPOverviewModel::rowCount(const QModelIndex &parent) const
{
    //qDebug() << Q_FUNC_INFO;
    int rowCount = 0;
    if (!_rootNode)
        return 0;

    NeatNode *parentNode;

    if (!parent.isValid())
        parentNode = _rootNode;
    else
        parentNode = static_cast<NeatNode*>(parent.internalPointer());

    if (parentNode && !parentNode->dtor())
        rowCount = parentNode->nodes().size();

    return rowCount;
}

int PHPOverviewModel::columnCount(const QModelIndex &parent) const
{
    //qDebug() << Q_FUNC_INFO;
    return 1;
}

int PHPOverviewModel::findRow(const NeatNode *node) const
{
    //qDebug() << Q_FUNC_INFO;
    int idx = 0;
    if (node && !node->dtor()) {
        //qDebug() << "parentNode";
        NeatNode *parentNode = node->parent() ? node->parent() : _rootNode;

        if (parentNode && !parentNode->dtor()) {
            //qDebug() << "QVectorIterator";
            //qDebug() << "QVectorIterator" << parentNode;
            //qDebug() << "QVectorIterator" << parentNode->parent();
            //qDebug() << "QVectorIterator" << parentNode;
            QVectorIterator<NeatNode*> i(parentNode->nodes());

            //qDebug() << "hasNext";
            for (; i.hasNext(); idx++) {
                if (i.next() == node) break;
            }
        }
    }

    return idx;
}

QVariant PHPOverviewModel::data(const QModelIndex &index, int role) const
{
    //qDebug() << Q_FUNC_INFO;
    if (!index.isValid())
        return QVariant();

    NeatNode *node = static_cast<NeatNode*>(index.internalPointer());

    if (node && !node->dtor()) {
        switch (role) {
        case Qt::DisplayRole:
            return node->node()->type() == ASTNode::FunctionDeclaration
                    ? QString("%1(%2)")
                      .arg(node->node()->nameToken().value())
                      .arg(node->node()->args())
                    : QString("%1")
                      .arg(node->node()->nameToken().value());

        case Qt::DecorationRole:
            if (node->node()->type() == ASTNode::FunctionDeclaration) {
                if (node->node()->dTypeFlags().testFlag(ASTNode::Static)) {
                    if (node->node()->dTypeFlags().testFlag(ASTNode::Public))
                        return CPlusPlus::Icons::iconForType(CPlusPlus::Icons::FuncPublicStaticIconType);
                    if (node->node()->dTypeFlags().testFlag(ASTNode::Private))
                        return CPlusPlus::Icons::iconForType(CPlusPlus::Icons::FuncPrivateStaticIconType);
                    if (node->node()->dTypeFlags().testFlag(ASTNode::Protected))
                        return CPlusPlus::Icons::iconForType(CPlusPlus::Icons::FuncProtectedStaticIconType);
                }
                else {
                    if (node->node()->dTypeFlags().testFlag(ASTNode::Public))
                        return CPlusPlus::Icons::iconForType(CPlusPlus::Icons::FuncPublicIconType);
                    if (node->node()->dTypeFlags().testFlag(ASTNode::Private))
                        return CPlusPlus::Icons::iconForType(CPlusPlus::Icons::FuncPrivateIconType);
                    if (node->node()->dTypeFlags().testFlag(ASTNode::Protected))
                        return CPlusPlus::Icons::iconForType(CPlusPlus::Icons::FuncProtectedIconType);
                }
            }

            else if (node->node()->type() == ASTNode::ClassDeclaration) {
                return CPlusPlus::Icons::iconForType(CPlusPlus::Icons::ClassIconType);
            }

            else if (node->node()->type() == ASTNode::PHPDocument) {
                return CPlusPlus::Icons::iconForType(CPlusPlus::Icons::NamespaceIconType);
            }

            else return QVariant();

        default:
            return QVariant();
        }
    }
}

void PHPOverviewModel::setRootNode(NeatNode *node)
{
    beginResetModel();
    clear();
    //qDebug() << Q_FUNC_INFO;
    _rootNode = node;
    endResetModel();

    emit updated();
}

void PHPOverviewModel::clear()
{
    beginResetModel();
    _rootNode = Q_NULLPTR;
    resetInternalData();
    endResetModel();
}

} // namespace Internal
} // namespace PHPQt5
