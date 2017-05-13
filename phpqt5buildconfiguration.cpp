#include "phpqt5buildconfiguration.h"

#include "phpqt5buildconfigurationwidget.h"
#include "phpqt5makebuildstep.h"
#include "PHPQt5project.h"

#include "phpqt5constants.h"

#include <projectexplorer/namedwidget.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildsteplist.h>
#include <utils/qtcassert.h>

#include <QMessageBox>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5BuildConfiguration::PHPQt5BuildConfiguration(Target *target)
    : BuildConfiguration(target, Constants::C_PHPQt5BUILDCONFIGURATION_ID)
{}

NamedWidget *PHPQt5BuildConfiguration::createConfigWidget()
{
    return new PHPQt5BuildConfigurationWidget(this);
}

BuildConfiguration::BuildType PHPQt5BuildConfiguration::buildType() const
{
    return m_buildType;
}

void PHPQt5BuildConfiguration::setBuildType(BuildConfiguration::BuildType type)
{
    m_buildType = type;

   // QString dir = QString(buildDirectory().toString()).replace(QRegExp("-debug$"),"");
   // dir = dir.replace(QRegExp("-release$"), "");
   // dir = dir.replace(QRegExp("/release$"), "");
   // dir = dir.replace(QRegExp("/debug$"), "");
/*
    switch (m_buildType) {
    case Debug:
        setBuildDirectory(Utils::FileName::fromString(dir + "-debug"));
        break;

    case Release:
    default:
        setBuildDirectory(Utils::FileName::fromString(dir + "-release"));
        break;
    }
    */
}

bool PHPQt5BuildConfiguration::fromMap(const QVariantMap &map)
{
    if (!BuildConfiguration::fromMap(map))
        return false;

    if (!canRestore(map))
        return false;

    const QString displayName = map[Constants::C_PHPQt5BUILDCONFIGURATION_DISPLAY_KEY].toString();
    const QString buildDirectory = map[Constants::C_PHPQt5BUILDCONFIGURATION_BUILDDIRECTORY_KEY].toString();

    setDisplayName(displayName);
    setBuildDirectory(FileName::fromString(buildDirectory));

    return true;
}

QVariantMap PHPQt5BuildConfiguration::toMap() const
{
    QVariantMap result = BuildConfiguration::toMap();
    result[Constants::C_PHPQt5BUILDCONFIGURATION_DISPLAY_KEY] = displayName();
    result[Constants::C_PHPQt5BUILDCONFIGURATION_BUILDDIRECTORY_KEY] = buildDirectory().toString();
    return result;
}

FileName PHPQt5BuildConfiguration::cacheDirectory() const
{
    return buildDirectory().appendPath(QStringLiteral("PHPQt5cache"));
}

FileName PHPQt5BuildConfiguration::outFilePath() const
{
    const PHPQt5MakeBuildStep *step = phpqt5MakeBuildStep();
    QTC_ASSERT(step, return FileName());
    return step->outFilePath();
}

FileName PHPQt5BuildConfiguration::projectFilePath() const
{
    const PHPQt5MakeBuildStep *step = phpqt5MakeBuildStep();
    QTC_ASSERT(step, return FileName());
    return step->projectFilePath();
}

bool PHPQt5BuildConfiguration::canRestore(const QVariantMap &map)
{
    return idFromMap(map) == Constants::C_PHPQt5BUILDCONFIGURATION_ID;
}

bool PHPQt5BuildConfiguration::hasPHPQt5MakeBuildStep() const
{
    BuildStepList *steps = stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    return steps ? steps->contains(Constants::C_PHPQt5MAKEBUILDSTEP_ID) : false;
}

bool PHPQt5BuildConfiguration::hasPHPQt5QMakeBuildStep() const
{
    BuildStepList *steps = stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    return steps ? steps->contains(Constants::C_PHPQt5QMAKEBUILDSTEP_ID) : false;
}

bool PHPQt5BuildConfiguration::hasPHPQt5JomBuildStep() const
{
    BuildStepList *steps = stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    return steps ? steps->contains(Constants::C_PHPQt5JOMBUILDSTEP_ID) : false;
}

bool PHPQt5BuildConfiguration::hasPHPQt5UicBuildStep() const
{
    BuildStepList *steps = stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    return steps ? steps->contains(Constants::C_PHPQt5UICBUILDSTEP_ID) : false;
}

bool PHPQt5BuildConfiguration::hasPHPQt5MakeCleanStep() const
{
    BuildStepList *steps = stepList(ProjectExplorer::Constants::BUILDSTEPS_CLEAN);
    return steps ? steps->contains(Constants::C_PHPQt5MAKECLEANSTEP_ID) : false;
}

const PHPQt5MakeBuildStep *PHPQt5BuildConfiguration::phpqt5MakeBuildStep() const
{
    BuildStepList *steps = stepList(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
    QTC_ASSERT(steps, return nullptr);
    foreach (BuildStep *step, steps->steps())
        if (step->id() == Constants::C_PHPQt5MAKEBUILDSTEP_ID)
            return qobject_cast<PHPQt5MakeBuildStep *>(step);
    return nullptr;
}

}

