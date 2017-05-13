#pragma once

#include "phpqt5_global.h"

#include <extensionsystem/iplugin.h>

namespace PHPQt5 {
namespace Internal {

class PHPQt5Plugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "PHPQt5.json")

public:
    PHPQt5Plugin();
    ~PHPQt5Plugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void triggerAction();
};

} // namespace Internal
} // namespace PHPQt5
