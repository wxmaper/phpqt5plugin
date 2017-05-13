#include "phpqt5plugin.h"
#include "phpqt5constants.h"
#include "phpqt5projectmanager.h"
#include "phpqt5toolsjsextension.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <coreplugin/jsexpander.h>
#include <coreplugin/fileiconprovider.h>
#include <coreplugin/iwizardfactory.h>
#include <utils/mimetypes/mimedatabase.h>

#include "phpqt5buildconfigurationfactory.h"
#include "phpqt5makebuildstepfactory.h"
#include "phpqt5qmakebuildstepfactory.h"
#include "jomBuildStep/phpqt5jombuildstepfactory.h"
#include "uicBuildStep/phpqt5uicbuildstepfactory.h"
#include "phpqt5makecleanstepfactory.h"
#include "phpqt5projectmanager.h"
#include "phpqt5runconfigurationfactory.h"
#include "phpqt5runcontrolfactory.h"
#include "projecteditorfactory.h"

#include "phpeditor/phpeditorfactory.h"
#include "phpeditor/phpoutline.h"

#include <texteditor/outlinefactory.h>

#include "debugger/phpqt5debugger.h"
#include "wizards/phpqt5formwizard.h"

using namespace Utils;
using namespace PHPQt5::Internal;

using namespace Core;

namespace PHPQt5 {
namespace Internal {

PHPQt5Plugin::PHPQt5Plugin()
{
    // Create your members
}

PHPQt5Plugin::~PHPQt5Plugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool PHPQt5Plugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    /*
    QAction *action = new QAction(tr("PHPQt5 Action"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
    connect(action, &QAction::triggered, this, &PHPQt5Plugin::triggerAction);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("PHPQt5"));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    JsExpander::registerQObjectForJs(QLatin1String("PHPQt5"), new PHPQt5ToolsJsExtension);
    */

    MimeDatabase::addMimeTypes(QLatin1String(":/phpqt5.mimetypes.xml"));

    IWizardFactory::registerFactoryCreator([] {
        return QList<IWizardFactory *> {
            new PHPQt5FormWizard
        };
    });

//    addAutoReleasedObject(new PHPQt5Settings);
//    addAutoReleasedObject(new PHPQt5SnippetProvider);


    addAutoReleasedObject(new PHPQt5ProjectManager);
    addAutoReleasedObject(new PHPQt5BuildConfigurationFactory);
    addAutoReleasedObject(new PHPQt5RunConfigurationFactory);
    addAutoReleasedObject(new PHPQt5MakeBuildStepFactory);
    addAutoReleasedObject(new PHPQt5QMakeBuildStepFactory);
    addAutoReleasedObject(new PHPQt5JomBuildStepFactory);
    addAutoReleasedObject(new PHPQt5UicBuildStepFactory);
    addAutoReleasedObject(new PHPQt5MakeCleanStepFactory);
    addAutoReleasedObject(new PHPQt5RunControlFactory);
    addAutoReleasedObject(PHPQt5OutputWindow::instance());
    addAutoReleasedObject(new PHPEditorFactory);
    addAutoReleasedObject(new Internal::ProjectEditorFactory);
    addAutoReleasedObject(new PHPOutlineWidgetFactory);
//    addAutoReleasedObject(new PHPQt5CodeStyleSettingsPage);
//    addAutoReleasedObject(new PHPQt5CodeStylePreferencesFactory);

    // Add MIME overlay icons (these icons displayed at Project dock panel)
    const QIcon phpIcon((QLatin1String(Constants::C_PHP_ICON_PATH)));
    if (!phpIcon.isNull()) {
        Core::FileIconProvider::registerIconOverlayForMimeType(phpIcon, Constants::C_PHP_MIMETYPE);
    }

    const QIcon phpqt5Icon((QLatin1String(Constants::C_PHPQT5_ICON_PATH)));
    if (!phpqt5Icon.isNull()) {
        Core::FileIconProvider::registerIconOverlayForMimeType(phpqt5Icon, Constants::C_PHPQT5_PROJECT_MIMETYPE);
    }

    return true;
}

void PHPQt5Plugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag PHPQt5Plugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void PHPQt5Plugin::triggerAction()
{
    QMessageBox::information(Core::ICore::mainWindow(),
                             tr("Action Triggered"),
                             tr("This is an action from PHPQt5."));
}

} // namespace Internal
} // namespace PHPQt5
