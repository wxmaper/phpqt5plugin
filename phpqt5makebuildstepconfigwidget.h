#ifndef PHPQT5MAKEBUILDSTEPWIDGET_H
#define PHPQT5MAKEBUILDSTEPWIDGET_H

#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5MakeBuildStep;

namespace Ui { class PHPQt5MakeBuildStepConfigWidget; }

class PHPQt5MakeBuildStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    PHPQt5MakeBuildStepConfigWidget(PHPQt5MakeBuildStep *buildStep);
    ~PHPQt5MakeBuildStepConfigWidget();

    QString summaryText() const override;
    QString displayName() const override;

private:
    void updateUi();
    void updateCommandLineText();
    void updateAdditionalArgumentsLineEdit();
    void updateDefaultArgumentsComboBox();

    void onAdditionalArgumentsTextEdited(const QString &text);
    void onDefaultArgumentsComboBoxIndexChanged(int index);

    PHPQt5MakeBuildStep *m_buildStep;
    QScopedPointer<Ui::PHPQt5MakeBuildStepConfigWidget> m_ui;
};

}

#endif // PHPQT5MAKEBUILDSTEPWIDGET_H
