#include "phpqt5qmakebuildstep.h"

#include "PHPQt5QMakebuildstep.h"
#include "phpqt5buildconfiguration.h"
#include "PHPQt5QMakebuildstepconfigwidget.h"

#include "phpqt5constants.h"
#include "phpqt5project.h"

#include <projectexplorer/buildconfiguration.h>
#include <utils/qtcassert.h>

#include <QDir>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5QMakeBuildStep::PHPQt5QMakeBuildStep(BuildStepList *parentList)
    : AbstractProcessStep(parentList, Constants::C_PHPQt5QMAKEBUILDSTEP_ID)
{
    setDefaultDisplayName(Constants::C_PHPQt5QMAKEBUILDSTEP_DISPLAY);
    setDisplayName(Constants::C_PHPQt5QMAKEBUILDSTEP_DISPLAY);

    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    connect(bc, &PHPQt5BuildConfiguration::buildDirectoryChanged,
            this, &PHPQt5QMakeBuildStep::updateProcessParameters);
    connect(this, &PHPQt5QMakeBuildStep::outFilePathChanged,
            bc, &PHPQt5BuildConfiguration::outFilePathChanged);
    updateProcessParameters();
}

PHPQt5QMakeBuildStep::DefaultBuildOptions PHPQt5QMakeBuildStep::defaultCompilerOptions() const
{
    return m_defaultOptions;
}

void PHPQt5QMakeBuildStep::setDefaultCompilerOptions(PHPQt5QMakeBuildStep::DefaultBuildOptions options)
{
    if (m_defaultOptions == options)
        return;
    m_defaultOptions = options;
    emit defaultCompilerOptionsChanged(options);
    updateProcessParameters();
}

BuildStepConfigWidget *PHPQt5QMakeBuildStep::createConfigWidget()
{
    return new PHPQt5QMakeBuildStepConfigWidget(this);
}

bool PHPQt5QMakeBuildStep::fromMap(const QVariantMap &map)
{
    AbstractProcessStep::fromMap(map);
    m_userCompilerOptions = map[Constants::C_PHPQt5QMAKEBUILDSTEP_USERCOMPILEROPTIONS].toString().split(QLatin1Char('|'));
    updateProcessParameters();
    return true;
}

QVariantMap PHPQt5QMakeBuildStep::toMap() const
{
    QVariantMap result = AbstractProcessStep::toMap();
    result[Constants::C_PHPQt5QMAKEBUILDSTEP_USERCOMPILEROPTIONS] = m_userCompilerOptions.join(QLatin1Char('|'));
    return result;
}

QStringList PHPQt5QMakeBuildStep::userCompilerOptions() const
{
    return m_userCompilerOptions;
}

void PHPQt5QMakeBuildStep::setUserCompilerOptions(const QStringList &options)
{
    m_userCompilerOptions = options;
    emit userCompilerOptionsChanged(options);
    updateProcessParameters();
}

FileName PHPQt5QMakeBuildStep::outFilePath() const
{
    return m_outFilePath;
}

void PHPQt5QMakeBuildStep::setOutFilePath(const FileName &outFilePath)
{
    if (outFilePath == m_outFilePath)
        return;
    m_outFilePath = outFilePath;
    emit outFilePathChanged(outFilePath);
}

void PHPQt5QMakeBuildStep::updateProcessParameters()
{
    updateOutFilePath();
    updateCommand();
    updateArguments();
    updateWorkingDirectory();
    updateEnvironment();
    emit processParametersChanged();
}

void PHPQt5QMakeBuildStep::updateOutFilePath()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);
    //const QString targetName = Utils::HostOsInfo::withExecutableSuffix(m_targetPHPQt5File.toFileInfo().baseName());
    //FileName outFilePath = bc->buildDirectory().appendPath(targetName);
    FileName outFilePath = bc->buildDirectory().appendPath("");
    setOutFilePath(outFilePath);
}

void PHPQt5QMakeBuildStep::updateCommand()
{
#ifdef Q_OS_WIN
        processParameters()->setCommand(QStringLiteral("qmake.exe"));

#else
        processParameters()->setCommand(QStringLiteral("qmake"));
#endif
}

void PHPQt5QMakeBuildStep::updateWorkingDirectory()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);
    processParameters()->setWorkingDirectory(bc->buildDirectory().toString());
}

void PHPQt5QMakeBuildStep::updateArguments()
{
    PHPQt5BuildConfiguration *bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);

    QStringList arguments;

    PHPQt5Project *p = qobject_cast<PHPQt5Project *>(project());

    arguments << QStringLiteral("\"%1/%2\"").arg(bc->buildDirectory().toString()).arg(p->projectFilePath().fileName().replace(QRegExp("\\.phpqt5$"), ".pro"));
    arguments << QStringLiteral("-spec win32-msvc2015");

    if (m_defaultOptions == Debug) {
      //  arguments << QStringLiteral("\"CONFIG+=debug\"");
      //  arguments << QStringLiteral("\"CONFIG+=qml_debug\"");
    }

   // arguments << QStringLiteral("&&");
   // arguments << QStringLiteral("jom.exe");
   // arguments << QStringLiteral("qmake_all");
   // arguments << QStringLiteral("&&");
   // arguments << QStringLiteral("jom.exe");

    arguments << m_userCompilerOptions;

    // Remove empty args
    auto predicate = [](const QString &str) { return str.isEmpty(); };
    auto it = std::remove_if(arguments.begin(), arguments.end(), predicate);
    arguments.erase(it, arguments.end());

    processParameters()->setArguments(arguments.join(QChar::Space));
}

void PHPQt5QMakeBuildStep::updateEnvironment()
{
    PHPQt5BuildConfiguration * bc = qobject_cast<PHPQt5BuildConfiguration *>(buildConfiguration());
    QTC_ASSERT(bc, return);

    Utils::Environment env = bc->environment();
    QDir applicationDirPath(QCoreApplication::applicationDirPath());
    applicationDirPath.cdUp();
    applicationDirPath.cdUp();
    applicationDirPath.cdUp();
    env.appendOrSetPath(applicationDirPath.canonicalPath() + "/PHPQt5");

    processParameters()->setEnvironment(env);
}

}



