#ifndef PHPQT5UICBUILDSTEPCONFIGWIDGET_H
#define PHPQT5UICBUILDSTEPCONFIGWIDGET_H

#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5UicBuildStep;

namespace Ui { class PHPQt5UicBuildStepConfigWidget; }

class PHPQt5UicBuildStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    PHPQt5UicBuildStepConfigWidget(PHPQt5UicBuildStep *buildStep);
    ~PHPQt5UicBuildStepConfigWidget();

    QString summaryText() const override;
    QString displayName() const override;

private:
    void updateUi();
    void updateCommandLineText();
    void updateAdditionalArgumentsLineEdit();

    void onAdditionalArgumentsTextEdited(const QString &text);

    PHPQt5UicBuildStep *m_buildStep;
    QScopedPointer<Ui::PHPQt5UicBuildStepConfigWidget> m_ui;
};

}

#endif // PHPQT5UICBUILDSTEPCONFIGWIDGET_H
