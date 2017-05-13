#include "phpqt5buildconfigurationfactory.h"
#include "phpqt5buildconfiguration.h"
#include "phpqt5makebuildstep.h"
#include "phpqt5qmakebuildstep.h"
#include "phpqt5makecleanstep.h"

#include "jomBuildStep/phpqt5jombuildstep.h"
#include "uicBuildStep/phpqt5uicbuildstep.h"

#include "PHPQt5project.h"

#include "phpqt5constants.h"

#include <coreplugin/documentmanager.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectmacroexpander.h>
#include <projectexplorer/target.h>
#include <utils/mimetypes/mimedatabase.h>
#include <utils/qtcassert.h>

#include <memory>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5BuildConfigurationFactory::PHPQt5BuildConfigurationFactory(QObject *parent)
    : IBuildConfigurationFactory(parent)
{}

QList<BuildInfo *> PHPQt5BuildConfigurationFactory::availableBuilds(const Target *parent) const
{
    // Retrieve the project path
    auto project = qobject_cast<PHPQt5Project *>(parent->project());
    QTC_ASSERT(project, return {});

    // Create the build info
    BuildInfo *info = createBuildInfo(parent->kit(), project->projectFilePath().toString(),
                                      BuildConfiguration::Debug);

    info->displayName.clear(); // ask for a name
    info->buildDirectory.clear(); // This depends on the displayName

    return { info };
}

QList<BuildInfo *> PHPQt5BuildConfigurationFactory::availableSetups(const Kit *k, const QString &projectPath) const
{
    BuildInfo *debug = createBuildInfo(k, projectPath, BuildConfiguration::Debug);
    BuildInfo *release = createBuildInfo(k, projectPath, BuildConfiguration::Release);
    return { debug, release };
}

BuildConfiguration *PHPQt5BuildConfigurationFactory::create(Target *parent, const BuildInfo *info) const
{
    PHPQt5Project *project = qobject_cast<PHPQt5Project *>(parent->project());
    QTC_ASSERT(project, return nullptr);

    // Create the build configuration and initialize it from build info
    PHPQt5BuildConfiguration *result = new PHPQt5BuildConfiguration(parent);
    result->setDisplayName(info->displayName);
    result->setDefaultDisplayName(info->displayName);
    result->setBuildDirectory(defaultBuildDirectory(parent->kit(),
                                                    project->projectFilePath().toString(),
                                                    info->displayName,
                                                    info->buildType));
    result->setBuildType(info->buildType);

    // Add PHPQt5Make build step
    {
        BuildStepList *buildSteps = result->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
        PHPQt5MakeBuildStep *phpqt5MakeBuildStep = new PHPQt5MakeBuildStep(buildSteps);
        PHPQt5MakeBuildStep::DefaultBuildOptions defaultOption;

        switch (info->buildType) {
        case BuildConfiguration::Release:
            defaultOption = PHPQt5MakeBuildStep::DefaultBuildOptions::Release;
            break;
        case BuildConfiguration::Debug:
            defaultOption = PHPQt5MakeBuildStep::DefaultBuildOptions::Debug;
            break;
        default:
            defaultOption = PHPQt5MakeBuildStep::DefaultBuildOptions::Empty;
            break;
        }
        phpqt5MakeBuildStep->setDefaultCompilerOptions(defaultOption);

        buildSteps->appendStep(phpqt5MakeBuildStep);
    }

    // Add qmake build step
    {
        BuildStepList *buildSteps = result->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
        PHPQt5QMakeBuildStep *phpqt5QMakeBuildStep = new PHPQt5QMakeBuildStep(buildSteps);
        PHPQt5QMakeBuildStep::DefaultBuildOptions defaultOption;

        switch (info->buildType) {
        case BuildConfiguration::Release:
            defaultOption = PHPQt5QMakeBuildStep::DefaultBuildOptions::Release;
            break;
        case BuildConfiguration::Debug:
            defaultOption = PHPQt5QMakeBuildStep::DefaultBuildOptions::Debug;
            break;
        default:
            defaultOption = PHPQt5QMakeBuildStep::DefaultBuildOptions::Empty;
            break;
        }
        phpqt5QMakeBuildStep->setDefaultCompilerOptions(defaultOption);

        buildSteps->appendStep(phpqt5QMakeBuildStep);
    }

    // Add PHPQt5Uic build step
    {
        BuildStepList *buildSteps = result->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
        PHPQt5UicBuildStep *phpqt5UicBuildStep = new PHPQt5UicBuildStep(buildSteps);
        PHPQt5UicBuildStep::DefaultBuildOptions defaultOption;

        switch (info->buildType) {
        case BuildConfiguration::Release:
            defaultOption = PHPQt5UicBuildStep::DefaultBuildOptions::Release;
            break;
        case BuildConfiguration::Debug:
            defaultOption = PHPQt5UicBuildStep::DefaultBuildOptions::Debug;
            break;
        default:
            defaultOption = PHPQt5UicBuildStep::DefaultBuildOptions::Empty;
            break;
        }
        phpqt5UicBuildStep->setDefaultCompilerOptions(defaultOption);

        buildSteps->appendStep(phpqt5UicBuildStep);
    }

    // Add jom build step
    {
        BuildStepList *buildSteps = result->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
        PHPQt5JomBuildStep *phpqt5JomBuildStep = new PHPQt5JomBuildStep(buildSteps);
        PHPQt5JomBuildStep::DefaultBuildOptions defaultOption;

        switch (info->buildType) {
        case BuildConfiguration::Release:
            defaultOption = PHPQt5JomBuildStep::DefaultBuildOptions::Release;
            break;
        case BuildConfiguration::Debug:
            defaultOption = PHPQt5JomBuildStep::DefaultBuildOptions::Debug;
            break;
        default:
            defaultOption = PHPQt5JomBuildStep::DefaultBuildOptions::Empty;
            break;
        }
        phpqt5JomBuildStep->setDefaultCompilerOptions(defaultOption);

        buildSteps->appendStep(phpqt5JomBuildStep);
    }

    // Add clean step
    {
        BuildStepList *cleanSteps = result->stepList(Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN));
        cleanSteps->appendStep(new PHPQt5MakeCleanStep(cleanSteps));
    }

    return result;
}

