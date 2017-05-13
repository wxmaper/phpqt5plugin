#include "phpqt5buildconfigurationwidget.h"
#include "phpqt5buildconfiguration.h"

#include "phpqt5constants.h"

#include <utils/pathchooser.h>
#include <utils/detailswidget.h>

#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

PHPQt5BuildConfigurationWidget::PHPQt5BuildConfigurationWidget(PHPQt5BuildConfiguration *bc)
    : NamedWidget(nullptr)
    , m_bc(bc)
{
    // Build UI
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);

    auto detailsWidget = new DetailsWidget();
    detailsWidget->setState(DetailsWidget::NoSummary);
    mainLayout->addWidget(detailsWidget);

    auto detailsInnerWidget = new QWidget();
    auto formLayout = new QFormLayout(detailsInnerWidget);
    detailsWidget->setWidget(detailsInnerWidget);

    m_buildDirectoryChooser = new PathChooser();
    formLayout->addRow(tr("Build directory:"), m_buildDirectoryChooser);

    // Connect signals
    connect(bc, &PHPQt5BuildConfiguration::buildDirectoryChanged,
            this, &PHPQt5BuildConfigurationWidget::updateUi);
    connect(m_buildDirectoryChooser, &PathChooser::pathChanged,
            this, &PHPQt5BuildConfigurationWidget::onPathEdited);

    setDisplayName(tr(Constants::C_PHPQt5BUILDCONFIGURATIONWIDGET_DISPLAY));
    updateUi();
}

PHPQt5BuildConfigurationWidget::~PHPQt5BuildConfigurationWidget() = default;

void PHPQt5BuildConfigurationWidget::updateUi()
{
    m_buildDirectoryChooser->setPath(m_bc->buildDirectory().toUserOutput());
}

void PHPQt5BuildConfigurationWidget::onPathEdited(const QString &path)
{
    m_bc->setBuildDirectory(FileName::fromUserInput(path));
}


}
