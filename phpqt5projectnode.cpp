#include "phpqt5projectnode.h"
#include "phpqt5project.h"

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5ProjectNode::PHPQt5ProjectNode(const FileName &projectFilePath)
    : ProjectNode(projectFilePath)
{
}

QList<ProjectAction> PHPQt5ProjectNode::supportedActions(Node *node) const
{
    static const QList<ProjectAction> fileActions = { ProjectAction::Rename,
                                                      ProjectAction::RemoveFile
                                                    };
    static const QList<ProjectAction> folderActions = { ProjectAction::AddNewFile,
                                                        ProjectAction::RemoveFile
                                                      };
    switch (node->nodeType()) {
    case FileNodeType:
        return fileActions;
    case FolderNodeType:
    case ProjectNodeType:
        return folderActions;
    default:
        return ProjectNode::supportedActions(node);
    }
}

bool PHPQt5ProjectNode::addSubProjects(const QStringList &)
{
    return false;
}

bool PHPQt5ProjectNode::canAddSubProject(const QString &) const
{
    return false;
}

bool PHPQt5ProjectNode::removeSubProjects(const QStringList &)
{
    return false;
}

bool PHPQt5ProjectNode::addFiles(const QStringList &, QStringList *)
{
    return true;
}

bool PHPQt5ProjectNode::removeFiles(const QStringList &, QStringList *)
{
    return true;
}

bool PHPQt5ProjectNode::deleteFiles(const QStringList &)
{
    return true;
}

bool PHPQt5ProjectNode::renameFile(const QString &, const QString &)
{
    return true;
}

}
