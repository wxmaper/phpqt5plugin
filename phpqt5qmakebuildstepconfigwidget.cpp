#include "phpqt5qmakebuildstepconfigwidget.h"

#include "ui_phpqt5qmakebuildstepconfigwidget.h"
#include "phpqt5buildconfiguration.h"
#include "PHPQt5QMakebuildstep.h"
#include "phpqt5project.h"

#include "phpqt5constants.h"

#include <utils/qtcassert.h>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5QMakeBuildStepConfigWidget::PHPQt5QMakeBuildStepConfigWidget(PHPQt5QMakeBuildStep *buildStep)
    : BuildStepConfigWidget()
    , m_buildStep(buildStep)
    , m_ui(new Ui::PHPQt5QMakeBuildStepConfigWidget())
{
    m_ui->setupUi(this);

    // Connect the project signals
    auto project = static_cast<PHPQt5Project *>(m_buildStep->project());
    connect(project, &PHPQt5Project::fileListChanged,
            this, &PHPQt5QMakeBuildStepConfigWidget::updateUi);

    // Connect build step signals
    connect(m_buildStep, &PHPQt5QMakeBuildStep::processParametersChanged,
            this, &PHPQt5QMakeBuildStepConfigWidget::updateUi);

    // Connect UI signals
    connect(m_ui->additionalArgumentsLineEdit, &QLineEdit::textEdited,
            this, &PHPQt5QMakeBuildStepConfigWidget::onAdditionalArgumentsTextEdited);

    updateUi();
}

PHPQt5QMakeBuildStepConfigWidget::~PHPQt5QMakeBuildStepConfigWidget() = default;

QString PHPQt5QMakeBuildStepConfigWidget::summaryText() const
{
    return tr(Constants::C_PHPQt5QMAKEBUILDSTEPWIDGET_SUMMARY);
}

QString PHPQt5QMakeBuildStepConfigWidget::displayName() const
{
    return tr(Constants::C_PHPQt5QMAKEBUILDSTEPWIDGET_DISPLAY);
}

void PHPQt5QMakeBuildStepConfigWidget::updateUi()
{
    updateCommandLineText();
    updateAdditionalArgumentsLineEdit();
}

void PHPQt5QMakeBuildStepConfigWidget::onAdditionalArgumentsTextEdited(const QString &text)
{
    m_buildStep->setUserCompilerOptions(text.split(QChar::Space));
}

void PHPQt5QMakeBuildStepConfigWidget::updateCommandLineText()
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

void PHPQt5QMakeBuildStepConfigWidget::updateAdditionalArgumentsLineEdit()
{
    const QString text = m_buildStep->userCompilerOptions().join(QChar::Space);
    m_ui->additionalArgumentsLineEdit->setText(text);
}

}
