#include "phpqt5runcontrolfactory.h"
#include "phpqt5runconfiguration.h"
#include "phpqt5runcontrol.h"

namespace PHPQt5 {

bool PHPQt5RunControlFactory::canRun(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode) const
{
    Q_UNUSED(mode)
    return dynamic_cast<PHPQt5RunConfiguration *>(runConfiguration);
}

ProjectExplorer::RunControl *PHPQt5RunControlFactory::create(ProjectExplorer::RunConfiguration *runConfiguration, Core::Id mode, QString *errorMessage)
{
    Q_UNUSED(errorMessage)
    QTC_ASSERT(canRun(runConfiguration, mode), return 0);
    return new PHPQt5RunControl(static_cast<PHPQt5RunConfiguration *>(runConfiguration), mode);
}

}

