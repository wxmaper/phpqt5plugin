#ifndef PHPQT5PROJECT_H
#define PHPQT5PROJECT_H

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include <QFileSystemWatcher>
#include <QElapsedTimer>
#include <QTimer>

namespace PHPQt5 {

class PHPQt5ProjectManager;
class PHPQt5ProjectNode;

class PHPQt5Project : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    PHPQt5Project(PHPQt5ProjectManager *projectManager, const QString &fileName);

    QString displayName() const override;
    QStringList files(FilesMode) const override;
    bool needsConfiguration() const override;
    bool supportsKit(ProjectExplorer::Kit *k, QString *errorMessage) const override;
    Utils::FileNameList projectFiles() const;

private:
    void scheduleProjectScan();
    void populateProject();
    void recursiveScanDirectory(const QDir &dir, QSet<QString> &container);

    QSet<QString> m_files;
    QFileSystemWatcher m_fsWatcher;

    QElapsedTimer m_lastProjectScan;
    QTimer m_projectScanTimer;
};

} // PHPQt5


#endif // PHPQT5PROJECT_H
