#include "projecteditor.h"
#include "projecteditorwidget.h"
#include "ui_projecteditorwidget.h"

#include "phpqt5constants.h"
#include "projectdocument.h"

#include <QJsonDocument>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QImageReader>
#include <QPainter>

#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditor.h>
#include <texteditor/textdocument.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectwindow.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/kitinformation.h>

using namespace ProjectExplorer;
using namespace PHPQt5;
using namespace PHPQt5::Internal;

namespace PHPQt5 {
namespace Internal {

Project *phpqt5Project(const Utils::FileName &fileName)
{
    foreach (Project *project, SessionManager::projects()) {
        if (!project->activeTarget())
            continue;
        Kit *kit = project->activeTarget()->kit();
        if (/*DeviceTypeKitInformation::deviceTypeId(kit) == PHPQt5::Constants::ANDROID_DEVICE_TYPE
                        &&*/ fileName.isChildOf(project->projectDirectory()))
            return project;
    }

    return 0;
}

ProjectTextEditorWidget::ProjectTextEditorWidget(ProjectEditorWidget *parent)
{
    setTextDocument(TextEditor::TextDocumentPtr(new ProjectDocument(parent)));
    textDocument()->setMimeType(QLatin1String(Constants::C_PHPQT5_PROJECT_MIMETYPE));
    //setupGenericHighlighter();
    setMarksVisible(false);
}

ProjectEditorWidget::ProjectEditorWidget(QWidget *parent) :
    QWidget(parent),
    m_dirty(false),
    ui(new Ui::ProjectEditorWidget)
{
    ui->setupUi(this);

    loadBuildTemplates();

    m_textEditorWidget = new ProjectTextEditorWidget(this);
    m_editor = new ProjectEditor(this);

    ui->frame_editor->setLayout(new QVBoxLayout);
    ui->frame_editor->layout()->setMargin(0);
    ui->frame_editor->layout()->addWidget(m_textEditorWidget);

    m_timerParseCheck.setInterval(800);
    m_timerParseCheck.setSingleShot(true);
    m_updateSource.setInterval(400);
    m_updateSource.setSingleShot(true);

    connect(m_textEditorWidget->document(), &QTextDocument::contentsChanged,
            this, &ProjectEditorWidget::startParseCheck);
    connect(m_textEditorWidget->textDocument(), &TextEditor::TextDocument::reloadFinished,
            this, [this](bool success) { if (success) updateAfterFileLoad(); });
    connect(m_textEditorWidget->textDocument(), &TextEditor::TextDocument::openFinishedSuccessfully,
            this, &ProjectEditorWidget::updateAfterFileLoad);

    connect(&m_updateSource, &QTimer::timeout,
            this, &ProjectEditorWidget::syncToEditor);
    // connect(&m_timerParseCheck, &QTimer::timeout,
    //         this, &ProjectEditorWidget::delayedParseCheck);

    m_ready = true;
}

ProjectEditorWidget::~ProjectEditorWidget()
{
    delete ui;
}

void ProjectEditorWidget::loadBuildTemplates()
{
    m_teplates.clear();
    ui->build_template->clear();

    BuildTplInfo tplInfo;
    tplInfo.id = "";
    tplInfo.desc = "no template";
    tplInfo.name = "";
    tplInfo.path = "";

    m_teplates.append(tplInfo);
    ui->build_template->addItem("");

    QDir templatesDir = QDir(qApp->applicationDirPath());

    if (templatesDir.cd("../../PHPQt5/templates/build")) {
        QDirIterator it(templatesDir.absolutePath(),
                        QDir::Dirs|QDir::NoDotAndDotDot);

        while (it.hasNext()) {
            QString tplPath = it.next();

            QFile tplFile(QString("%1/tpl.json").arg(tplPath));
            if (tplFile.exists()) {
                qDebug() << Q_FUNC_INFO << tplFile.fileName();
                if (tplFile.open(QFile::ReadOnly)) {
                    QJsonParseError parseError;
                    QJsonDocument doc = QJsonDocument::fromJson(tplFile.readAll(),
                                                                &parseError);

                    if (parseError.error != QJsonParseError::NoError) {
                        QMessageBox::warning(Q_NULLPTR, "Parse Error", parseError.errorString());
                        continue;
                    }

                    BuildTplInfo tplInfo;
                    QJsonObject tpl = doc.object();

                    tplInfo.id = QFileInfo(tplPath).baseName();
                    tplInfo.desc = tpl.value("desc").toString().trimmed();
                    tplInfo.name = tpl.value("name").toString().trimmed();
                    tplInfo.path = tplPath;

                    if (!tplInfo.name.isEmpty()) {
                        m_teplates.append(tplInfo);

                        QString icon = tpl.value("icon").toString().trimmed();
                        if (icon.isEmpty()) {
                            ui->build_template->addItem(tplInfo.name);
                        }
                        else {
                            ui->build_template->addItem(QIcon(QString("%1/%2").arg(tplPath).arg(icon)),
                                                        tplInfo.name);
                        }
                    }
                }
            }
        }
    }
}

ProjectEditorWidget::EditorPage ProjectEditorWidget::activePage() const
{
    return General;
}

bool ProjectEditorWidget::setActivePage(ProjectEditorWidget::EditorPage page)
{
    qDebug() << Q_FUNC_INFO;
    EditorPage prevPage = activePage();

    if (prevPage == page)
        return true;

    if (page == Source) {
        syncToEditor();
    } else {
        if (!syncToWidgets())
            return false;
    }

    return true;
}

Core::IEditor *ProjectEditorWidget::editor() const
{
    return m_editor;
}

TextEditor::TextEditorWidget *ProjectEditorWidget::textEditorWidget() const
{
    return m_textEditorWidget;
}

bool ProjectEditorWidget::isModified() const
{
    return m_dirty;
}

void ProjectEditorWidget::setDirty(bool dirty)
{
    if (dirty == m_dirty)
        return;

    m_dirty = dirty;
    emit guiChanged();
}

void ProjectEditorWidget::preSave()
{

}

void ProjectEditorWidget::postSave()
{
    const Utils::FileName docPath = m_textEditorWidget->textDocument()->filePath();
    ProjectExplorer::Project *project = phpqt5Project(docPath);

    if (project) {
        if (Target *target = project->activeTarget()) {
            //AndroidQtSupport *androidQtSupport = AndroidManager::androidQtSupport(target);
            //if (androidQtSupport)
            //    androidQtSupport->manifestSaved(target);
        }
    }

    syncToWidgets();
}

void ProjectEditorWidget::startParseCheck()
{
    //m_timerParseCheck.start();
}

void ProjectEditorWidget::updateAfterFileLoad()
{
    qDebug() << Q_FUNC_INFO;
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(m_textEditorWidget->toPlainText().toUtf8(),
                                                &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(Q_NULLPTR, "Parse Error", parseError.errorString());
        m_textEditorWidget->setFocus();
        return;
    }

    syncToWidgets(doc);
}

void ProjectEditorWidget::delayedParseCheck()
{
    //syncToWidgets();
}

void ProjectEditorWidget::syncToEditor()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_ready)
        return;

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(m_textEditorWidget->toPlainText().toUtf8(),
                                                &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(Q_NULLPTR, "Parse Error", parseError.errorString());
        return;
    }

