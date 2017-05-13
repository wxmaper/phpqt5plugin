#ifndef PHPQT5RUNCONTROLFACTORY_H
#define PHPQT5RUNCONTROLFACTORY_H

#include <projectexplorer/runconfiguration.h>

namespace PHPQt5 {

class PHPQt5RunControlFactory : public ProjectExplorer::IRunControlFactory
{
public:
    bool canRun(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode) const override;
    ProjectExplorer::RunControl *create(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode, QString *errorMessage) override;
};

}

#endif // PHPQT5RUNCONTROLFACTORY_H
