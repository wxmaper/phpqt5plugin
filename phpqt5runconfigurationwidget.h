#ifndef PHPQT5RUNCONFIGURATIONWIDGET_H
#define PHPQT5RUNCONFIGURATIONWIDGET_H

#include <QWidget>

namespace PHPQt5 {

class PHPQt5RunConfiguration;

class PHPQt5RunConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PHPQt5RunConfigurationWidget(PHPQt5RunConfiguration *rc, QWidget *parent = 0);

private:
    void updateUi();

    PHPQt5RunConfiguration* m_rc;
};

}


#endif // PHPQT5RUNCONFIGURATIONWIDGET_H