    QJsonObject root = doc.object();

    QJsonObject build;
    if (root.contains("build") && root.value("build").isObject()) {
        build = root.take("build").toObject();
    }

    build.insert("save_php7ts_md5", ui->build_md5lib->isChecked());
    build.insert("save_ini_md5", ui->build_md5ini->isChecked());
    build.insert("template", m_teplates.at(ui->build_template->currentIndex()).id);

    ui->label_icon_warn->clear();
    const Utils::FileName docPath = m_textEditorWidget->textDocument()->filePath();
    if (m_appIconPath.mid(0, docPath.parentDir().toString().length()) == docPath.parentDir().toString()) {
        build.insert("icon", m_appIconPath.mid(docPath.parentDir().toString().length() + 1));
    }
    else {
        build.insert("icon", m_appIconPath);
        if (!m_appIconPath.isEmpty()) {
            QFile iconFile(m_appIconPath);

            if (iconFile.exists()) {
                ui->label_icon_warn->setText("Warning! Application icon "
                                             "location outside the project directory.");
            }
            else {
                ui->label_icon_warn->setText("Warning! Application icon not found.");
            }
        }
    }

    root.insert("build", build);

    QJsonObject app;
    if (root.contains("app") && root.value("app").isObject()) {
        app = root.take("app").toObject();
    }

    app.insert("name", ui->app_name->text());
    app.insert("version", ui->app_version->text());
    app.insert("orgName", ui->app_org->text());
    app.insert("orgDomain", ui->app_domain->text());
    root.insert("app", app);

    QJsonObject debug;
    if (root.contains("debug") && root.value("debug").isObject()) {
        debug = root.take("debug").toObject();
        debug.insert("socketName", ui->debug_socketName->text());
        root.insert("debug", debug);
    }
    else if (!ui->debug_socketName->text().trimmed().isEmpty()) {
        debug.insert("socketName", ui->debug_socketName->text());
        root.insert("debug", debug);
    }

    QJsonDocument newDoc(root);

    QString result = QString::fromUtf8(newDoc.toJson());

    if (result == m_textEditorWidget->toPlainText())
        return;

    m_textEditorWidget->setPlainText(result);
    m_textEditorWidget->document()->setModified(true);

    m_dirty = false;
}

