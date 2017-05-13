#include "phpqt5makecleanstepconfigwidget.h"

#include "ui_phpqt5makecleanstepconfigwidget.h"
#include "phpqt5makecleanstep.h"

#include "phpqt5constants.h"

#include "projectexplorer/buildconfiguration.h"

using namespace ProjectExplorer;

namespace PHPQt5 {

PHPQt5MakeCleanStepConfigWidget::PHPQt5MakeCleanStepConfigWidget(PHPQt5MakeCleanStep *cleanStep)
    : BuildStepConfigWidget()
    , m_cleanStep(cleanStep)
    , m_ui(new Ui::PHPQt5MakeCleanStepConfigWidget())
{
    m_ui->setupUi(this);
    connect(cleanStep->buildConfiguration(), &BuildConfiguration::buildDirectoryChanged,
            this, &PHPQt5MakeCleanStepConfigWidget::updateUi);
    updateUi();
}

PHPQt5MakeCleanStepConfigWidget::~PHPQt5MakeCleanStepConfigWidget() = default;

QString PHPQt5MakeCleanStepConfigWidget::summaryText() const
{
    return tr(Constants::C_PHPQt5MAKECLEANSTEPWIDGET_SUMMARY);
}

QString PHPQt5MakeCleanStepConfigWidget::displayName() const
{
    return tr(Constants::C_PHPQt5MAKECLEANSTEPWIDGET_DISPLAY);
}

void PHPQt5MakeCleanStepConfigWidget::updateUi()
{
    auto buildDiretory = m_cleanStep->buildConfiguration()->buildDirectory();
    m_ui->workingDirectoryLineEdit->setText(buildDiretory.toString());
}

}

