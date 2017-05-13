#include "phpqt5jombuildstepfactory.h"

#include "../phpqt5buildconfiguration.h"
#include "PHPQt5Jombuildstep.h"
#include "PHPQt5Jombuildstepfactory.h"

#include "../phpqt5constants.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <utils/qtcassert.h>

#include <memory>

using namespace ProjectExplorer;

namespace PHPQt5 {

PHPQt5JomBuildStepFactory::PHPQt5JomBuildStepFactory(QObject *parent)
    : IBuildStepFactory(parent)
{}

QList<BuildStepInfo> PHPQt5JomBuildStepFactory::availableSteps(BuildStepList *parent) const
{
    if (parent->id() != ProjectExplorer::Constants::BUILDSTEPS_BUILD)
        return {};

    PHPQt5BuildConfiguration *bc = qobject_cast<PHPQt5BuildConfiguration *>(parent->parent());
    if (!bc || bc->hasPHPQt5JomBuildStep())
        return {};

    return {{ Constants::C_PHPQt5JOMBUILDSTEP_ID, tr("jom Build Step") }};
}

BuildStep *PHPQt5JomBuildStepFactory::create(BuildStepList *parent, Core::Id)
{
    return new PHPQt5JomBuildStep(parent);
}

BuildStep *PHPQt5JomBuildStepFactory::clone(BuildStepList *parent, BuildStep *buildStep)
{
    QTC_ASSERT(parent, return nullptr);
    QTC_ASSERT(buildStep, return nullptr);
    std::unique_ptr<PHPQt5JomBuildStep> result(new PHPQt5JomBuildStep(parent));
    return result->fromMap(buildStep->toMap()) ? result.release() : nullptr;
}

}
