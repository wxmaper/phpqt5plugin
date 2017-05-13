#include "phpqt5runconfigurationwidget.h"
#include "phpqt5runconfiguration.h"

#include <projectexplorer/runconfigurationaspects.h>

#include <QFormLayout>

using namespace ProjectExplorer;

namespace PHPQt5 {

PHPQt5RunConfigurationWidget::PHPQt5RunConfigurationWidget(PHPQt5RunConfiguration *rc,
                                                     QWidget *parent)
    : QWidget(parent)
    , m_rc(rc)
{
    QTC_ASSERT(rc, return);
    auto fl = new QFormLayout(this);
    fl->setMargin(0);
    fl->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    rc->extraAspect<ArgumentsAspect>()->addToMainConfigurationWidget(this, fl);
    rc->extraAspect<TerminalAspect>()->addToMainConfigurationWidget(this,fl);
    connect(rc, &PHPQt5RunConfiguration::enabledChanged, this, &PHPQt5RunConfigurationWidget::updateUi);
    updateUi();
}


void PHPQt5RunConfigurationWidget::updateUi()
{
    setEnabled(m_rc->isEnabled());
}

}
