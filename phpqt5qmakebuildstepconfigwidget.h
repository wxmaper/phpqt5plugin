#ifndef PHPQT5QMAKEBUILDSTEPCONFIGWIDGET_H
#define PHPQT5QMAKEBUILDSTEPCONFIGWIDGET_H


#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5QMakeBuildStep;

namespace Ui { class PHPQt5QMakeBuildStepConfigWidget; }

class PHPQt5QMakeBuildStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    PHPQt5QMakeBuildStepConfigWidget(PHPQt5QMakeBuildStep *buildStep);
    ~PHPQt5QMakeBuildStepConfigWidget();

    QString summaryText() const override;
    QString displayName() const override;

private:
    void updateUi();
    void updateCommandLineText();
    void updateAdditionalArgumentsLineEdit();

    void onAdditionalArgumentsTextEdited(const QString &text);

    PHPQt5QMakeBuildStep *m_buildStep;
    QScopedPointer<Ui::PHPQt5QMakeBuildStepConfigWidget> m_ui;
};

}

#endif // PHPQT5QMAKEBUILDSTEPCONFIGWIDGET_H
