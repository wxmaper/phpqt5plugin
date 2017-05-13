#ifndef PHPQT5UICBUILDSTEPFACTORY_H
#define PHPQT5UICBUILDSTEPFACTORY_H

#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5UicBuildStepFactory : public ProjectExplorer::IBuildStepFactory
{
    Q_OBJECT

public:
    PHPQt5UicBuildStepFactory(QObject *parent = nullptr);

    QList<ProjectExplorer::BuildStepInfo>
        availableSteps(ProjectExplorer::BuildStepList *parent) const override;

    ProjectExplorer::BuildStep *create(ProjectExplorer::BuildStepList *parent, Core::Id id) override;
    ProjectExplorer::BuildStep *clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *product) override;
};

}


#endif // PHPQT5UICBUILDSTEPFACTORY_H
