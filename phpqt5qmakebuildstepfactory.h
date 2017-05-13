#ifndef PHPQT5QMAKEBUILDSTEPFACTORY_H
#define PHPQT5QMAKEBUILDSTEPFACTORY_H


#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5QMakeBuildStepFactory : public ProjectExplorer::IBuildStepFactory
{
    Q_OBJECT

public:
    PHPQt5QMakeBuildStepFactory(QObject *parent = nullptr);

    QList<ProjectExplorer::BuildStepInfo>
        availableSteps(ProjectExplorer::BuildStepList *parent) const override;

    ProjectExplorer::BuildStep *create(ProjectExplorer::BuildStepList *parent, Core::Id id) override;
    ProjectExplorer::BuildStep *clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *product) override;
};

}


#endif // PHPQT5QMAKEBUILDSTEPFACTORY_H
