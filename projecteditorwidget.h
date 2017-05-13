#ifndef PHPQT5_INTERNAL_PROJECTEDITORWIDGET_H
#define PHPQT5_INTERNAL_PROJECTEDITORWIDGET_H

#include <texteditor/texteditor.h>
#include <QWidget>

namespace PHPQt5 {
namespace Internal {

class ProjectEditor;
class ProjectEditorWidget;

namespace Ui {
class ProjectEditorWidget;
}

class ProjectTextEditorWidget : public TextEditor::TextEditorWidget
{
public:
    explicit ProjectTextEditorWidget(ProjectEditorWidget *parent);
};

struct BuildTplInfo {
    QString id;
    QString name;
    QString desc;
    QString path;
};

class ProjectEditorWidget : public QWidget
{
    Q_OBJECT

public:
    enum EditorPage {
        General = 0,
        Source = 1
    };

    explicit ProjectEditorWidget(QWidget *parent = 0);
    ~ProjectEditorWidget();

    void loadBuildTemplates();

    EditorPage activePage() const;
    bool setActivePage(EditorPage page);

    Core::IEditor *editor() const;
    TextEditor::TextEditorWidget *textEditorWidget() const;

    bool isModified() const;
    void setDirty(bool dirty = true);

    void preSave();
    void postSave();

public slots:
    void startParseCheck();
    void updateAfterFileLoad();
    void delayedParseCheck();

signals:
    void guiChanged();

private slots:
    void on_build_md5lib_clicked(bool checked);
    void on_build_md5ini_clicked(bool checked);
    void on_build_template_currentIndexChanged(int index);
    void on_build_template_activated(int index);
    void on_pushButton_app_icon_clicked();

    void on_app_name_textEdited(const QString &arg1);
    void on_app_version_textEdited(const QString &arg1);
    void on_app_org_textEdited(const QString &arg1);
    void on_app_domain_textEdited(const QString &arg1);
    void on_debug_socketName_textEdited(const QString &arg1);

private:
    void syncToEditor();
    bool syncToWidgets();
    void syncToWidgets(QJsonDocument doc);
    void loadIcon();

    bool m_ready = false;
    QTimer m_timerParseCheck;
    QTimer m_updateSource;
    bool m_dirty; // indicates that we need to call syncToEditor()

    Ui::ProjectEditorWidget *ui;

    TextEditor::TextEditorWidget *m_textEditorWidget;
    ProjectEditor *m_editor;

    QList<BuildTplInfo> m_teplates;
    QString m_appIconPath;
};


} // namespace Internal
} // namespace PHPQt5
#endif // PHPQT5_INTERNAL_PROJECTEDITORWIDGET_H