bool ProjectEditorWidget::syncToWidgets()
{
    qDebug() << Q_FUNC_INFO;
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(m_textEditorWidget->toPlainText().toUtf8(),
                                                &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(Q_NULLPTR, "Parse Error", parseError.errorString());
        return false;
    }

    syncToWidgets(doc);

    return true;
}

void ProjectEditorWidget::syncToWidgets(QJsonDocument doc)
{
    qDebug() << Q_FUNC_INFO;
    QJsonObject root = doc.object();

    if (root.contains("build") && root.value("build").isObject()) {
        const QJsonObject &build = root.value("build").toObject();

        ui->build_md5lib->setChecked(build.value("save_php7ts_md5").toBool(false));
        ui->build_md5ini->setChecked(build.value("save_ini_md5").toBool(false));

        QString tplId = build.value("template").toString().trimmed();

        ui->build_template->setCurrentIndex(0);
        for (int idx = 0; idx < m_teplates.size(); idx++) {
            const BuildTplInfo &tplInfo = m_teplates.at(idx);
            if (tplInfo.id == tplId) {
                ui->build_template->setCurrentIndex(idx);
                break;
            }
        }

        ui->label_icon_warn->clear();
        m_appIconPath = build.value("icon").toString().trimmed();
        const Utils::FileName docPath = m_textEditorWidget->textDocument()->filePath();
        QFileInfo iconFI(m_appIconPath);

        if (!iconFI.isAbsolute()) {
            QFile iconFile(QString("%1/%2").arg(docPath.parentDir().toString())
                           .arg(m_appIconPath));
            if (!iconFile.exists()) {
                ui->label_icon_warn->setText("Warning! Application icon not found.");
            }
            else m_appIconPath = iconFile.fileName();
        }
        else {
            QFile iconFile(m_appIconPath);
            if (iconFile.exists()) {
                ui->label_icon_warn->setText("Warning! Application icon "
                                             "location outside the project directory.");
            }
            else {
                ui->label_icon_warn->setText("Warning! Application icon not found.");
            }
        }

        loadIcon();
    }
    else {
        // error
        return;
    }

    if (root.contains("app") && root.value("app").isObject()) {
        const QJsonObject &app = root.value("app").toObject();

        ui->app_name->setText(app.value("name").toString());
        ui->app_version->setText(app.value("version").toString());
        ui->app_org->setText(app.value("orgName").toString());
        ui->app_domain->setText(app.value("orgDomain").toString());
        //ui->build_icon->setText(app.value("icon").toString());
    }
    else {
        // error
        return;
    }

    if (root.contains("debug") && root.value("debug").isObject()) {
        const QJsonObject &debug = root.value("debug").toObject();

        ui->debug_socketName->setText(debug.value("socketName").toString());
    }
}

void ProjectEditorWidget::loadIcon()
{
    if (!m_appIconPath.isEmpty()) {
        ui->pushButton_app_icon->setIcon(QIcon(m_appIconPath));
    }
}

void ProjectEditorWidget::on_build_md5lib_clicked(bool checked)
{
    Q_UNUSED(checked)

    setDirty();
    if (!m_updateSource.isActive())
        m_updateSource.start();
}

void ProjectEditorWidget::on_build_md5ini_clicked(bool checked)
{
    Q_UNUSED(checked)

    setDirty();
    if (!m_updateSource.isActive())
        m_updateSource.start();
}

void ProjectEditorWidget::on_build_template_currentIndexChanged(int index)
{
    ui->build_template_desc->setText(m_teplates.at(index).desc);
    if (!m_updateSource.isActive())
        m_updateSource.start();
}

void ProjectEditorWidget::on_app_name_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)

    if (!m_updateSource.isActive())
        m_updateSource.start();
}

void ProjectEditorWidget::on_app_version_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)

    if (!m_updateSource.isActive())
        m_updateSource.start();
}

void ProjectEditorWidget::on_app_org_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)

    if (!m_updateSource.isActive())
        m_updateSource.start();
}

void ProjectEditorWidget::on_app_domain_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)

    if (!m_updateSource.isActive())
        m_updateSource.start();
}

void ProjectEditorWidget::on_debug_socketName_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1)

    if (!m_updateSource.isActive())
        m_updateSource.start();
}

void ProjectEditorWidget::on_build_template_activated(int index)
{
    Q_UNUSED(index)

    setDirty();
}

void ProjectEditorWidget::on_pushButton_app_icon_clicked()
{
    const Utils::FileName docPath = m_textEditorWidget->textDocument()->filePath();

    QString iconPath = QFileDialog::getOpenFileName(Q_NULLPTR, "Select application icon",
                                                    docPath.parentDir().toString(), "*.ico");

    if (!iconPath.isEmpty()) {
        m_appIconPath = iconPath;
        loadIcon();
        setDirty();
        if (!m_updateSource.isActive())
            m_updateSource.start();
    }
}

} // namespace Internal
} // namespace PHPQt5


