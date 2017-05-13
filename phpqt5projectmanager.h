#ifndef PHPPROJECTMANAGER_H
#define PHPPROJECTMANAGER_H

#include <projectexplorer/iprojectmanager.h>

namespace PHPQt5 {

class PHPQt5ProjectManager : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    QString mimeType() const override;

    ProjectExplorer::Project *openProject(const QString &fileName, QString *errorString) override;
};

}

#endif // PHPPROJECTMANAGER_H
