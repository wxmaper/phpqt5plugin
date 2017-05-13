#ifndef PHPQT5JOMBUILDSTEPFACTORY_H
#define PHPQT5JOMBUILDSTEPFACTORY_H

#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5JomBuildStepFactory : public ProjectExplorer::IBuildStepFactory
{
    Q_OBJECT

public:
    PHPQt5JomBuildStepFactory(QObject *parent = nullptr);

    QList<ProjectExplorer::BuildStepInfo>
        availableSteps(ProjectExplorer::BuildStepList *parent) const override;

    ProjectExplorer::BuildStep *create(ProjectExplorer::BuildStepList *parent, Core::Id id) override;
    ProjectExplorer::BuildStep *clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *product) override;
};

}


#endif // PHPQT5JOMBUILDSTEPFACTORY_H