bool PHPQt5BuildConfigurationFactory::canRestore(const Target *parent, const QVariantMap &map) const
{
    Q_UNUSED(parent);
    return PHPQt5BuildConfiguration::canRestore(map);
}

BuildConfiguration *PHPQt5BuildConfigurationFactory::restore(Target *parent, const QVariantMap &map)
{
    QTC_ASSERT(canRestore(parent, map), return nullptr);

    // Create the build configuration
    auto result = new PHPQt5BuildConfiguration(parent);

    // Restore from map
    bool status = result->fromMap(map);
    QTC_ASSERT(status, return nullptr);

    return result;
}

bool PHPQt5BuildConfigurationFactory::canClone(const Target *parent, BuildConfiguration *product) const
{
    QTC_ASSERT(parent, return false);
    QTC_ASSERT(product, return false);
    return product->id() == Constants::C_PHPQt5BUILDCONFIGURATION_ID;
}

BuildConfiguration *PHPQt5BuildConfigurationFactory::clone(Target *parent, BuildConfiguration *product)
{
    QTC_ASSERT(parent, return nullptr);
    QTC_ASSERT(product, return nullptr);
    auto buildConfiguration = qobject_cast<PHPQt5BuildConfiguration *>(product);
    QTC_ASSERT(buildConfiguration, return nullptr);
    std::unique_ptr<PHPQt5BuildConfiguration> result(new PHPQt5BuildConfiguration(parent));
    return result->fromMap(buildConfiguration->toMap()) ? result.release() : nullptr;
}

int PHPQt5BuildConfigurationFactory::priority(const Kit *k, const QString &projectPath) const
{
    MimeDatabase mdb;
    if (k && mdb.mimeTypeForFile(projectPath).matchesName(Constants::C_PHPQT5_PROJECT_MIMETYPE))
        return 0;
    return -1;
}

int PHPQt5BuildConfigurationFactory::priority(const Target *parent) const
{
    return canHandle(parent) ? 0 : -1;
}

bool PHPQt5BuildConfigurationFactory::canHandle(const Target *t) const
{
    if (!t->project()->supportsKit(t->kit()))
        return false;
    return qobject_cast<PHPQt5Project *>(t->project());
}

FileName PHPQt5BuildConfigurationFactory::defaultBuildDirectory(const Kit *k,
                                                                const QString &projectFilePath,
                                                                const QString &bc,
                                                                BuildConfiguration::BuildType buildType)
{
    QFileInfo projectFileInfo(projectFilePath);

    ProjectMacroExpander expander(projectFilePath, projectFileInfo.baseName(), k, bc, buildType);
    QString buildDirectory = expander.expand(Core::DocumentManager::buildDirectory());

    if (FileUtils::isAbsolutePath(buildDirectory))
        return FileName::fromString(buildDirectory);

    auto projectDir = FileName::fromString(projectFileInfo.absoluteDir().absolutePath());
    auto result = projectDir.appendPath(buildDirectory);

    return result;
}

BuildInfo *PHPQt5BuildConfigurationFactory::createBuildInfo(const Kit *k, const QString &projectFilePath,
                                                            BuildConfiguration::BuildType buildType) const
{
    auto result = new BuildInfo(this);
    result->buildType = buildType;
    result->displayName = BuildConfiguration::buildTypeName(buildType);
    result->buildDirectory = defaultBuildDirectory(k, projectFilePath, result->displayName, buildType);
    result->kitId = k->id();
    result->typeName = tr("Build");
    return result;
}

} // namespace PHPQt5
