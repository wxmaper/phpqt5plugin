#ifndef PHPQT5DEBUGGER_H
#define PHPQT5DEBUGGER_H

#include "../phpqt5_global.h"
#include  <coreplugin/ioutputpane.h>

namespace Utils { class FileName; }
namespace PHPQt5 {

class PHPQT5SHARED_EXPORT PHPQt5OutputWindow : public Core::IOutputPane
{
    Q_OBJECT

public:
    ~PHPQt5OutputWindow() override;

    QWidget *outputWidget(QWidget *parent) override;
    QList<QWidget *> toolBarWidgets() const override;
    QString displayName() const override;

    int priorityInStatusBar() const override;

    void clearContents() override;
    void visibilityChanged(bool visible) override;

    void setFocus() override;
    bool hasFocus() const override;
    bool canFocus() const override;

    bool canNavigate() const override;
    bool canNext() const override;
    bool canPrevious() const override;
    void goToNext() override;
    void goToPrev() override;

    static PHPQt5OutputWindow *instance();

    enum MessageStyle {
        None,
        Error, // Red error text
        Warning, // Dark yellow warning text
        Command, // A bold command with timetamp "10:00 " + "Executing: vcs -diff"
        Message, // A blue message text (e.g. "command has finished successfully")
    };

    void appendText(const QString &text);
public slots:
    void incommingConnection();
    void readDebugData();

private:
    PHPQt5OutputWindow();
    QString m_debugDataBuffer;
};

} // namespace VcsBase


#endif // PHPQT5DEBUGGER_H
