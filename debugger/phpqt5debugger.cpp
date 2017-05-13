#include "phpqt5debugger.h"

#include <coreplugin/editormanager/editormanager.h>

#include <aggregation/aggregate.h>
#include <coreplugin/find/basetextfind.h>
#include <coreplugin/outputwindow.h>
#include <utils/fileutils.h>
#include <utils/outputformatter.h>
#include <utils/theme/theme.h>

#include <QPlainTextEdit>
#include <QTextCharFormat>
#include <QContextMenuEvent>
#include <QTextBlock>
#include <QMenu>
#include <QAction>
#include <QTextBlockUserData>

#include <QPointer>
#include <QTextCodec>
#include <QDir>
#include <QTextStream>
#include <QTime>
#include <QPoint>
#include <QFileInfo>
#include <QLocalSocket>

#include <QLocalServer>


namespace PHPQt5 {
namespace Internal {

const char C_PHPQT5_OUTPUT_PANE[] = "PHPQt5.OutputPane";

// Store repository along with text blocks
class RepositoryUserData : public QTextBlockUserData
{
public:
    explicit RepositoryUserData(const QString &repo) : m_repository(repo) {}
    const QString &repository() const { return m_repository; }

private:
    const QString m_repository;
};

// A plain text edit with a special context menu containing "Clear" and
// and functions to append specially formatted entries.
class OutputWindowPlainTextEdit : public Core::OutputWindow
{
public:
    explicit OutputWindowPlainTextEdit(QWidget *parent = 0);
    ~OutputWindowPlainTextEdit();

    void appendLines(QString const& s, const QString &repository = QString());
    void appendLinesWithStyle(QString const& s, enum PHPQt5OutputWindow::MessageStyle style, const QString &repository = QString());

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

private:
    void setFormat(enum PHPQt5OutputWindow::MessageStyle style);
    QString identifierUnderCursor(const QPoint &pos, QString *repository = 0) const;

    const QTextCharFormat m_defaultFormat;
    QTextCharFormat m_errorFormat;
    QTextCharFormat m_warningFormat;
    QTextCharFormat m_commandFormat;
    QTextCharFormat m_messageFormat;
    Utils::OutputFormatter *m_formatter;
};

OutputWindowPlainTextEdit::OutputWindowPlainTextEdit(QWidget *parent) :
    Core::OutputWindow(Core::Context(C_PHPQT5_OUTPUT_PANE), parent),
    m_defaultFormat(currentCharFormat()),
    m_errorFormat(m_defaultFormat),
    m_warningFormat(m_defaultFormat),
    m_commandFormat(m_defaultFormat),
    m_messageFormat(m_defaultFormat)
{
    using Utils::Theme;
    setReadOnly(true);
    setUndoRedoEnabled(false);
    setFrameStyle(QFrame::NoFrame);
    m_errorFormat.setForeground(Utils::creatorTheme()->color(Theme::OutputPanes_ErrorMessageTextColor));
    m_warningFormat.setForeground(Utils::creatorTheme()->color(Theme::OutputPanes_WarningMessageTextColor));
    m_commandFormat.setFontWeight(QFont::Bold);
    m_messageFormat.setForeground(Utils::creatorTheme()->color(Theme::OutputPanes_MessageOutput));
    m_formatter = new Utils::OutputFormatter;
    m_formatter->setPlainTextEdit(this);
    Aggregation::Aggregate *agg = new Aggregation::Aggregate;
    agg->add(this);
    agg->add(new Core::BaseTextFind(this));
}

OutputWindowPlainTextEdit::~OutputWindowPlainTextEdit()
{
    delete m_formatter;
}

// Search back for beginning of word
static inline int firstWordCharacter(const QString &s, int startPos)
{
    for ( ; startPos >= 0 ; startPos--) {
        if (s.at(startPos).isSpace())
            return startPos + 1;
    }
    return 0;
}

QString OutputWindowPlainTextEdit::identifierUnderCursor(const QPoint &widgetPos, QString *repository) const
{
    if (repository)
        repository->clear();
    // Get the blank-delimited word under cursor. Note that
    // using "SelectWordUnderCursor" does not work since it breaks
    // at delimiters like '/'. Get the whole line
    QTextCursor cursor = cursorForPosition(widgetPos);
    const int cursorDocumentPos = cursor.position();
    cursor.select(QTextCursor::BlockUnderCursor);
    if (!cursor.hasSelection())
        return QString();
    QString block = cursor.selectedText();
    // Determine cursor position within line and find blank-delimited word
    const int cursorPos = cursorDocumentPos - cursor.block().position();
    const int blockSize = block.size();
    if (cursorPos < 0 || cursorPos >= blockSize || block.at(cursorPos).isSpace())
        return QString();
    // Retrieve repository if desired
    if (repository)
        if (QTextBlockUserData *data = cursor.block().userData())
            *repository = static_cast<const RepositoryUserData*>(data)->repository();
    // Find first non-space character of word and find first non-space character past
    const int startPos = firstWordCharacter(block, cursorPos);
    int endPos = cursorPos;
    for ( ; endPos < blockSize && !block.at(endPos).isSpace(); endPos++) ;
    return endPos > startPos ? block.mid(startPos, endPos - startPos) : QString();
}

void OutputWindowPlainTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    // Add 'open file'
    QString repository;
    const QString token = identifierUnderCursor(event->pos(), &repository);
    QAction *openAction = 0;
    if (!token.isEmpty()) {
        // Check for a file, expand via repository if relative
        QFileInfo fi(token);
        if (!repository.isEmpty() && !fi.isFile() && fi.isRelative())
            fi = QFileInfo(repository + QLatin1Char('/') + token);
        if (fi.isFile())  {
            menu->addSeparator();
            openAction = menu->addAction(PHPQt5OutputWindow::tr("Open \"%1\"").
                                         arg(QDir::toNativeSeparators(fi.fileName())));
            openAction->setData(fi.absoluteFilePath());
        }
    }
    // Add 'clear'
    menu->addSeparator();
    QAction *clearAction = menu->addAction(PHPQt5OutputWindow::tr("Clear"));

