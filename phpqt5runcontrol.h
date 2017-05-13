#ifndef PHPQT5RUNCONTROL_H
#define PHPQT5RUNCONTROL_H

#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/runnables.h>

#include <QCoreApplication>

namespace PHPQt5 {

class PHPQt5RunConfiguration;

class PHPQt5RunControl : public ProjectExplorer::RunControl
{
    Q_DECLARE_TR_FUNCTIONS(PHPQt5::PHPQt5RunControl)

public:
    PHPQt5RunControl(PHPQt5RunConfiguration *runConfiguration, Core::Id mode);

    void start() override;
    StopResult stop() override;
    bool isRunning() const override;

private:
    void processStarted();
    void processExited(int exitCode, QProcess::ExitStatus status);
    void slotAppendMessage(const QString &err, Utils::OutputFormat isError);

    ProjectExplorer::ApplicationLauncher m_applicationLauncher;
    bool m_running;
    ProjectExplorer::StandardRunnable m_runnable;
};

}

#endif // PHPQT5RUNCONTROL_H
