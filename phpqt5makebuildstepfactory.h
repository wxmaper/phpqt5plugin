#ifndef PHPQT5MAKEBUILDSTEPFACTORY_H
#define PHPQT5MAKEBUILDSTEPFACTORY_H

#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5MakeBuildStepFactory : public ProjectExplorer::IBuildStepFactory
{
    Q_OBJECT

public:
    PHPQt5MakeBuildStepFactory(QObject *parent = nullptr);

    QList<ProjectExplorer::BuildStepInfo>
        availableSteps(ProjectExplorer::BuildStepList *parent) const override;

    ProjectExplorer::BuildStep *create(ProjectExplorer::BuildStepList *parent, Core::Id id) override;
    ProjectExplorer::BuildStep *clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *product) override;
};

}


#endif // PHPQT5MAKEBUILDSTEPFACTORY_H