    // Run
    QAction *action = menu->exec(event->globalPos());
    if (action) {
        if (action == clearAction) {
            clear();
            return;
        }
        if (action == openAction) {
            const QString fileName = action->data().toString();
            Core::EditorManager::openEditor(fileName);
        }
    }
    delete menu;
}

void OutputWindowPlainTextEdit::appendLines(QString const& s, const QString &repository)
{
    if (s.isEmpty())
        return;

    const int previousLineCount = document()->lineCount();

    const QChar newLine(QLatin1Char('\n'));
    const QChar lastChar = s.at(s.size() - 1);
    const bool appendNewline = (lastChar != QLatin1Char('\r') && lastChar != newLine);
    m_formatter->appendMessage(appendNewline ? s + newLine : s, currentCharFormat());

    // Scroll down
    moveCursor(QTextCursor::End);
    ensureCursorVisible();
    if (!repository.isEmpty()) {
        // Associate repository with new data.
        QTextBlock block = document()->findBlockByLineNumber(previousLineCount);
        for ( ; block.isValid(); block = block.next())
            block.setUserData(new RepositoryUserData(repository));
    }
}

void OutputWindowPlainTextEdit::appendLinesWithStyle(QString const& s, enum PHPQt5OutputWindow::MessageStyle style, const QString &repository)
{
    setFormat(style);

    if (style == PHPQt5OutputWindow::Command) {
        const QString timeStamp = QTime::currentTime().toString(QLatin1String("\nHH:mm "));
        appendLines(timeStamp + s, repository);
    }
    else {
        appendLines(s, repository);
    }

    setCurrentCharFormat(m_defaultFormat);
}

void OutputWindowPlainTextEdit::setFormat(enum PHPQt5OutputWindow::MessageStyle style)
{
    switch (style) {
    case PHPQt5OutputWindow::Warning:
        setCurrentCharFormat(m_warningFormat);
        break;
    case PHPQt5OutputWindow::Error:
        setCurrentCharFormat(m_errorFormat);
        break;
    case PHPQt5OutputWindow::Message:
        setCurrentCharFormat(m_messageFormat);
        break;
    case PHPQt5OutputWindow::Command:
        setCurrentCharFormat(m_commandFormat);
        break;
    default:
    case PHPQt5OutputWindow::None:
        setCurrentCharFormat(m_defaultFormat);
        break;
    }
}

} // namespace Internal

