#include "phpqt5runconfiguration.h"
#include "phpqt5buildconfiguration.h"
#include "phpqt5runconfigurationwidget.h"

#include "phpqt5constants.h"

#include <projectexplorer/runnables.h>
#include <projectexplorer/localenvironmentaspect.h>
#include <projectexplorer/runconfigurationaspects.h>

#include <utils/environment.h>

#include <QDir>
#include <QFileInfo>
#include <QVariantMap>
#include <QMessageBox>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5RunConfiguration::PHPQt5RunConfiguration(Target *parent, Core::Id id)
    : RunConfiguration(parent, id)
    , m_buildConfiguration(nullptr)
    , m_workingDirectoryAspect(new WorkingDirectoryAspect(this, PHPQt5::Constants::C_PHPQt5RUNCONFIGURATION_WORKINGDIRECTORYASPECT_ID))
    , m_argumentAspect(new ArgumentsAspect(this, PHPQt5::Constants::C_PHPQt5RUNCONFIGURATION_ARGUMENTASPECT_ID))
    , m_terminalAspect(new TerminalAspect(this, PHPQt5::Constants::C_PHPQt5RUNCONFIGURATION_TERMINALASPECT_ID))
    , m_localEnvironmentAspect(new LocalEnvironmentAspect(this, LocalEnvironmentAspect::BaseEnvironmentModifier()))
{
    m_terminalAspect->setRunMode(ApplicationLauncher::Gui);

    addExtraAspect(m_argumentAspect);
    addExtraAspect(m_terminalAspect);
    addExtraAspect(m_localEnvironmentAspect);

    setDisplayName(tr(Constants::C_PHPQt5RUNCONFIGURATION_DISPLAY));
    setDefaultDisplayName(tr(Constants::C_PHPQt5RUNCONFIGURATION_DEFAULT_DISPLAY));

    // Connect target signals
    connect(this->target(), &Target::activeBuildConfigurationChanged,
            this, &PHPQt5RunConfiguration::updateConfiguration);

    updateConfiguration();
}

QWidget *PHPQt5RunConfiguration::createConfigurationWidget()
{
    return new PHPQt5RunConfigurationWidget(this);
}

Runnable PHPQt5RunConfiguration::runnable() const
{
    StandardRunnable result;
    result.runMode = m_terminalAspect->runMode();
    result.executable = m_executable;
    result.commandLineArguments = m_argumentAspect->arguments();
    result.workingDirectory = m_workingDirectoryAspect->workingDirectory().toString();
    result.environment = m_localEnvironmentAspect->environment();
    return result;
}


QVariantMap PHPQt5RunConfiguration::toMap() const
{
    auto result = RunConfiguration::toMap();
    result[Constants::C_PHPQt5RUNCONFIGURATION_EXECUTABLE_KEY] = m_executable;
    return result;
}

bool PHPQt5RunConfiguration::fromMap(const QVariantMap &map)
{
    bool result = RunConfiguration::fromMap(map);
    if (!result)
        return result;
    m_executable = map[Constants::C_PHPQt5RUNCONFIGURATION_EXECUTABLE_KEY].toString();
    return true;
}

void PHPQt5RunConfiguration::setExecutable(const QString &executable)
{
    if (m_executable == executable)
        return;

    m_executable = executable;
    emit executableChanged(executable);
}

void PHPQt5RunConfiguration::setWorkingDirectory(const QString &workingDirectory)
{
    m_workingDirectoryAspect->setDefaultWorkingDirectory(FileName::fromString(workingDirectory));
}

void PHPQt5RunConfiguration::updateConfiguration()
{

    PHPQt5BuildConfiguration *buildConfiguration = qobject_cast<PHPQt5BuildConfiguration *>(activeBuildConfiguration());

    QTC_ASSERT(buildConfiguration, return);
    setActiveBuildConfiguration(buildConfiguration);
    const QFileInfo outFileInfo = buildConfiguration->outFilePath().toFileInfo();

#ifdef Q_OS_WIN
    static const QString execExt = ".exe";
#else
    static const QString execExt = "";
#endif

    switch (buildConfiguration->buildType()) {
    case BuildConfiguration::BuildType::Release: {
        const QString execFileName = buildConfiguration->projectFilePath().fileName()
                .replace(QRegExp("\\.phpqt5$"), execExt);
        setExecutable(QString("%1/%2/%3")
                      .arg(outFileInfo.absoluteFilePath())
                      .arg("release")
                      .arg(execFileName));
    } break;

    case BuildConfiguration::BuildType::Debug: {
        const QString execFileName = buildConfiguration->projectFilePath().fileName()
                .replace(QRegExp("\\.phpqt5$"), QString("-debug%1").arg(execExt));
        setExecutable(QString("%1/%2/%3")
                      .arg(outFileInfo.absoluteFilePath())
                      .arg("release")
                      .arg(execFileName));
    } break;

    default: //nothing
        break;
    }


    setWorkingDirectory(outFileInfo.absoluteDir().absolutePath());
}

void PHPQt5RunConfiguration::setActiveBuildConfiguration(PHPQt5BuildConfiguration *activeBuildConfiguration)
{
    if (m_buildConfiguration == activeBuildConfiguration)
        return;

    if (m_buildConfiguration) {
        disconnect(m_buildConfiguration, &PHPQt5BuildConfiguration::buildDirectoryChanged,
                   this, &PHPQt5RunConfiguration::updateConfiguration);
        disconnect(m_buildConfiguration, &PHPQt5BuildConfiguration::outFilePathChanged,
                   this, &PHPQt5RunConfiguration::updateConfiguration);
    }

    m_buildConfiguration = activeBuildConfiguration;

    if (m_buildConfiguration) {
        connect(m_buildConfiguration, &PHPQt5BuildConfiguration::buildDirectoryChanged,
                this, &PHPQt5RunConfiguration::updateConfiguration);
        connect(m_buildConfiguration, &PHPQt5BuildConfiguration::outFilePathChanged,
                this, &PHPQt5RunConfiguration::updateConfiguration);
        //connect(m_buildConfiguration, &PHPQt5BuildConfiguration::buildTypeChanged,
        //        this, &PHPQt5RunConfiguration::updateConfiguration);
    }
}

}
