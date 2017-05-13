#include "phpqt5formwizarddialog.h"
#include "ui_phpqt5formwizardpage.h"

#include <QFileDialog>


namespace PHPQt5 {
namespace Internal {

PHPQt5FormWizardPage::PHPQt5FormWizardPage(QWidget *parent)
    : QWizardPage(parent),
      ui(new Ui::PHPQt5FormWizardPage)
{
    ui->setupUi(this);
    ui->lineEdit_className->setValidator(new QRegExpValidator(QRegExp(QLatin1String("[a-zA-Z_][a-zA-Z_0-9]*")),
                                                              this));
    emit completeChanged(false);
}

PHPQt5FormWizardPage::~PHPQt5FormWizardPage()
{
    delete ui;
}

bool PHPQt5FormWizardPage::validatePage()
{
    QFileInfo pathInfo(ui->lineEdit_path->text());
    m_params.clear();

    if (!(pathInfo.exists() && pathInfo.isAbsolute() && pathInfo.isDir())) {
        ui->lineEdit_path->setStyleSheet(QLatin1String("color: red;"));
        ui->label_info->setText(QStringLiteral("Please correct errors"));

        emit completeChanged(false);

        return false;
    }

    QFileInfo uiInfo(QStringLiteral("%1/forms/%2.ui")
                     .arg(ui->lineEdit_path->text()).arg(ui->lineEdit_className->text().toLower()));

    QFileInfo phpInfo(QStringLiteral("%1/scripts/%2.php")
                      .arg(ui->lineEdit_path->text()).arg(ui->lineEdit_className->text().toLower()));

    if (uiInfo.exists() || phpInfo.exists()) {
        ui->lineEdit_className->setStyleSheet(QLatin1String("color: red;"));
        ui->label_info->setText(QStringLiteral("File \"%1\" currentrly exsists.")
                                .arg(uiInfo.exists()
                                     ? uiInfo.absoluteFilePath()
                                     : phpInfo.absoluteFilePath()));

        emit completeChanged(false);

        return false;
    }

    ui->lineEdit_path->setStyleSheet(QLatin1String(""));
    ui->lineEdit_className->setStyleSheet(QLatin1String(""));

    ui->label_info->setText(QStringLiteral("This wizard will create who files:"
                                           "<br/>&nbsp;&nbsp;&nbsp;&nbsp;- %1"
                                           "<br/>&nbsp;&nbsp;&nbsp;&nbsp;- %2")
                            .arg(QDir::toNativeSeparators(
                                     QStringLiteral("%1/forms/%2.ui").arg(pathInfo.absolutePath())
                                                                          .arg(ui->lineEdit_className->text().toLower())))
                            .arg(QDir::toNativeSeparators(
                                     QStringLiteral("%1/scripts/%2.ui").arg(pathInfo.absolutePath())
                                                                            .arg(ui->lineEdit_className->text().toLower()))));

    m_params.phpPath = phpInfo;
    m_params.uiPath = uiInfo;
    m_params.path = pathInfo;
    m_params.className = ui->lineEdit_className->text();
    m_params.baseClass = ui->comboBox_baseClass->currentText();

    emit completeChanged(true);

    return true;
}

void PHPQt5FormWizardPage::on_lineEdit_className_textChanged(const QString &)
{
    validatePage();
}

bool PHPQt5FormWizardPage::isComplete()
{
    return validatePage();
}

void PHPQt5FormWizardPage::on_pushButton_browse_clicked()
{
    const QString path = QFileDialog::getExistingDirectory(this,
                                                           tr("Select path"));

    if (!path.isEmpty()) {
        ui->lineEdit_path->setText(QDir::toNativeSeparators(path));
    }
}

void PHPQt5FormWizardPage::on_lineEdit_path_textChanged(const QString &)
{
    validatePage();
}

PHPQt5FormWizardDialog::PHPQt5FormWizardDialog(const Core::BaseFileWizardFactory *factory,
                                               const Core::WizardDialogParameters &parameters) :
    Core::BaseFileWizard(factory, QVariantMap())
{
    m_page = new PHPQt5FormWizardPage(this);
    m_page->setDefaultPath(parameters.defaultPath());
    addPage(m_page);

    connect(m_page, SIGNAL(completeChanged(bool)),
            this, SLOT(onCompleteChanged(bool)));

    QAbstractButton *button = this->button(QWizard::FinishButton);
    button->setEnabled(false);
}

PHPQt5FormWizardDialog::~PHPQt5FormWizardDialog()
{

}

bool PHPQt5FormWizardDialog::validateCurrentPage()
{
    return true;
}

const PHPQt5FormParameters PHPQt5FormWizardDialog::parameters() const
{
    return m_page->params();
}

void PHPQt5FormWizardDialog::onCompleteChanged(bool c)
{
    QAbstractButton *button = this->button(QWizard::FinishButton);
    button->setEnabled(c);
}

void PHPQt5FormWizardPage::setDefaultPath(const QString &defaultPath)
{
    ui->lineEdit_path->setText(QDir::toNativeSeparators(defaultPath));
}

const QString PHPQt5FormWizardPage::path() const
{
    return ui->lineEdit_path->text();
}

const QString PHPQt5FormWizardPage::className() const
{
    return ui->lineEdit_className->text();
}

const QString PHPQt5FormWizardPage::baseClass() const
{
    return ui->comboBox_baseClass->currentText();
}

const PHPQt5FormParameters PHPQt5FormWizardPage::params() const
{
    return m_params;
}

void PHPQt5FormWizardPage::on_comboBox_baseClass_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    validatePage();
}

}
}



