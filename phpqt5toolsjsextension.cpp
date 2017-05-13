#include "phpqt5toolsjsextension.h"

#include <projectexplorer/project.h>

namespace PHPQt5 {
namespace Internal {


PHPQt5ToolsJsExtension::PHPQt5ToolsJsExtension(QObject *parent)
    : QObject(parent) { }

QString PHPQt5ToolsJsExtension::formsPath(const QString &path) const
{
    return path;
}

QString PHPQt5ToolsJsExtension::scriptsPath(const QString &path) const
{
    return path;
}


}
}
