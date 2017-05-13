DEFINES += PHPQT5_LIBRARY

# Qt Creator linking

## Either set the IDE_SOURCE_TREE when running qmake,
## or set the QTC_SOURCE environment variable, to override the default setting
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = $$(QTC_SOURCE)
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = "D:/src/qt-creator-opensource-src-4.2.1.tar/qt-creator-opensource-src-4.2.1"

## Either set the IDE_BUILD_TREE when running qmake,
## or set the QTC_BUILD environment variable, to override the default setting
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = "D:/PQBuilder-0.6.1/Tools/QtCreator"

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

DEPENDPATH += "D:/src/qt-creator-opensource-src-4.2.1.tar/build-qtcreator-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/lib/qtcreator/plugins"
DEPENDPATH += "D:/src/qt-creator-opensource-src-4.2.1.tar/build-qtcreator-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/lib/qtcreator"
DEPENDPATH += "D:/src/qt-creator-opensource-src-4.2.1.tar/build-qtcreator-Desktop_Qt_5_8_0_MSVC2015_32bit-Release/lib/"

# PHPQt5 files

SOURCES += phpqt5plugin.cpp \
    phpqt5project.cpp \
    phpqt5projectnode.cpp \
    phpqt5projectmanager.cpp \
    phpqt5makebuildstep.cpp \
    phpqt5makebuildstepconfigwidget.cpp \
    phpqt5makebuildstepfactory.cpp \
    phpqt5makecleanstep.cpp \
    phpqt5makecleanstepconfigwidget.cpp \
    phpqt5makecleanstepfactory.cpp \
    phpqt5buildconfiguration.cpp \
    phpqt5buildconfigurationfactory.cpp \
    phpqt5buildconfigurationwidget.cpp \
    phpeditor/phpautocompleter.cpp \
    phpeditor/phpcompletionassistprovider.cpp \
    phpeditor/phphighlighter.cpp \
    phpeditor/phpindenter.cpp \
    phpeditor/phptoken.cpp \
    phpeditor/phpeditorfactory.cpp \
    phpqt5qmakebuildstep.cpp \
    phpqt5qmakebuildstepfactory.cpp \
    phpqt5qmakebuildstepconfigwidget.cpp \
    phpqt5runconfiguration.cpp \
    phpqt5runconfigurationfactory.cpp \
    phpqt5runconfigurationwidget.cpp \
    phpqt5runcontrol.cpp \
    phpqt5runcontrolfactory.cpp \
    debugger/phpqt5debugger.cpp \
    phpeditor/phpmodel.cpp \
    phpeditor/phpcompletionassist.cpp \
    phpeditor/dynvarsprocessor.cpp \
    phpqt5toolsjsextension.cpp \
    wizards/phpqt5formwizard.cpp \
    wizards/phpqt5formwizarddialog.cpp \
    phpeditor/phpeditor.cpp \
    phpeditor/phpeditordocument.cpp \
    parser/astnode.cpp \
    parser/scanner.cpp \
    parser/token.cpp \
    parser/tokeninfo.cpp \
    phpeditor/phpoutline.cpp \
    phpeditor/phpoverviewmodel.cpp \
    phpeditor/phpeditoroutline.cpp \
    parser/ast.cpp \
    parser/parser.cpp \
    projecteditor.cpp \
    projecteditorwidget.cpp \
    projecteditorfactory.cpp \
    projectdocument.cpp \
    jomBuildStep/phpqt5jombuildstep.cpp \
    jomBuildStep/phpqt5jombuildstepconfigwidget.cpp \
    jomBuildStep/phpqt5jombuildstepfactory.cpp \
    uicBuildStep/phpqt5uicbuildstep.cpp \
    uicBuildStep/phpqt5uicbuildstepconfigwidget.cpp \
    uicBuildStep/phpqt5uicbuildstepfactory.cpp

HEADERS += phpqt5plugin.h \
        phpqt5_global.h \
        phpqt5constants.h \
    phpqt5project.h \
    phpqt5projectnode.h \
    phpqt5projectmanager.h \
    phpqt5makebuildstep.h \
    phpqt5makebuildstepconfigwidget.h \
    phpqt5makebuildstepfactory.h \
    phpqt5makecleanstep.h \
    phpqt5makecleanstepconfigwidget.h \
    phpqt5makecleanstepfactory.h \
    phpqt5buildconfiguration.h \
    phpqt5buildconfigurationfactory.h \
    phpqt5buildconfigurationwidget.h \
    phpeditor/fontsettings.h \
    phpeditor/phpautocompleter.h \
    phpeditor/phpcompletionassistprovider.h \
    phpeditor/phpeditorconstants.h \
    phpeditor/phpformattoken.h \
    phpeditor/phphighlighter.h \
    phpeditor/phpindenter.h \
    phpeditor/phptoken.h \
    phpeditor/phptokeninfo.h \
    phpeditor/phpeditorfactory.h \
    phpqt5qmakebuildstep.h \
    phpqt5qmakebuildstepfactory.h \
    phpqt5qmakebuildstepconfigwidget.h \
    phpqt5runconfiguration.h \
    phpqt5runconfigurationfactory.h \
    phpqt5runconfigurationwidget.h \
    phpqt5runcontrol.h \
    phpqt5runcontrolfactory.h \
    debugger/phpqt5debugger.h \
    phpeditor/phpcompletionassist.h \
    phpeditor/dynvarsprocessor.h \
    phpqt5toolsjsextension.h \
    wizards/phpqt5formwizard.h \
    wizards/phpqt5formwizarddialog.h \
    phpeditor/phpeditor.h \
    phpeditor/phpeditordocument.h \
    parser/astnode.h \
    parser/scanner.h \
    parser/sourcecodestream.h \
    parser/token.h \
    parser/tokeninfo.h \
    phpeditor/phpoutline.h \
    phpeditor/phpoverviewmodel.h \
    phpeditor/phpeditoroutline.h \
    parser/ast.h \
    parser/parser.h \
    projecteditor.h \
    projecteditorwidget.h \
    projecteditorfactory.h \
    projectdocument.h \
    jomBuildStep/phpqt5jombuildstep.h \
    jomBuildStep/phpqt5jombuildstepconfigwidget.h \
    jomBuildStep/phpqt5jombuildstepfactory.h \
    uicBuildStep/phpqt5uicbuildstep.h \
    uicBuildStep/phpqt5uicbuildstepconfigwidget.h \
    uicBuildStep/phpqt5uicbuildstepfactory.h \
    phpqt5classhelpers.h

QTC_PLUGIN_NAME = PHPQt5
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin\
    projectexplorer\
    qmljseditor\
    qmljstools

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)

RESOURCES += \
    phpqt5.qrc

DISTFILES +=

FORMS += \
    phpqt5makebuildstepconfigwidget.ui \
    phpqt5makecleanstepconfigwidget.ui \
    phpqt5qmakebuildstepconfigwidget.ui \
    wizards/phpqt5formwizardpage.ui \
    projecteditorwidget.ui \
    jomBuildStep/phpqt5jombuildstepconfigwidget.ui \
    uicBuildStep/phpqt5uicbuildstepconfigwidget.ui
