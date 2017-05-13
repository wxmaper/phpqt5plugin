#include "phpqt5uicbuildstepfactory.h"

#include "../phpqt5buildconfiguration.h"
#include "PHPQt5Uicbuildstep.h"
#include "PHPQt5Uicbuildstepfactory.h"

#include "../phpqt5constants.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <utils/qtcassert.h>

#include <memory>

using namespace ProjectExplorer;

namespace PHPQt5 {

PHPQt5UicBuildStepFactory::PHPQt5UicBuildStepFactory(QObject *parent)
    : IBuildStepFactory(parent)
{}

QList<BuildStepInfo> PHPQt5UicBuildStepFactory::availableSteps(BuildStepList *parent) const
{
    if (parent->id() != ProjectExplorer::Constants::BUILDSTEPS_BUILD)
        return {};

    PHPQt5BuildConfiguration *bc = qobject_cast<PHPQt5BuildConfiguration *>(parent->parent());
    if (!bc || bc->hasPHPQt5UicBuildStep())
        return {};

    return {{ Constants::C_PHPQt5UICBUILDSTEP_ID, tr("uic Build Step") }};
}

BuildStep *PHPQt5UicBuildStepFactory::create(BuildStepList *parent, Core::Id)
{
    return new PHPQt5UicBuildStep(parent);
}

BuildStep *PHPQt5UicBuildStepFactory::clone(BuildStepList *parent, BuildStep *buildStep)
{
    QTC_ASSERT(parent, return nullptr);
    QTC_ASSERT(buildStep, return nullptr);
    std::unique_ptr<PHPQt5UicBuildStep> result(new PHPQt5UicBuildStep(parent));
    return result->fromMap(buildStep->toMap()) ? result.release() : nullptr;
}

}
