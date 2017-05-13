#ifndef PHPQT5JOMBUILDSTEPCONFIGWIDGET_H
#define PHPQT5JOMBUILDSTEPCONFIGWIDGET_H

#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5JomBuildStep;

namespace Ui { class PHPQt5JomBuildStepConfigWidget; }

class PHPQt5JomBuildStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    PHPQt5JomBuildStepConfigWidget(PHPQt5JomBuildStep *buildStep);
    ~PHPQt5JomBuildStepConfigWidget();

    QString summaryText() const override;
    QString displayName() const override;

private:
    void updateUi();
    void updateCommandLineText();
    void updateAdditionalArgumentsLineEdit();

    void onAdditionalArgumentsTextEdited(const QString &text);

    PHPQt5JomBuildStep *m_buildStep;
    QScopedPointer<Ui::PHPQt5JomBuildStepConfigWidget> m_ui;
};

}

#endif // PHPQT5JOMBUILDSTEPCONFIGWIDGET_H
