#include "phpqt5makecleanstepfactory.h"

#include "phpqt5buildconfiguration.h"
#include "phpqt5makecleanstep.h"

#include "phpqt5constants.h"

#include <projectexplorer/projectexplorerconstants.h>

#include <memory>

using namespace ProjectExplorer;

namespace PHPQt5 {

PHPQt5MakeCleanStepFactory::PHPQt5MakeCleanStepFactory(QObject *parent)
    : IBuildStepFactory(parent)
{}

QList<BuildStepInfo> PHPQt5MakeCleanStepFactory::availableSteps(BuildStepList *parent) const
{
    if (parent->id() != ProjectExplorer::Constants::BUILDSTEPS_CLEAN)
        return {};

    PHPQt5BuildConfiguration* bc = qobject_cast<PHPQt5BuildConfiguration *>(parent->parent());
    if (!bc || bc->hasPHPQt5MakeCleanStep())
        return {};

    return {{ Constants::C_PHPQt5MAKECLEANSTEP_ID,
              tr(PHPQt5::Constants::C_PHPQt5MAKECLEANSTEP_DISPLAY),
              BuildStepInfo::Unclonable }};
}

BuildStep *PHPQt5MakeCleanStepFactory::create(BuildStepList *parent, Core::Id)
{
    return new PHPQt5MakeCleanStep(parent);
}

BuildStep *PHPQt5MakeCleanStepFactory::clone(BuildStepList *, BuildStep *)
{
    return nullptr;
}

}
