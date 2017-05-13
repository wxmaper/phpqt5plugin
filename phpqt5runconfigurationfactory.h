#ifndef PHPQT5RUNCONFIGURATIONFACTORY_H
#define PHPQT5RUNCONFIGURATIONFACTORY_H

#include <projectexplorer/runconfiguration.h>

namespace PHPQt5 {

class PHPQt5RunConfigurationFactory : public ProjectExplorer::IRunConfigurationFactory
{
public:
    PHPQt5RunConfigurationFactory();

    QList<Core::Id> availableCreationIds(ProjectExplorer::Target *parent, CreationMode mode) const override;

    QString displayNameForId(Core::Id id) const override;

    bool canCreate(ProjectExplorer::Target *parent, Core::Id id) const override;

    bool canRestore(ProjectExplorer::Target *parent,
                    const QVariantMap &map) const override;

    bool canClone(ProjectExplorer::Target *parent,
                  ProjectExplorer::RunConfiguration *product) const override;

    ProjectExplorer::RunConfiguration *clone(ProjectExplorer::Target *parent,
                                             ProjectExplorer::RunConfiguration *product) override;

private:
    bool canHandle(ProjectExplorer::Target *parent) const;

    ProjectExplorer::RunConfiguration *doCreate(ProjectExplorer::Target *parent, Core::Id id) override;
    ProjectExplorer::RunConfiguration *doRestore(ProjectExplorer::Target *parent, const QVariantMap &map) override;
};

}


#endif // PHPQT5RUNCONFIGURATIONFACTORY_H
