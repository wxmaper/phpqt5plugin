#ifndef PHPQT5MAKECLEANSTEP_H
#define PHPQT5MAKECLEANSTEP_H

#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>
#include <utils/fileutils.h>

namespace PHPQt5 {

class PHPQt5MakeCleanStep : public ProjectExplorer::BuildStep
{
    Q_OBJECT

public:
    PHPQt5MakeCleanStep(ProjectExplorer::BuildStepList *parentList);

    ProjectExplorer::BuildStepConfigWidget *createConfigWidget() override;

    bool init(QList<const BuildStep *> &earlierSteps) override;

    void run(QFutureInterface<bool> &fi) override;

private:
    bool removeCacheDirectory();
    bool removeOutFilePath();

    Utils::FileName m_buildDir;
};

}


#endif // PHPQT5MAKECLEANSTEP_H
