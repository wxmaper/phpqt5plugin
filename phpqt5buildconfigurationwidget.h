#ifndef PHPQT5BUILDCONFIGURATIONWIDGET_H
#define PHPQT5BUILDCONFIGURATIONWIDGET_H

#include <projectexplorer/namedwidget.h>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

namespace Utils { class PathChooser; }

namespace PHPQt5 {

class PHPQt5BuildConfiguration;

class PHPQt5BuildConfigurationWidget : public ProjectExplorer::NamedWidget
{
    Q_OBJECT

public:
    explicit PHPQt5BuildConfigurationWidget(PHPQt5BuildConfiguration *bc);
    ~PHPQt5BuildConfigurationWidget();

private:
    void updateUi();
    void onPathEdited(const QString &path);

    PHPQt5BuildConfiguration *m_bc;
    Utils::PathChooser *m_buildDirectoryChooser;
};

}


#endif // PHPQT5BUILDCONFIGURATIONWIDGET_H
