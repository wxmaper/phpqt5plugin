#ifndef PHPQT5JOMBUILDSTEP_H
#define PHPQT5JOMBUILDSTEP_H

#include <projectexplorer/abstractprocessstep.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildstep.h>
#include <projectexplorer/buildsteplist.h>

namespace PHPQt5 {

class PHPQt5JomBuildStep : public ProjectExplorer::AbstractProcessStep
{
    Q_OBJECT

public:
    enum DefaultBuildOptions { Empty = 0, Debug, Release};

    PHPQt5JomBuildStep(ProjectExplorer::BuildStepList *parentList);

    ProjectExplorer::BuildStepConfigWidget *createConfigWidget() override;

    bool fromMap(const QVariantMap &map) override;
    QVariantMap toMap() const override;

    QStringList userCompilerOptions() const;
    void setUserCompilerOptions(const QStringList &options);

    Utils::FileName outFilePath() const;

    PHPQt5JomBuildStep::DefaultBuildOptions defaultCompilerOptions() const;
    void setDefaultCompilerOptions(PHPQt5JomBuildStep::DefaultBuildOptions options);

signals:
    void userCompilerOptionsChanged(const QStringList &options);
    void defaultCompilerOptionsChanged(DefaultBuildOptions options);
    void processParametersChanged();
    void outFilePathChanged(const Utils::FileName &outFilePath);

private:
    void setOutFilePath(const Utils::FileName &outFilePath);

    void updateOutFilePath();
    void updateProcessParameters();
    void updateCommand();
    void updateWorkingDirectory();
    void updateArguments();
    void updateEnvironment();

    DefaultBuildOptions m_defaultOptions;
    QStringList m_userCompilerOptions;
    Utils::FileName m_outFilePath;
};

}
#endif // PHPQT5JOMBUILDSTEP_H
