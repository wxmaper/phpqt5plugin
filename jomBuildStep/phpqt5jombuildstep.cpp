#include "phpqt5jombuildstep.h"

#include "PHPQt5Jombuildstep.h"
#include "../phpqt5buildconfiguration.h"
#include "PHPQt5Jombuildstepconfigwidget.h"

#include "../phpqt5constants.h"
#include "../phpqt5project.h"

#include <projectexplorer/buildconfiguration.h>
#include <utils/qtcassert.h>

#include <QDir>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5JomBuildStep::PHPQt5JomBuildStep(BuildStepList *parentList)
    : AbstractProcessStep(parentList, Constants::C_PHPQt5JOMBUILDSTEP_ID)
{
    setDefaultDisplayName(Constants::C_PHPQt5JOMBUILDSTEP_DISPLAY);
    setDisplayName(Constants::C_PHPQt5JOMBUILDSTEP_DISPLAY);

    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    connect(bc, &PHPQt5BuildConfiguration::buildDirectoryChanged,
            this, &PHPQt5JomBuildStep::updateProcessParameters);
    connect(this, &PHPQt5JomBuildStep::outFilePathChanged,
            bc, &PHPQt5BuildConfiguration::outFilePathChanged);
    updateProcessParameters();
}

PHPQt5JomBuildStep::DefaultBuildOptions PHPQt5JomBuildStep::defaultCompilerOptions() const
{
    return m_defaultOptions;
}

void PHPQt5JomBuildStep::setDefaultCompilerOptions(PHPQt5JomBuildStep::DefaultBuildOptions options)
{
    if (m_defaultOptions == options)
        return;
    m_defaultOptions = options;
    emit defaultCompilerOptionsChanged(options);
    updateProcessParameters();
}

BuildStepConfigWidget *PHPQt5JomBuildStep::createConfigWidget()
{
    return new PHPQt5JomBuildStepConfigWidget(this);
}

bool PHPQt5JomBuildStep::fromMap(const QVariantMap &map)
{
    AbstractProcessStep::fromMap(map);
    m_userCompilerOptions = map[Constants::C_PHPQt5JOMBUILDSTEP_USERCOMPILEROPTIONS].toString().split(QLatin1Char('|'));
    updateProcessParameters();
    return true;
}

QVariantMap PHPQt5JomBuildStep::toMap() const
{
    QVariantMap result = AbstractProcessStep::toMap();
    result[Constants::C_PHPQt5JOMBUILDSTEP_USERCOMPILEROPTIONS] = m_userCompilerOptions.join(QLatin1Char('|'));
    return result;
}

QStringList PHPQt5JomBuildStep::userCompilerOptions() const
{
    return m_userCompilerOptions;
}

void PHPQt5JomBuildStep::setUserCompilerOptions(const QStringList &options)
{
    m_userCompilerOptions = options;
    emit userCompilerOptionsChanged(options);
    updateProcessParameters();
}

FileName PHPQt5JomBuildStep::outFilePath() const
{
    return m_outFilePath;
}

void PHPQt5JomBuildStep::setOutFilePath(const FileName &outFilePath)
{
    if (outFilePath == m_outFilePath)
        return;
    m_outFilePath = outFilePath;
    emit outFilePathChanged(outFilePath);
}

void PHPQt5JomBuildStep::updateProcessParameters()
{
    updateOutFilePath();
    updateCommand();
    updateArguments();
    updateWorkingDirectory();
    updateEnvironment();
    emit processParametersChanged();
}

void PHPQt5JomBuildStep::updateOutFilePath()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);
    //const QString targetName = Utils::HostOsInfo::withExecutableSuffix(m_targetPHPQt5File.toFileInfo().baseName());
    //FileName outFilePath = bc->buildDirectory().appendPath(targetName);
    FileName outFilePath = bc->buildDirectory().appendPath("");
    setOutFilePath(outFilePath);
}

void PHPQt5JomBuildStep::updateCommand()
{
#ifdef Q_OS_WIN
        processParameters()->setCommand(QStringLiteral("jom.exe"));

#else
        processParameters()->setCommand(QStringLiteral("jom"));
#endif
}

void PHPQt5JomBuildStep::updateWorkingDirectory()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);
    processParameters()->setWorkingDirectory(bc->buildDirectory().toString());
}

void PHPQt5JomBuildStep::updateArguments()
{
    PHPQt5BuildConfiguration *bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);

    QStringList arguments;

    arguments << QStringLiteral("qmake_all");
    arguments << QStringLiteral("&&");

#ifdef Q_OS_WIN
    arguments << QStringLiteral("jom.exe");

#else
    arguments << QStringLiteral("jom");
#endif

    arguments << m_userCompilerOptions;

    // Remove empty args
    auto predicate = [](const QString &str) { return str.isEmpty(); };
    auto it = std::remove_if(arguments.begin(), arguments.end(), predicate);
    arguments.erase(it, arguments.end());

    processParameters()->setArguments(arguments.join(QChar::Space));
}

void PHPQt5JomBuildStep::updateEnvironment()
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



