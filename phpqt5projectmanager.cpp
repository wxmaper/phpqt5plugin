#include "phpqt5projectmanager.h"
#include "phpqt5project.h"
#include "phpqt5constants.h"

using namespace ProjectExplorer;

namespace PHPQt5 {

QString PHPQt5ProjectManager::mimeType() const
{
    return QLatin1String(Constants::C_PHPQT5_PROJECT_MIMETYPE);
}

Project *PHPQt5ProjectManager::openProject(const QString &fileName, QString *errorString)
{
    if (!QFileInfo(fileName).isFile()) {
        *errorString = tr("Failed opening project \"%1\": Project is not a file.").arg(fileName);
        return nullptr;
    }

    return new PHPQt5Project(this, fileName);
}

}
