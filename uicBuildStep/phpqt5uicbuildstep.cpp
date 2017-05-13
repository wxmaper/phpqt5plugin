#include "phpqt5uicbuildstep.h"

#include "PHPQt5Uicbuildstep.h"
#include "../phpqt5buildconfiguration.h"
#include "PHPQt5Uicbuildstepconfigwidget.h"

#include "../phpqt5constants.h"
#include "../phpqt5project.h"

#include <projectexplorer/buildconfiguration.h>
#include <utils/qtcassert.h>

#include <QDir>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5UicBuildStep::PHPQt5UicBuildStep(BuildStepList *parentList)
    : AbstractProcessStep(parentList, Constants::C_PHPQt5UICBUILDSTEP_ID)
{
    setDefaultDisplayName(Constants::C_PHPQt5UICBUILDSTEP_DISPLAY);
    setDisplayName(Constants::C_PHPQt5UICBUILDSTEP_DISPLAY);

    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    connect(bc, &PHPQt5BuildConfiguration::buildDirectoryChanged,
            this, &PHPQt5UicBuildStep::updateProcessParameters);
    connect(this, &PHPQt5UicBuildStep::outFilePathChanged,
            bc, &PHPQt5BuildConfiguration::outFilePathChanged);
    updateProcessParameters();
}

PHPQt5UicBuildStep::DefaultBuildOptions PHPQt5UicBuildStep::defaultCompilerOptions() const
{
    return m_defaultOptions;
}

void PHPQt5UicBuildStep::setDefaultCompilerOptions(PHPQt5UicBuildStep::DefaultBuildOptions options)
{
    if (m_defaultOptions == options)
        return;
    m_defaultOptions = options;
    emit defaultCompilerOptionsChanged(options);
    updateProcessParameters();
}

BuildStepConfigWidget *PHPQt5UicBuildStep::createConfigWidget()
{
    return new PHPQt5UicBuildStepConfigWidget(this);
}

bool PHPQt5UicBuildStep::fromMap(const QVariantMap &map)
{
    AbstractProcessStep::fromMap(map);
    m_userCompilerOptions = map[Constants::C_PHPQt5UICBUILDSTEP_USERCOMPILEROPTIONS].toString().split(QLatin1Char('|'));
    updateProcessParameters();
    return true;
}

QVariantMap PHPQt5UicBuildStep::toMap() const
{
    QVariantMap result = AbstractProcessStep::toMap();
    result[Constants::C_PHPQt5UICBUILDSTEP_USERCOMPILEROPTIONS] = m_userCompilerOptions.join(QLatin1Char('|'));
    return result;
}

QStringList PHPQt5UicBuildStep::userCompilerOptions() const
{
    return m_userCompilerOptions;
}

void PHPQt5UicBuildStep::setUserCompilerOptions(const QStringList &options)
{
    m_userCompilerOptions = options;
    emit userCompilerOptionsChanged(options);
    updateProcessParameters();
}

FileName PHPQt5UicBuildStep::outFilePath() const
{
    return m_outFilePath;
}

void PHPQt5UicBuildStep::setOutFilePath(const FileName &outFilePath)
{
    if (outFilePath == m_outFilePath)
        return;
    m_outFilePath = outFilePath;
    emit outFilePathChanged(outFilePath);
}

void PHPQt5UicBuildStep::updateProcessParameters()
{
    updateOutFilePath();
    updateCommand();
    updateArguments();
    updateWorkingDirectory();
    updateEnvironment();
    emit processParametersChanged();
}

void PHPQt5UicBuildStep::updateOutFilePath()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);
    //const QString targetName = Utils::HostOsInfo::withExecutableSuffix(m_targetPHPQt5File.toFileInfo().baseName());
    //FileName outFilePath = bc->buildDirectory().appendPath(targetName);
    FileName outFilePath = bc->buildDirectory().appendPath("");
    setOutFilePath(outFilePath);
}

void PHPQt5UicBuildStep::updateCommand()
{
#ifdef Q_OS_WIN
        processParameters()->setCommand(QStringLiteral("PHPQt5Uic.exe"));

#else
        processParameters()->setCommand(QStringLiteral("PHPQt5Uic"));
#endif
}

void PHPQt5UicBuildStep::updateWorkingDirectory()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);
    processParameters()->setWorkingDirectory(bc->buildDirectory().toString());
}

void PHPQt5UicBuildStep::updateArguments()
{
    PHPQt5BuildConfiguration *bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);

    QStringList arguments;

    PHPQt5Project *p = qobject_cast<PHPQt5Project *>(project());

    arguments << QStringLiteral("--projectPath:\"%1\"").arg(p->projectFilePath().toString());
    arguments << QStringLiteral("--buildPath:\"%1\"").arg(m_outFilePath.toString());

    arguments << m_userCompilerOptions;

    arguments << m_userCompilerOptions;

    // Remove empty args
    auto predicate = [](const QString &str) { return str.isEmpty(); };
    auto it = std::remove_if(arguments.begin(), arguments.end(), predicate);
    arguments.erase(it, arguments.end());

    processParameters()->setArguments(arguments.join(QChar::Space));
}

void PHPQt5UicBuildStep::updateEnvironment()
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



