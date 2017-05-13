#ifndef PHPQT5RUNCONFIGURATION_H
#define PHPQT5RUNCONFIGURATION_H


#include <projectexplorer/runconfiguration.h>

namespace ProjectExplorer {
class WorkingDirectoryAspect;
class ArgumentsAspect;
class TerminalAspect;
class LocalEnvironmentAspect;
}

namespace PHPQt5 {

class PHPQt5BuildConfiguration;

class PHPQt5RunConfiguration : public ProjectExplorer::RunConfiguration
{
    Q_OBJECT

public:
    PHPQt5RunConfiguration(ProjectExplorer::Target *parent, Core::Id id);

    QWidget *createConfigurationWidget() override;
    ProjectExplorer::Runnable runnable() const override;
    QVariantMap toMap() const override;
    bool fromMap(const QVariantMap &map) override;

signals:
    void executableChanged(const QString &args);
    void runModeChanged(ProjectExplorer::ApplicationLauncher::Mode);
    void workingDirectoryChanged(const QString &workingDirectory);
    void commandLineArgumentsChanged(const QString &args);
    void runInTerminalChanged(bool);

private:
    void setExecutable(const QString &path);
    void setWorkingDirectory(const QString &path);
    void setCommandLineArguments(const QString &args);
    void updateConfiguration();
    void setActiveBuildConfiguration(PHPQt5BuildConfiguration *activeBuildConfiguration);

    QString m_executable;
    PHPQt5BuildConfiguration *m_buildConfiguration;
    ProjectExplorer::WorkingDirectoryAspect* m_workingDirectoryAspect;
    ProjectExplorer::ArgumentsAspect* m_argumentAspect;
    ProjectExplorer::TerminalAspect* m_terminalAspect;
    ProjectExplorer::LocalEnvironmentAspect* m_localEnvironmentAspect;
};

}


#endif // PHPQT5RUNCONFIGURATION_H
