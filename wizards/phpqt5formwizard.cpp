#include "phpqt5formwizard.h"
#include "phpqt5formwizarddialog.h"

#include <coreplugin/basefilewizard.h>

#include <QDir>

using namespace Core;

namespace PHPQt5 {
namespace Internal {

PHPQt5FormWizard::PHPQt5FormWizard()
{
    setId("P.PHPQt5Form");
    setCategory("O.PHPQt5");
    setDisplayCategory("PHPQt5");
    setDisplayName(tr("Qt Designer Form"));
    setDescription(tr("Creates a Qt Designer form that you can add to a PHPQt5 Project. "
                      "This is useful if you already have an existing "
                      "class for the UI business logic."));
    setIcon(QIcon(QLatin1String(":/wizards/images/gui.png")));
}

Core::BaseFileWizard *PHPQt5FormWizard::create(QWidget *parent,
                                               const Core::WizardDialogParameters &parameters) const
{
    PHPQt5FormWizardDialog *w = new PHPQt5FormWizardDialog(this, parameters);
    return w;
}

GeneratedFiles PHPQt5FormWizard::generateFiles(const QWizard *w, QString *errorMessage) const
{
    const PHPQt5FormWizardDialog *dialog = qobject_cast<const PHPQt5FormWizardDialog *>(w);
    const PHPQt5FormParameters params = dialog->parameters();

    // Как узнать путь до проекта? :-/
    QDir pd;
    QDir d(params.uiPath.absoluteDir());
    qDebug() << Q_FUNC_INFO << d;
    while (pd != d) {
        QFileInfoList i = d.entryInfoList({ "*.phpqt5" },
                                          QDir::Files|
                                          QDir::NoDotAndDotDot);
        qDebug() << d;
        qDebug() << i.size() ;
        if (i.size() == 0) {
            pd = d;
            d.cdUp();
        }
        else break;
    }

    qDebug() << pd << d;

    GeneratedFiles files;
    if (pd != d) {
        QString uiSrcPath = params.uiPath.absoluteFilePath().mid(d.absolutePath().length());
        GeneratedFile phpFile(params.phpPath.absoluteFilePath());
        phpFile.setContents(QString(C_PHPFILE_DATA)
                            .replace("%{ClassName}", params.className)
                            .replace("%{BaseClass}", params.baseClass)
                            .replace("%{UiSrcPath}", uiSrcPath));
        GeneratedFile uiFile(params.uiPath.absoluteFilePath());

        if (params.baseClass == "QDialog") {
            uiFile.setContents(QString(C_UIFILE_DATA_DIALOG)
                               .replace("%{ClassName}", params.className)
                               .replace("%{BaseClass}", params.baseClass)
                               .replace("%{UiSrcPath}", uiSrcPath));
        }
        else if (params.baseClass == "QMainWindow") {
            uiFile.setContents(QString(C_UIFILE_DATA_MAINWINDOW)
                               .replace("%{ClassName}", params.className)
                               .replace("%{BaseClass}", params.baseClass)
                               .replace("%{UiSrcPath}", uiSrcPath));
        }
        else {
            uiFile.setContents(QString(C_UIFILE_DATA_WIDGET)
                               .replace("%{ClassName}", params.className)
                               .replace("%{BaseClass}", params.baseClass)
                               .replace("%{UiSrcPath}", uiSrcPath));
        }

        phpFile.setAttributes(GeneratedFile::OpenEditorAttribute);
        uiFile.setAttributes(GeneratedFile::OpenEditorAttribute);

        files.append(phpFile);
        files.append(uiFile);
    }
    else {
        if (errorMessage)
            *errorMessage = QStringLiteral("Can't found PHPQt5 project file.");
    }

    return files;
}

}
}
