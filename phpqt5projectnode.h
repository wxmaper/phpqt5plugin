#ifndef PHPQT5PROJECTNODE_H
#define PHPQT5PROJECTNODE_H

#include <projectexplorer/projectnodes.h>

namespace Utils { class FileName; }

namespace PHPQt5 {

class PHPQt5Project;

class PHPQt5ProjectNode : public ProjectExplorer::ProjectNode
{
public:
    PHPQt5ProjectNode(const Utils::FileName &projectFilePath);

    QList<ProjectExplorer::ProjectAction> supportedActions(Node *node) const override;
    bool canAddSubProject(const QString &) const override;
    bool addSubProjects(const QStringList &) override;
    bool removeSubProjects(const QStringList &) override;
    bool addFiles(const QStringList &, QStringList *) override;
    bool removeFiles(const QStringList &, QStringList *) override;
    bool deleteFiles(const QStringList &) override;
    bool renameFile(const QString &, const QString &) override;
};

}

#endif // PHPQT5PROJECTNODE_H
