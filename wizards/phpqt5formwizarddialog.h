#ifndef PHPQT5FORMWIZARDDIALOG_H
#define PHPQT5FORMWIZARDDIALOG_H

#include <coreplugin/basefilewizard.h>
#include <coreplugin/basefilewizardfactory.h>

#include <QFileInfo>

namespace Ui {
class PHPQt5FormWizardPage;
}

namespace PHPQt5 {
namespace Internal {

struct PHPQt5FormParameters {
    void clear() {
        path.setFile(""); phpPath.setFile(""); uiPath.setFile("");
        baseClass.clear(); className.clear();
    }

    QFileInfo path;
    QFileInfo phpPath;
    QFileInfo uiPath;
    QString baseClass;
    QString className;
};

class PHPQt5FormWizardPage : public QWizardPage {
    Q_OBJECT

public:
    explicit PHPQt5FormWizardPage(QWidget *parent = Q_NULLPTR);
    ~PHPQt5FormWizardPage();
    bool validatePage();
    bool isComplete();
    void setDefaultPath(const QString &defaultPath);

    const QString path() const;
    const QString className() const;
    const QString baseClass() const;
    const PHPQt5FormParameters params() const;

signals:
    void reject();
    void accept();
    void completeChanged(bool);

private slots:
    void on_pushButton_browse_clicked();
    void on_lineEdit_path_textChanged(const QString &);
    void on_lineEdit_className_textChanged(const QString &);

    void on_comboBox_baseClass_currentIndexChanged(int index);

private:
    Ui::PHPQt5FormWizardPage *ui;
    PHPQt5FormParameters m_params;
};

class PHPQt5FormWizardDialog : public Core::BaseFileWizard
{
    Q_OBJECT

public:
    explicit PHPQt5FormWizardDialog(const Core::BaseFileWizardFactory *factory,
                                    const Core::WizardDialogParameters &parameters);
    ~PHPQt5FormWizardDialog();

    const PHPQt5FormParameters parameters() const;
    bool validateCurrentPage() override;

public slots:
    void onCompleteChanged(bool c);

private:
    PHPQt5FormWizardPage *m_page;
};

}
}

#endif // PHPQT5FORMWIZARDDIALOG_H
