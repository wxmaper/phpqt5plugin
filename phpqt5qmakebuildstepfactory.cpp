#include "phpqt5qmakebuildstepfactory.h"

#include "phpqt5buildconfiguration.h"
#include "PHPQt5QMakebuildstep.h"
#include "PHPQt5QMakebuildstepfactory.h"

#include "phpqt5constants.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <utils/qtcassert.h>

#include <memory>

using namespace ProjectExplorer;

namespace PHPQt5 {

PHPQt5QMakeBuildStepFactory::PHPQt5QMakeBuildStepFactory(QObject *parent)
    : IBuildStepFactory(parent)
{}

QList<BuildStepInfo> PHPQt5QMakeBuildStepFactory::availableSteps(BuildStepList *parent) const
{
    if (parent->id() != ProjectExplorer::Constants::BUILDSTEPS_BUILD)
        return {};

    PHPQt5BuildConfiguration *bc = qobject_cast<PHPQt5BuildConfiguration *>(parent->parent());
    if (!bc || bc->hasPHPQt5QMakeBuildStep())
        return {};

    return {{ Constants::C_PHPQt5QMAKEBUILDSTEP_ID, tr("qmake Build Step") }};
}

BuildStep *PHPQt5QMakeBuildStepFactory::create(BuildStepList *parent, Core::Id)
{
    return new PHPQt5QMakeBuildStep(parent);
}

BuildStep *PHPQt5QMakeBuildStepFactory::clone(BuildStepList *parent, BuildStep *buildStep)
{
    QTC_ASSERT(parent, return nullptr);
    QTC_ASSERT(buildStep, return nullptr);
    std::unique_ptr<PHPQt5QMakeBuildStep> result(new PHPQt5QMakeBuildStep(parent));
    return result->fromMap(buildStep->toMap()) ? result.release() : nullptr;
}

}
