#include "phpqt5uicbuildstepconfigwidget.h"

#include "ui_phpqt5uicbuildstepconfigwidget.h"
#include "../phpqt5buildconfiguration.h"
#include "PHPQt5Uicbuildstep.h"
#include "../phpqt5project.h"

#include "../phpqt5constants.h"

#include <utils/qtcassert.h>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5UicBuildStepConfigWidget::PHPQt5UicBuildStepConfigWidget(PHPQt5UicBuildStep *buildStep)
    : BuildStepConfigWidget()
    , m_buildStep(buildStep)
    , m_ui(new Ui::PHPQt5UicBuildStepConfigWidget())
{
    m_ui->setupUi(this);

    // Connect the project signals
    auto project = static_cast<PHPQt5Project *>(m_buildStep->project());
    connect(project, &PHPQt5Project::fileListChanged,
            this, &PHPQt5UicBuildStepConfigWidget::updateUi);

    // Connect build step signals
    connect(m_buildStep, &PHPQt5UicBuildStep::processParametersChanged,
            this, &PHPQt5UicBuildStepConfigWidget::updateUi);

    // Connect UI signals
    connect(m_ui->additionalArgumentsLineEdit, &QLineEdit::textEdited,
            this, &PHPQt5UicBuildStepConfigWidget::onAdditionalArgumentsTextEdited);

    updateUi();
}

PHPQt5UicBuildStepConfigWidget::~PHPQt5UicBuildStepConfigWidget() = default;

QString PHPQt5UicBuildStepConfigWidget::summaryText() const
{
    return tr(Constants::C_PHPQt5UICBUILDSTEPWIDGET_SUMMARY);
}

QString PHPQt5UicBuildStepConfigWidget::displayName() const
{
    return tr(Constants::C_PHPQt5UICBUILDSTEPWIDGET_DISPLAY);
}

void PHPQt5UicBuildStepConfigWidget::updateUi()
{
    updateCommandLineText();
    updateAdditionalArgumentsLineEdit();
}

void PHPQt5UicBuildStepConfigWidget::onAdditionalArgumentsTextEdited(const QString &text)
{
    m_buildStep->setUserCompilerOptions(text.split(QChar::Space));
}

void PHPQt5UicBuildStepConfigWidget::updateCommandLineText()
{
    ProcessParameters *parameters = m_buildStep->processParameters();

    QStringList command;
    command << parameters->command();
    command << parameters->arguments();

    // Remove empty args
    auto predicate = [](const QString & str) { return str.isEmpty(); };
    auto it = std::remove_if(command.begin(), command.end(), predicate);
    command.erase(it, command.end());

    m_ui->commandTextEdit->setText(command.join(QChar::LineFeed));
}

void PHPQt5UicBuildStepConfigWidget::updateAdditionalArgumentsLineEdit()
{
    const QString text = m_buildStep->userCompilerOptions().join(QChar::Space);
    m_ui->additionalArgumentsLineEdit->setText(text);
}

}
