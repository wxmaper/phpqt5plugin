#include "phpqt5makebuildstep.h"

#include "phpqt5makebuildstep.h"
#include "phpqt5buildconfiguration.h"
#include "phpqt5makebuildstepconfigwidget.h"

#include "phpqt5constants.h"
#include "phpqt5project.h"

#include <projectexplorer/buildconfiguration.h>
#include <utils/qtcassert.h>

#include <QDir>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5MakeBuildStep::PHPQt5MakeBuildStep(BuildStepList *parentList)
    : AbstractProcessStep(parentList, Constants::C_PHPQt5MAKEBUILDSTEP_ID)
{
    setDefaultDisplayName(tr(Constants::C_PHPQt5MAKEBUILDSTEP_DISPLAY));
    setDisplayName(tr(Constants::C_PHPQt5MAKEBUILDSTEP_DISPLAY));

    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    connect(bc, &PHPQt5BuildConfiguration::buildDirectoryChanged,
            this, &PHPQt5MakeBuildStep::updateProcessParameters);
    connect(this, &PHPQt5MakeBuildStep::outFilePathChanged,
            bc, &PHPQt5BuildConfiguration::outFilePathChanged);
    updateProcessParameters();
}

BuildStepConfigWidget *PHPQt5MakeBuildStep::createConfigWidget()
{
    return new PHPQt5MakeBuildStepConfigWidget(this);
}

bool PHPQt5MakeBuildStep::fromMap(const QVariantMap &map)
{
    AbstractProcessStep::fromMap(map);
    m_userCompilerOptions = map[Constants::C_PHPQt5MAKEBUILDSTEP_USERCOMPILEROPTIONS].toString().split(QLatin1Char('|'));
    m_defaultOptions = static_cast<DefaultBuildOptions>(map[Constants::C_PHPQt5MAKEBUILDSTEP_DEFAULTBUILDOPTIONS].toInt());
    updateProcessParameters();
    return true;
}

QVariantMap PHPQt5MakeBuildStep::toMap() const
{
    QVariantMap result = AbstractProcessStep::toMap();
    result[Constants::C_PHPQt5MAKEBUILDSTEP_USERCOMPILEROPTIONS] = m_userCompilerOptions.join(QLatin1Char('|'));
    result[Constants::C_PHPQt5MAKEBUILDSTEP_DEFAULTBUILDOPTIONS] = m_defaultOptions;
    return result;
}

QStringList PHPQt5MakeBuildStep::userCompilerOptions() const
{
    return m_userCompilerOptions;
}

void PHPQt5MakeBuildStep::setUserCompilerOptions(const QStringList &options)
{
    m_userCompilerOptions = options;
    emit userCompilerOptionsChanged(options);
    updateProcessParameters();
}

PHPQt5MakeBuildStep::DefaultBuildOptions PHPQt5MakeBuildStep::defaultCompilerOptions() const
{
    return m_defaultOptions;
}

void PHPQt5MakeBuildStep::setDefaultCompilerOptions(PHPQt5MakeBuildStep::DefaultBuildOptions options)
{
    if (m_defaultOptions == options)
        return;
    m_defaultOptions = options;
    emit defaultCompilerOptionsChanged(options);
    updateProcessParameters();
}

FileName PHPQt5MakeBuildStep::outFilePath() const
{
    return m_outFilePath;
}

FileName PHPQt5MakeBuildStep::projectFilePath() const
{
    PHPQt5Project *p = qobject_cast<PHPQt5Project *>(project());
    return p->projectFilePath();
}

void PHPQt5MakeBuildStep::setOutFilePath(const FileName &outFilePath)
{
    if (outFilePath == m_outFilePath)
        return;
    m_outFilePath = outFilePath;
    emit outFilePathChanged(outFilePath);
}

void PHPQt5MakeBuildStep::updateProcessParameters()
{
    updateOutFilePath();
    updateCommand();
    updateArguments();
    updateWorkingDirectory();
    updateEnvironment();
    emit processParametersChanged();
}

void PHPQt5MakeBuildStep::updateOutFilePath()
{
    PHPQt5BuildConfiguration *bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);
    //const QString targetName = Utils::HostOsInfo::withExecutableSuffix(m_targetPHPQt5File.toFileInfo().baseName());
    //FileName outFilePath = bc->buildDirectory().appendPath(targetName);
    FileName outFilePath = bc->buildDirectory().appendPath("");
    setOutFilePath(outFilePath);
}

void PHPQt5MakeBuildStep::updateCommand()
{
#ifdef Q_OS_WIN
        processParameters()->setCommand(QStringLiteral("PHPQt5Make.exe"));

#else
        processParameters()->setCommand(QStringLiteral("PHPQt5Make"));
#endif
}

void PHPQt5MakeBuildStep::updateWorkingDirectory()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);
    processParameters()->setWorkingDirectory(bc->buildDirectory().toString());
}

void PHPQt5MakeBuildStep::updateArguments()
{
    PHPQt5BuildConfiguration *bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);

    QStringList arguments;

    switch (m_defaultOptions) {
    case Release:
        bc->setBuildType(BuildConfiguration::BuildType::Release);
        break;

    case Debug:
        bc->setBuildType(BuildConfiguration::BuildType::Debug);
        arguments << QStringLiteral("--debug");
        break;

    default:
        break;
    }

    PHPQt5Project *p = qobject_cast<PHPQt5Project *>(project());

    arguments << QStringLiteral("--projectPath:\"%1\"").arg(p->projectFilePath().toString());
    arguments << QStringLiteral("--buildPath:\"%1\"").arg(m_outFilePath.toString());

    arguments << m_userCompilerOptions;

    // Remove empty args
    auto predicate = [](const QString &str) { return str.isEmpty(); };
    auto it = std::remove_if(arguments.begin(), arguments.end(), predicate);
    arguments.erase(it, arguments.end());

    processParameters()->setArguments(arguments.join(QChar::Space));
}

void PHPQt5MakeBuildStep::updateEnvironment()
{
    PHPQt5BuildConfiguration * bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);

    Utils::Environment env = bc->environment();
    QDir applicationDirPath(QCoreApplication::applicationDirPath());
    applicationDirPath.cdUp();
    applicationDirPath.cdUp();
    applicationDirPath.cdUp();
    env.appendOrSetPath(applicationDirPath.canonicalPath() + "/Tools/PHPQt5");

    processParameters()->setEnvironment(env);
}

}


