#ifndef PHPQT5TOOLSJSEXTENSION_H
#define PHPQT5TOOLSJSEXTENSION_H

#include <QObject>

namespace PHPQt5 {
namespace Internal {


/**
 * This class extends the JS features in our macro expander.
 */
class PHPQt5ToolsJsExtension : public QObject
{
    Q_OBJECT

public:
    PHPQt5ToolsJsExtension(QObject *parent = 0);

    Q_INVOKABLE QString formsPath(const QString &path) const;
    Q_INVOKABLE QString scriptsPath(const QString &path) const;
};


}
}
#endif // PHPQT5TOOLSJSEXTENSION_H
