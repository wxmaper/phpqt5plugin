#include "phpqt5makebuildstepfactory.h"

#include "phpqt5buildconfiguration.h"
#include "phpqt5makebuildstep.h"
#include "phpqt5makebuildstepfactory.h"

#include "phpqt5constants.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <utils/qtcassert.h>

#include <memory>

using namespace ProjectExplorer;

namespace PHPQt5 {

PHPQt5MakeBuildStepFactory::PHPQt5MakeBuildStepFactory(QObject *parent)
    : IBuildStepFactory(parent)
{}

QList<BuildStepInfo> PHPQt5MakeBuildStepFactory::availableSteps(BuildStepList *parent) const
{
    if (parent->id() != ProjectExplorer::Constants::BUILDSTEPS_BUILD)
        return {};

    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(parent->parent());
    if (!bc || bc->hasPHPQt5MakeBuildStep())
        return {};

    return {{ Constants::C_PHPQt5MAKEBUILDSTEP_ID, tr("PHPQt5Make Build Step") }};
}

BuildStep *PHPQt5MakeBuildStepFactory::create(BuildStepList *parent, Core::Id)
{
    return new PHPQt5MakeBuildStep(parent);
}

BuildStep *PHPQt5MakeBuildStepFactory::clone(BuildStepList *parent, BuildStep *buildStep)
{
    QTC_ASSERT(parent, return nullptr);
    QTC_ASSERT(buildStep, return nullptr);
    std::unique_ptr<PHPQt5MakeBuildStep> result(new PHPQt5MakeBuildStep(parent));
    return result->fromMap(buildStep->toMap()) ? result.release() : nullptr;
}

}
