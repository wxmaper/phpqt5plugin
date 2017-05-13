#include "phpqt5jombuildstepconfigwidget.h"

#include "ui_phpqt5jombuildstepconfigwidget.h"
#include "../phpqt5buildconfiguration.h"
#include "PHPQt5Jombuildstep.h"
#include "../phpqt5project.h"

#include "../phpqt5constants.h"

#include <utils/qtcassert.h>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5JomBuildStepConfigWidget::PHPQt5JomBuildStepConfigWidget(PHPQt5JomBuildStep *buildStep)
    : BuildStepConfigWidget()
    , m_buildStep(buildStep)
    , m_ui(new Ui::PHPQt5JomBuildStepConfigWidget())
{
    m_ui->setupUi(this);

    // Connect the project signals
    auto project = static_cast<PHPQt5Project *>(m_buildStep->project());
    connect(project, &PHPQt5Project::fileListChanged,
            this, &PHPQt5JomBuildStepConfigWidget::updateUi);

    // Connect build step signals
    connect(m_buildStep, &PHPQt5JomBuildStep::processParametersChanged,
            this, &PHPQt5JomBuildStepConfigWidget::updateUi);

    // Connect UI signals
    connect(m_ui->additionalArgumentsLineEdit, &QLineEdit::textEdited,
            this, &PHPQt5JomBuildStepConfigWidget::onAdditionalArgumentsTextEdited);

    updateUi();
}

PHPQt5JomBuildStepConfigWidget::~PHPQt5JomBuildStepConfigWidget() = default;

QString PHPQt5JomBuildStepConfigWidget::summaryText() const
{
    return tr(Constants::C_PHPQt5JOMBUILDSTEPWIDGET_SUMMARY);
}

QString PHPQt5JomBuildStepConfigWidget::displayName() const
{
    return tr(Constants::C_PHPQt5JOMBUILDSTEPWIDGET_DISPLAY);
}

void PHPQt5JomBuildStepConfigWidget::updateUi()
{
    updateCommandLineText();
    updateAdditionalArgumentsLineEdit();
}

void PHPQt5JomBuildStepConfigWidget::onAdditionalArgumentsTextEdited(const QString &text)
{
    m_buildStep->setUserCompilerOptions(text.split(QChar::Space));
}

void PHPQt5JomBuildStepConfigWidget::updateCommandLineText()
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

void PHPQt5JomBuildStepConfigWidget::updateAdditionalArgumentsLineEdit()
{
    const QString text = m_buildStep->userCompilerOptions().join(QChar::Space);
    m_ui->additionalArgumentsLineEdit->setText(text);
}

}
