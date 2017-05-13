#include "phpqt5makebuildstepconfigwidget.h"

#include "ui_phpqt5makebuildstepconfigwidget.h"
#include "phpqt5buildconfiguration.h"
#include "phpqt5makebuildstep.h"
#include "phpqt5project.h"

#include "phpqt5constants.h"

#include <utils/qtcassert.h>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5MakeBuildStepConfigWidget::PHPQt5MakeBuildStepConfigWidget(PHPQt5MakeBuildStep *buildStep)
    : BuildStepConfigWidget()
    , m_buildStep(buildStep)
    , m_ui(new Ui::PHPQt5MakeBuildStepConfigWidget())
{
    m_ui->setupUi(this);

    // Connect the project signals
    auto project = static_cast<PHPQt5Project *>(m_buildStep->project());
    connect(project, &PHPQt5Project::fileListChanged,
            this, &PHPQt5MakeBuildStepConfigWidget::updateUi);

    // Connect build step signals
    connect(m_buildStep, &PHPQt5MakeBuildStep::processParametersChanged,
            this, &PHPQt5MakeBuildStepConfigWidget::updateUi);

    // Connect UI signals
    connect(m_ui->additionalArgumentsLineEdit, &QLineEdit::textEdited,
            this, &PHPQt5MakeBuildStepConfigWidget::onAdditionalArgumentsTextEdited);
    connect(m_ui->defaultArgumentsComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, &PHPQt5MakeBuildStepConfigWidget::onDefaultArgumentsComboBoxIndexChanged);

    updateUi();
}

PHPQt5MakeBuildStepConfigWidget::~PHPQt5MakeBuildStepConfigWidget() = default;

QString PHPQt5MakeBuildStepConfigWidget::summaryText() const
{
    return tr(Constants::C_PHPQt5MAKEBUILDSTEPWIDGET_SUMMARY);
}

QString PHPQt5MakeBuildStepConfigWidget::displayName() const
{
    return tr(Constants::C_PHPQt5MAKEBUILDSTEPWIDGET_DISPLAY);
}

void PHPQt5MakeBuildStepConfigWidget::onDefaultArgumentsComboBoxIndexChanged(int index)
{
    auto options = static_cast<PHPQt5MakeBuildStep::DefaultBuildOptions>(index);
    m_buildStep->setDefaultCompilerOptions(options);
}

void PHPQt5MakeBuildStepConfigWidget::updateUi()
{
    updateCommandLineText();
    updateAdditionalArgumentsLineEdit();
    updateDefaultArgumentsComboBox();
}

void PHPQt5MakeBuildStepConfigWidget::onAdditionalArgumentsTextEdited(const QString &text)
{
    m_buildStep->setUserCompilerOptions(text.split(QChar::Space));
}

void PHPQt5MakeBuildStepConfigWidget::updateCommandLineText()
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

void PHPQt5MakeBuildStepConfigWidget::updateAdditionalArgumentsLineEdit()
{
    const QString text = m_buildStep->userCompilerOptions().join(QChar::Space);
    m_ui->additionalArgumentsLineEdit->setText(text);
}

void PHPQt5MakeBuildStepConfigWidget::updateDefaultArgumentsComboBox()
{
    const int index = m_buildStep->defaultCompilerOptions();
    m_ui->defaultArgumentsComboBox->setCurrentIndex(index);
}

}
