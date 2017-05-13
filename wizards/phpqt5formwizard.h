#ifndef PHPQT5FORMWIZARD_H
#define PHPQT5FORMWIZARD_H

#include "qmakeprojectmanager/wizards/qtwizard.h"
#include <projectexplorer/baseprojectwizarddialog.h>
#include <projectexplorer/customwizard/customwizard.h>

namespace PHPQt5 {
namespace Internal {

static const char C_PHPFILE_DATA[] = "<?php\n\n"
                                     "class %{ClassName} extends %{BaseClass} {\n"
                                     "    public function __construct($parent = null) {\n"
                                     "        parent::__construct($parent);\n"
                                     "        $this->ui = setupUi($this);\n"
                                     "    }\n"
                                     "}\n";

static const char C_UIFILE_DATA_WIDGET[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                           "<ui version=\"4.0\">\n"
                                           " <class>%{ClassName}</class>\n"
                                           " <widget class=\"QWidget\" name=\"%{ClassName}\">\n"
                                           "  <property name=\"geometry\">\n"
                                           "   <rect>\n"
                                           "    <x>0</x>\n"
                                           "    <y>0</y>\n"
                                           "    <width>400</width>\n"
                                           "    <height>300</height>\n"
                                           "   </rect>\n"
                                           "  </property>\n"
                                           "  <property name=\"windowTitle\">\n"
                                           "   <string>Form</string>\n"
                                           "  </property>\n"
                                           " </widget>\n"
                                           " <resources/>\n"
                                           " <connections/>\n"
                                           "</ui>\n";

static const char C_UIFILE_DATA_DIALOG[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                                           "<ui version=\"4.0\">"
                                           " <class>%{ClassName}</class>"
                                           " <widget class=\"QDialog\" name=\"%{ClassName}\">"
                                           "  <property name=\"geometry\">"
                                           "   <rect>"
                                           "    <x>0</x>"
                                           "    <y>0</y>"
                                           "    <width>400</width>"
                                           "    <height>300</height>"
                                           "   </rect>"
                                           "  </property>"
                                           "  <property name=\"windowTitle\">"
                                           "   <string>Dialog</string>"
                                           "  </property>"
                                           " </widget>"
                                           " <resources/>"
                                           " <connections/>"
                                           "</ui>";

static const char C_UIFILE_DATA_MAINWINDOW[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                                               "<ui version=\"4.0\">\n"
                                               " <class>%{ClassName}</class>\n"
                                               " <widget class=\"QMainWindow\" name=\"%{ClassName}\">\n"
                                               "  <property name=\"geometry\">\n"
                                               "   <rect>\n"
                                               "    <x>0</x>\n"
                                               "    <y>0</y>\n"
                                               "    <width>800</width>\n"
                                               "    <height>600</height>\n"
                                               "   </rect>\n"
                                               "  </property>\n"
                                               "  <property name=\"windowTitle\">\n"
                                               "   <string>MainWindow</string>\n"
                                               "  </property>\n"
                                               "  <widget class=\"QWidget\" name=\"centralwidget\"/>\n"
                                               "  <widget class=\"QMenuBar\" name=\"menubar\">\n"
                                               "   <property name=\"geometry\">\n"
                                               "    <rect>\n"
                                               "     <x>0</x>\n"
                                               "     <y>0</y>\n"
                                               "     <width>800</width>\n"
                                               "     <height>21</height>\n"
                                               "    </rect>\n"
                                               "   </property>\n"
                                               "  </widget>\n"
                                               "  <widget class=\"QStatusBar\" name=\"statusbar\"/>\n"
                                               " </widget>\n"
                                               " <resources/>\n"
                                               " <connections/>\n"
                                               "</ui>\n";

class PHPQt5FormWizard : public Core::BaseFileWizardFactory
{
    Q_OBJECT

public:
    PHPQt5FormWizard();

private:
    Core::BaseFileWizard *create(QWidget *parent, const Core::WizardDialogParameters &parameters) const;

    Core::GeneratedFiles generateFiles(const QWizard *w, QString *errorMessage) const;

private:

};

}
}
#endif // PHPQT5FORMWIZARD_H
