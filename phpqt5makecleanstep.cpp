#include "phpqt5makecleanstep.h"

#include "phpqt5buildconfiguration.h"
#include "phpqt5makecleanstepconfigwidget.h"

#include "phpqt5constants.h"

#include <utils/qtcassert.h>

#include <QDir>
#include <QDateTime>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5MakeCleanStep::PHPQt5MakeCleanStep(BuildStepList *parentList)
    : BuildStep(parentList, Constants::C_PHPQt5MAKECLEANSTEP_ID)
{
    setDefaultDisplayName(tr("PHPQt5Make Clean Step"));
    setDisplayName(tr("PHPQt5Make Clean Step"));
}

BuildStepConfigWidget *PHPQt5MakeCleanStep::createConfigWidget()
{
    return new PHPQt5MakeCleanStepConfigWidget(this);
}

bool PHPQt5MakeCleanStep::init(QList<const BuildStep *> &)
{
    FileName buildDir = buildConfiguration()->buildDirectory();
    bool result = buildDir.exists();
    if (result)
        m_buildDir = buildDir;
    return result;
}

void PHPQt5MakeCleanStep::run(QFutureInterface<bool> &fi)
{
    if (!m_buildDir.exists()) {
        emit addOutput(tr("Build directory \"%1\" does not exist.").arg(m_buildDir.toUserOutput()), BuildStep::ErrorMessageOutput);
        reportRunResult(fi, false);
        return;
    }

    if (!removeCacheDirectory()) {
        emit addOutput(tr("Failed to delete the cache directory."), BuildStep::ErrorMessageOutput);
        reportRunResult(fi, false);
        return;
    }

    if (!removeOutFilePath()) {
        emit addOutput(tr("Failed to delete the out file."), BuildStep::ErrorMessageOutput);
        reportRunResult(fi, false);
        return;
    }

    emit addOutput(tr("Clean step completed successfully."), BuildStep::NormalOutput);
    reportRunResult(fi, true);
}

bool PHPQt5MakeCleanStep::removeCacheDirectory()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration*>(buildConfiguration());
    QTC_ASSERT(bc, return false);
    if (!bc->cacheDirectory().exists())
        return true;
    QDir dir = QDir::fromNativeSeparators(bc->cacheDirectory().toString());
    const QString dirName = dir.dirName();
    if (!dir.cdUp())
        return false;
    const QString newName = QStringLiteral("%1.bkp.%2").arg(dirName, QString::number(QDateTime::currentMSecsSinceEpoch()));
    return dir.rename(dirName, newName);
}

bool PHPQt5MakeCleanStep::removeOutFilePath()
{
    auto bc = qobject_cast<PHPQt5BuildConfiguration*>(buildConfiguration());
    QTC_ASSERT(bc, return false);
    if (!bc->outFilePath().exists())
        return true;
    return QFile(bc->outFilePath().toFileInfo().absoluteFilePath()).remove();
}

}

