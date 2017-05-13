#ifndef PHPQT5MAKECLEANSTEPCONFIGWIDGET_H
#define PHPQT5MAKECLEANSTEPCONFIGWIDGET_H

#include <projectexplorer/buildstep.h>

namespace PHPQt5 {

class PHPQt5MakeCleanStep;

namespace Ui { class PHPQt5MakeCleanStepConfigWidget; }

class PHPQt5MakeCleanStepConfigWidget : public ProjectExplorer::BuildStepConfigWidget
{
    Q_OBJECT

public:
    PHPQt5MakeCleanStepConfigWidget(PHPQt5MakeCleanStep *cleanStep);

    ~PHPQt5MakeCleanStepConfigWidget();

    QString summaryText() const override;

    QString displayName() const override;

private:
    void updateUi();

    PHPQt5MakeCleanStep *m_cleanStep;
    QScopedPointer<Ui::PHPQt5MakeCleanStepConfigWidget> m_ui;
};

}


#endif // PHPQT5MAKECLEANSTEPCONFIGWIDGET_H