// ------------------- VcsBaseOutputWindowPrivate
class PHPQt5OutputWindowPrivate
{
public:
    Internal::OutputWindowPlainTextEdit widget;
};

static QLocalServer *m_server = 0;
static PHPQt5OutputWindow *m_instance = 0;
static PHPQt5OutputWindowPrivate *d = 0;

PHPQt5OutputWindow::PHPQt5OutputWindow()
{
    d = new PHPQt5OutputWindowPrivate;
    m_instance = this;

    if (!m_server)
        m_server = new QLocalServer(this);

    m_server->listen(QStringLiteral("PQEngine Debug Server"));
    connect(m_server, SIGNAL(newConnection()),
            this, SLOT(incommingConnection()));
}

PHPQt5OutputWindow::~PHPQt5OutputWindow()
{
    m_instance = 0;
    delete d;
}

QWidget *PHPQt5OutputWindow::outputWidget(QWidget *parent)
{
    if (parent != d->widget.parent())
        d->widget.setParent(parent);
    return &d->widget;
}

QList<QWidget *> PHPQt5OutputWindow::toolBarWidgets() const
{
    return {};
}

QString PHPQt5OutputWindow::displayName() const
{
    return tr("PHPQt5 Debugger");
}

int PHPQt5OutputWindow::priorityInStatusBar() const
{
    return -1;
}

void PHPQt5OutputWindow::clearContents()
{
    d->widget.clear();
}

void PHPQt5OutputWindow::visibilityChanged(bool visible)
{
    if (visible)
        d->widget.setFocus();
}

void PHPQt5OutputWindow::setFocus()
{
}

bool PHPQt5OutputWindow::hasFocus() const
{
    return false;
}

bool PHPQt5OutputWindow::canFocus() const
{
    return false;
}

bool PHPQt5OutputWindow::canNavigate() const
{
    return false;
}

bool PHPQt5OutputWindow::canNext() const
{
    return false;
}

bool PHPQt5OutputWindow::canPrevious() const
{
    return false;
}

void PHPQt5OutputWindow::goToNext()
{
}

void PHPQt5OutputWindow::goToPrev()
{
}

PHPQt5OutputWindow *PHPQt5OutputWindow::instance()
{
    if (!m_instance)
        (void) new PHPQt5OutputWindow;
    return m_instance;
}

void PHPQt5OutputWindow::incommingConnection()
{
    QLocalSocket *socket = m_server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(readDebugData()));

    //$this->sockets[] = $socket;
}

void PHPQt5OutputWindow::appendText(const QString &text)
{
    d->widget.appendLinesWithStyle(text, MessageStyle::None, "");
}

void PHPQt5OutputWindow::readDebugData()
{
    QLocalSocket *socket = (QLocalSocket *)sender();
    m_debugDataBuffer = QLatin1String(socket->readAll());

    int pos = m_debugDataBuffer.indexOf(":::%");

    while (pos > 0) {
        QStringList data = m_debugDataBuffer.mid(0, pos).split(":::");
        m_debugDataBuffer = m_debugDataBuffer.mid(pos+4);

        QString clientEngineId = data.at(0);
        QString clientAppCoreName = data.at(1);
        QString thread = data.at(2);
        QString debugLevel = data.at(3);
        QString debugTitle = data.at(4);
        QString debugMessage = QString(data.at(5)).replace("\r","").replace("\n","");

        d->widget.appendLinesWithStyle(QString("%1").arg(debugMessage), MessageStyle::None, clientAppCoreName);

        pos = m_debugDataBuffer.indexOf(":::%");
    }
}

} // namespace
