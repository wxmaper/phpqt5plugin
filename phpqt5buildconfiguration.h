#ifndef PHPQT5BUILDCONFIGURATION_H
#define PHPQT5BUILDCONFIGURATION_H

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/target.h>

#include <QPointer>

namespace PHPQt5 {

class PHPQt5Project;
class PHPQt5MakeBuildStep;

class PHPQt5BuildConfiguration : public ProjectExplorer::BuildConfiguration
{
    Q_OBJECT

public:
    PHPQt5BuildConfiguration(ProjectExplorer::Target *target);

    ProjectExplorer::NamedWidget *createConfigWidget() override;

    ProjectExplorer::BuildConfiguration::BuildType buildType() const override;

    void setBuildType(BuildConfiguration::BuildType type);

    bool fromMap(const QVariantMap &map) override;
    QVariantMap toMap() const override;

    Utils::FileName cacheDirectory() const;
    Utils::FileName outFilePath() const;
    Utils::FileName projectFilePath() const;

    static bool canRestore(const QVariantMap &map);

    bool hasPHPQt5MakeBuildStep() const;
    bool hasPHPQt5QMakeBuildStep() const;
    bool hasPHPQt5JomBuildStep() const;
    bool hasPHPQt5UicBuildStep() const;
    bool hasPHPQt5MakeCleanStep() const;

signals:
    void outFilePathChanged(const Utils::FileName &outFilePath);

private:
    const PHPQt5MakeBuildStep *phpqt5MakeBuildStep() const;
    BuildConfiguration::BuildType m_buildType = BuildConfiguration::BuildType::Unknown;
};

}

#endif // PHPQT5BUILDCONFIGURATION_H
