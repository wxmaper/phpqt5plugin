#pragma once

#include <QObject>
#include <QtGlobal>

namespace PHPQt5 {
namespace Constants {

const char ACTION_ID[] = "PHPQt5.Action";
const char MENU_ID[] = "PHPQt5.Menu";

const char C_PHPQT5PROJECT_ID[] = "PHPQt5.PHPQt5Project";
const char C_PHPEDITOR_ID[] = "PHPQt5.PHPEditor";
const char C_EDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("OpenWith::Editors", "PHP Editor");
const char C_PHP_ICON_PATH[] = ":/images/php.png";
const char C_PHPQT5_ICON_PATH[] = ":/images/paw.png";

const char C_PHPLANGUAGE_ID[] = "PHP";
const char C_PHPCODESTYLESETTINGSPAGE_ID[] = "PHPQt5.PHPCodeStyleSettings";
const char C_PHPCODESTYLESETTINGSPAGE_DISPLAY[] = QT_TRANSLATE_NOOP("PHPCodeStyleSettingsPage", "Code Style");
const char C_PHPCODESTYLESETTINGSPAGE_CATEGORY[] = "Z.PHP";
const char C_PHPCODESTYLESETTINGSPAGE_CATEGORY_DISPLAY[] = QT_TRANSLATE_NOOP("PHPCodeStyleSettingsPage", "PHP");
const char C_PHPLANGUAGE_NAME[] = QT_TRANSLATE_NOOP("PHPCodeStylePreferencesFactory", "PHP");
const char C_PHPGLOBALCODESTYLE_ID[] = "PHPGlobal";
const QString C_PHPSNIPPETSGROUP_ID = QStringLiteral("PHP.PHPSnippetsGroup");
const char C_PHPSNIPPETPROVIDER_DISPLAY[] = QT_TRANSLATE_NOOP("PHPSnippetProvider", "PHP");


// PHPQt5BuildConfigurationWidget
const char C_PHPQt5BUILDCONFIGURATIONWIDGET_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5BuildConfigurationWidget","General");

// PHPQt5BuildConfiguration
const char C_PHPQt5BUILDCONFIGURATION_ID[] = "PHPQt5.PHPQt5BuildConfiguration";
const QString C_PHPQt5BUILDCONFIGURATION_DISPLAY_KEY = QStringLiteral("PHPQt5.PHPQt5BuildConfiguration.Display");
const QString C_PHPQt5BUILDCONFIGURATION_BUILDDIRECTORY_KEY = QStringLiteral("PHPQt5.PHPQt5BuildConfiguration.BuildDirectory");

// PHPQt5MakeBuildStep
const char C_PHPQt5MAKEBUILDSTEP_ID[] = "PHPQt5.PHPQt5MakeBuildStep";
const char C_PHPQt5MAKEBUILDSTEP_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5MakeBuildStep", "<b>PHPQt5Make</b>");
const QString C_PHPQt5MAKEBUILDSTEP_USERCOMPILEROPTIONS = QStringLiteral("PHPQt5.PHPQt5MakeBuildStep.UserCompilerOptions");
const QString C_PHPQt5MAKEBUILDSTEP_DEFAULTBUILDOPTIONS = QStringLiteral("PHPQt5.PHPQt5MakeBuildStep.DefaultBuildOptions");

// PHPQt5QMakeBuildStep
const char C_PHPQt5QMAKEBUILDSTEP_ID[] = "PHPQt5.PHPQt5QMakeBuildStep";
const char C_PHPQt5QMAKEBUILDSTEP_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5QMakeBuildStep", "<b>qmake</b>");
const QString C_PHPQt5QMAKEBUILDSTEP_USERCOMPILEROPTIONS = QStringLiteral("PHPQt5.PHPQt5QMakeBuildStep.UserCompilerOptions");
const QString C_PHPQt5QMAKEBUILDSTEP_DEFAULTBUILDOPTIONS = QStringLiteral("PHPQt5.PHPQt5QMakeBuildStep.DefaultBuildOptions");

// PHPQt5JomBuildStep
const char C_PHPQt5JOMBUILDSTEP_ID[] = "PHPQt5.PHPQt5JomBuildStep";
const char C_PHPQt5JOMBUILDSTEP_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5JomBuildStep", "<b>jom</b>");
const QString C_PHPQt5JOMBUILDSTEP_USERCOMPILEROPTIONS = QStringLiteral("PHPQt5.PHPQt5JomBuildStep.UserCompilerOptions");
const QString C_PHPQt5JOMBUILDSTEP_DEFAULTBUILDOPTIONS = QStringLiteral("PHPQt5.PHPQt5JomBuildStep.DefaultBuildOptions");

// PHPQt5UicBuildStep
const char C_PHPQt5UICBUILDSTEP_ID[] = "PHPQt5.PHPQt5UicBuildStep";
const char C_PHPQt5UICBUILDSTEP_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5UicBuildStep", "<b>PHPQt5Uic</b>");
const QString C_PHPQt5UICBUILDSTEP_USERCOMPILEROPTIONS = QStringLiteral("PHPQt5.PHPQt5UicBuildStep.UserCompilerOptions");
const QString C_PHPQt5UICBUILDSTEP_DEFAULTBUILDOPTIONS = QStringLiteral("PHPQt5.PHPQt5UicBuildStep.DefaultBuildOptions");

// PHPQt5QMakeBuildStepWidget
const char C_PHPQt5QMAKEBUILDSTEPWIDGET_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5QMakeBuildStepConfigWidget", "<b>qmake</b>");
const char C_PHPQt5QMAKEBUILDSTEPWIDGET_SUMMARY[] = QT_TRANSLATE_NOOP("PHPQt5QMakeBuildStepConfigWidget", "<b>qmake</b>");

// PHPQt5JomBuildStepWidget
const char C_PHPQt5JOMBUILDSTEPWIDGET_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5JomBuildStepConfigWidget", "<b>jom</b>");
const char C_PHPQt5JOMBUILDSTEPWIDGET_SUMMARY[] = QT_TRANSLATE_NOOP("PHPQt5JomBuildStepConfigWidget", "<b>jom</b>");

// PHPQt5UicBuildStepWidget
const char C_PHPQt5UICBUILDSTEPWIDGET_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5UicBuildStepConfigWidget", "<b>PHPQt5Uic</b>");
const char C_PHPQt5UICBUILDSTEPWIDGET_SUMMARY[] = QT_TRANSLATE_NOOP("PHPQt5UicBuildStepConfigWidget", "<b>PHPQt5Uic</b>");

// PHPQt5MakeBuildStepWidget
const char C_PHPQt5MAKEBUILDSTEPWIDGET_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5MakeBuildStepConfigWidget", "<b>PHPQt5Make</b>");
const char C_PHPQt5MAKEBUILDSTEPWIDGET_SUMMARY[] = QT_TRANSLATE_NOOP("PHPQt5MakeBuildStepConfigWidget", "<b>PHPQt5Make</b>");

// PHPQt5MakeCleanStep
const char C_PHPQt5MAKECLEANSTEP_ID[] = "PHPQt5.PHPQt5MakeCleanStep";
const char C_PHPQt5MAKECLEANSTEP_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5MakeCleanStepFactory", "PHPQt5Make Clean Step");

// PHPQt5MakeCleanStepWidget
const char C_PHPQt5MAKECLEANSTEPWIDGET_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5MakeCleanStepWidget", "PHPQt5Make clean step");
const char C_PHPQt5MAKECLEANSTEPWIDGET_SUMMARY[] = QT_TRANSLATE_NOOP("PHPQt5MakeCleanStepWidget", "PHPQt5Make clean step");

// PHPQt5RunConfiguration
const char C_PHPQt5RUNCONFIGURATION_ID[] = "PHPQt5.PHPQt5RunConfiguration";
const char C_PHPQt5RUNCONFIGURATION_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5RunConfiguration", "Current Build Target");
const char C_PHPQt5RUNCONFIGURATION_DEFAULT_DISPLAY[] = QT_TRANSLATE_NOOP("PHPQt5RunConfiguration", "Current Build Target");
const QString C_PHPQt5RUNCONFIGURATION_EXECUTABLE_KEY = QStringLiteral("PHPQt5.PHPQt5RunConfiguration.Executable");
const QString C_PHPQt5RUNCONFIGURATION_WORKINGDIRECTORY_KEY = QStringLiteral("PHPQt5.PHPQt5RunConfiguration.WorkingDirectory");
const QString C_PHPQt5RUNCONFIGURATION_COMMANDLINEARGS_KEY = QStringLiteral("PHPQt5.PHPQt5RunConfiguration.CommandlineArgs");
const QString C_PHPQt5RUNCONFIGURATION_RUNMODE_KEY = QStringLiteral("PHPQt5.PHPQt5RunConfiguration.RunMode");
const QString C_PHPQt5RUNCONFIGURATION_WORKINGDIRECTORYASPECT_ID = QStringLiteral("PHPQt5.PHPQt5RunConfiguration.WorkingDirectoryAspect");
const QString C_PHPQt5RUNCONFIGURATION_ARGUMENTASPECT_ID = QStringLiteral("PHPQt5.PHPQt5RunConfiguration.ArgumentAspect");
const QString C_PHPQt5RUNCONFIGURATION_TERMINALASPECT_ID = QStringLiteral("PHPQt5.PHPQt5RunConfiguration.TerminalAspect");


const char PROJECT_EDITOR_ID[] = "PHPQt5.ProjectEditor.Id";
const char PROJECT_EDITOR_CONTEXT[] = "PHPQt5.ProjectEditor.Id";

/*******************************************************************************
 * MIME type
 ******************************************************************************/
const char C_PHP_MIMETYPE[] = "application/x-php";
const char C_PHP_MIME_ICON[] = "application-x-php";
const char C_PHPQT5_PROJECT_MIMETYPE[] = "application/x-phpqt5-project";

} // namespace PHPQt5
} // namespace Constants
