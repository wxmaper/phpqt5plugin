#include "phpqt5project.h"
#include "phpqt5projectnode.h"
#include "phpqt5projectmanager.h"

#include "phpqt5constants.h"

#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <texteditor/textdocument.h>

#include <utils/algorithm.h>

#include <QFileInfo>
#include <QQueue>

using namespace ProjectExplorer;
using namespace Utils;

namespace PHPQt5 {

const int MIN_TIME_BETWEEN_PROJECT_SCANS = 4500;

PHPQt5Project::PHPQt5Project(PHPQt5ProjectManager *projectManager, const QString &fileName)
{
    setId(Constants::C_PHPQT5PROJECT_ID);
    setProjectManager(projectManager);
    setDocument(new TextEditor::TextDocument);
    document()->setFilePath(FileName::fromString(fileName));
    QFileInfo fi = QFileInfo(fileName);
    QDir dir = fi.dir();
    setRootProjectNode(new PHPQt5ProjectNode(FileName::fromString(dir.absolutePath())));
    rootProjectNode()->setDisplayName(dir.dirName());

    m_projectScanTimer.setSingleShot(true);
    connect(&m_projectScanTimer, &QTimer::timeout, this, &PHPQt5Project::populateProject);

    populateProject();

    connect(&m_fsWatcher, &QFileSystemWatcher::directoryChanged,
            this, &PHPQt5Project::scheduleProjectScan);

    m_fsWatcher.addPath(QDir(projectDirectory().toString()).absolutePath());
}

QString PHPQt5Project::displayName() const
{
    return rootProjectNode()->displayName();
}

QStringList PHPQt5Project::files(FilesMode) const
{
    return QStringList(m_files.toList());
}

bool PHPQt5Project::needsConfiguration() const
{
    return targets().empty();
}

void PHPQt5Project::scheduleProjectScan()
{
    auto elapsedTime = m_lastProjectScan.elapsed();
    if (elapsedTime < MIN_TIME_BETWEEN_PROJECT_SCANS) {
        if (!m_projectScanTimer.isActive()) {
            m_projectScanTimer.setInterval(MIN_TIME_BETWEEN_PROJECT_SCANS - elapsedTime);
            m_projectScanTimer.start();
        }
    } else {
        populateProject();
    }
}

void PHPQt5Project::populateProject()
{
    m_lastProjectScan.start();

    QSet<QString> oldFiles = m_files;
    m_files.clear();

    recursiveScanDirectory(QDir(projectDirectory().toString() + "/scripts"), m_files);
    recursiveScanDirectory(QDir(projectDirectory().toString() + "/forms"), m_files);
    recursiveScanDirectory(QDir(projectDirectory().toString() + "/res"), m_files);
    //  recursiveScanDirectory(QDir(projectDirectory().toString() + "/cpp"), m_files);

    //  if (QFile(projectFilePath().toString()).exists())
    //      m_files << projectFilePath().toString();
    for (const QFileInfo &info : QDir(projectDirectory().toString()).entryInfoList(
             QDir::Files |
             QDir::NoDotAndDotDot |
             QDir::NoSymLinks |
             QDir::CaseSensitive)) {
        if (info.suffix() == "phpqt5") {
            m_files << info.filePath();
        }
    }

    if (m_files == oldFiles)
        return;

    QList<FileNode *> fileNodes = Utils::transform(m_files.toList(), [](const QString &f) {
        const QFileInfo info(f);
        const QString suffix = info.suffix();

        if (suffix == "php")
            return new FileNode(FileName::fromString(f), SourceType, false);
        else if(suffix == "ui")
            return new FileNode(FileName::fromString(f), FormType, false);
        else if(suffix == "qml")
            return new FileNode(FileName::fromString(f), QMLType, false);
        else
            return new FileNode(FileName::fromString(f), ProjectFileType, false);
    });
    rootProjectNode()->buildTree(fileNodes);

    emit fileListChanged();
    emit parsingFinished();
}

void PHPQt5Project::recursiveScanDirectory(const QDir &dir, QSet<QString> &container)
{
    static const QRegExp userFilePattern(QLatin1String(".*\\.user$"));
    static const QRegExp autosaveFilePattern(QLatin1String(".*\\.autosave$"));

    for (const QFileInfo &info : dir.entryInfoList(QDir::AllDirs |
                                                   QDir::Files |
                                                   QDir::NoDotAndDotDot |
                                                   QDir::NoSymLinks |
                                                   QDir::CaseSensitive)) {
        if (info.isDir())
            recursiveScanDirectory(QDir(info.filePath()), container);
        else if (userFilePattern.indexIn(info.fileName()) == -1
                 && autosaveFilePattern.indexIn(info.fileName()) == -1)
            container << info.filePath();
    }

    m_fsWatcher.addPath(dir.absolutePath());
}

bool PHPQt5Project::supportsKit(Kit *k, QString *) const
{
    return k->isValid();
}

FileNameList PHPQt5Project::projectFiles() const
{
    FileNameList result;

    QQueue<FolderNode *> folders;
    folders.enqueue(rootProjectNode());

    while (!folders.isEmpty()) {
        FolderNode *folder = folders.takeFirst();
        for (FileNode *file : folder->fileNodes()) {
            if (file->displayName().endsWith(QLatin1String(".phpqt5")))
                result.append(file->filePath());
            //if (file->displayName().endsWith(QLatin1String(".php")))
            //    result.append(file->filePath());
        }
        folders.append(folder->subFolderNodes());
    }

    return result;
}

}
