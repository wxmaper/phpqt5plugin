#include "phpqt5runcontrol.h"
#include "phpqt5runconfiguration.h"

#include <projectexplorer/runnables.h>
#include <utils/qtcprocess.h>
#include <utils/outputformat.h>

#include <QDir>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5RunControl::PHPQt5RunControl(PHPQt5RunConfiguration *rc, Core::Id mode)
    : RunControl(rc, mode)
    , m_running(false)
    , m_runnable(rc->runnable().as<StandardRunnable>())
{
    connect(&m_applicationLauncher, &ApplicationLauncher::appendMessage,
            this, &PHPQt5RunControl::slotAppendMessage);
    connect(&m_applicationLauncher, &ApplicationLauncher::processStarted,
            this, &PHPQt5RunControl::processStarted);
    connect(&m_applicationLauncher, &ApplicationLauncher::processExited,
            this, &PHPQt5RunControl::processExited);
    connect(&m_applicationLauncher, &ApplicationLauncher::bringToForegroundRequested,
            this, &RunControl::bringApplicationToForeground);
}

void PHPQt5RunControl::start()
{
    emit started();
    m_running = true;
    m_applicationLauncher.start(m_runnable);
    setApplicationProcessHandle(ProcessHandle(m_applicationLauncher.applicationPID()));
}

ProjectExplorer::RunControl::StopResult PHPQt5RunControl::stop()
{
    m_applicationLauncher.stop();
    return StoppedSynchronously;
}

bool PHPQt5RunControl::isRunning() const
{
    return m_running;
}

void PHPQt5RunControl::processStarted()
{
    // Console processes only know their pid after being started
    setApplicationProcessHandle(ProcessHandle(m_applicationLauncher.applicationPID()));
}

void PHPQt5RunControl::processExited(int exitCode, QProcess::ExitStatus status)
{
    m_running = false;
    setApplicationProcessHandle(ProcessHandle());
    QString msg;
    if (status == QProcess::CrashExit) {
        msg = tr("%1 crashed")
                .arg(QDir::toNativeSeparators(m_runnable.executable));
    } else {
        msg = tr("%1 exited with code %2")
                .arg(QDir::toNativeSeparators(m_runnable.executable)).arg(exitCode);
    }
    appendMessage(msg + QLatin1Char('\n'), NormalMessageFormat);
    emit finished();
}

void PHPQt5RunControl::slotAppendMessage(const QString &err, OutputFormat format)
{
    appendMessage(err, format);
}

}
