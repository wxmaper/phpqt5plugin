#ifndef PHPQT5BUILDCONFIURATIONFACTORY_H
#define PHPQT5BUILDCONFIURATIONFACTORY_H

#include <projectexplorer/buildconfiguration.h>

namespace PHPQt5 {

class PHPQt5BuildConfigurationFactory : public ProjectExplorer::IBuildConfigurationFactory
{
    Q_OBJECT

public:
    explicit PHPQt5BuildConfigurationFactory(QObject *parent = 0);

    QList<ProjectExplorer::BuildInfo *> availableBuilds(const ProjectExplorer::Target *parent) const override;

    QList<ProjectExplorer::BuildInfo *> availableSetups(const ProjectExplorer::Kit *k,
                                                        const QString &projectPath) const override;

    ProjectExplorer::BuildConfiguration *create(ProjectExplorer::Target *parent,
                                                const ProjectExplorer::BuildInfo *info) const override;

    bool canRestore(const ProjectExplorer::Target *parent,
                    const QVariantMap &map) const override;

    ProjectExplorer::BuildConfiguration *restore(ProjectExplorer::Target *parent,
                                                 const QVariantMap &map) override;

    bool canClone(const ProjectExplorer::Target *parent,
                  ProjectExplorer::BuildConfiguration *product) const override;

    ProjectExplorer::BuildConfiguration *clone(ProjectExplorer::Target *parent,
                                               ProjectExplorer::BuildConfiguration *product) override;

    int priority(const ProjectExplorer::Kit *k, const QString &projectPath) const override;

    int priority(const ProjectExplorer::Target *parent) const override;

private:
    bool canHandle(const ProjectExplorer::Target *t) const;
    static Utils::FileName defaultBuildDirectory(const ProjectExplorer::Kit *k,
                                                 const QString &projectPath,
                                                 const QString &bc, ProjectExplorer::BuildConfiguration::BuildType buildType);

    ProjectExplorer::BuildInfo *createBuildInfo(const ProjectExplorer::Kit *k,
                                                const QString &projectPath,
                                                ProjectExplorer::BuildConfiguration::BuildType buildType) const;
};

}


#endif // PHPQT5BUILDCONFIURATIONFACTORY_H
