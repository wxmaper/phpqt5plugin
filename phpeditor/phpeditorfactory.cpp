#include "phpeditorfactory.h"
#include "../phpqt5constants.h"
#include "phpeditorplugin.h"
#include "phpindenter.h"
#include "phphighlighter.h"
#include "phpautocompleter.h"
#include "fontsettings.h"
#include "phpformattoken.h"
#include "phpcompletionassistprovider.h"

#include <coreplugin/fileiconprovider.h>
#include <coreplugin/editormanager/editormanager.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <texteditor/texteditoractionhandler.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/textdocument.h>

#include <utils/qtcassert.h>
#include <utils/mimetypes/mimedatabase.h>
#include <utils/mimetypes/mimemagicrule_p.h>
#include <utils/mimetypes/mimetype.h>
#include <utils/uncommentselection.h>

#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

#include "phpeditor.h"
#include "phpeditordocument.h"

using namespace TextEditor;

namespace PHPQt5 {
namespace Internal {

PHPEditorFactory::PHPEditorFactory()
{
    loadSyntaxRules();

    setId(Constants::C_PHPEDITOR_ID);
    setDisplayName(QCoreApplication::translate("OpenWith::Editors", Constants::C_EDITOR_DISPLAY_NAME));
    addMimeType(Constants::C_PHP_MIMETYPE);

    setDocumentCreator([]() { return new PHPEditorDocument(Constants::C_PHPEDITOR_ID); });
    setEditorWidgetCreator([]() { return new PHPEditorWidget; });
    setEditorCreator([]() { return new PHPEditor(); });
    setSyntaxHighlighterCreator([=] { return new PHPHighlighter(m_words,
                                                                m_fontFormats,
                                                                m_formatKeys,
                                                                m_formats,
                                                                m_savedState,
                                                                &m_dvp); });

    setCommentStyle(Utils::CommentDefinition::CppStyle);
    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);

    setIndenterCreator([=] { return new PHPIndenter(&m_dvp); });
    setAutoCompleterCreator([]() { return new PHPAutoCompleter; });
    setCompletionAssistProvider(new PHPCompletionAssistProvider(&m_words,
                                                                &m_functions,
                                                                &m_dvp));
    //    setAutoCompleterCreator([]() {
    //        AutoCompleter *ac = new AutoCompleter;
    //        ac->setAutoInsertBracketsEnabled(true);
    //        ac->setAutoInsertQuotesEnabled(true);
    //        ac->setSurroundWithBracketsEnabled(true);
    //        ac->setSurroundWithQuotesEnabled(true);
    //        return ac;
    //    });

    setEditorActionHandlers(TextEditorActionHandler::Format
                            | TextEditorActionHandler::UnCommentSelection
                            | TextEditorActionHandler::UnCollapseAll
                            | TextEditorActionHandler::FollowSymbolUnderCursor);

    Core::FileIconProvider::registerIconOverlayForSuffix(
                Constants::C_PHP_ICON_PATH, "php");
}

void PHPEditorFactory::loadSyntaxRules()
{
    static const QLatin1String xmlTagList = QLatin1String("list");
    static const QLatin1String xmlTagItem = QLatin1String("item");
    static const QLatin1String xmlTagArgs = QLatin1String("args");
    static const QLatin1String xmlTagFormats = QLatin1String("formats");
    static const QLatin1String xmlTagFormat = QLatin1String("format");
    static const QLatin1String xmlTagFargsList = QLatin1String("fargslist");
    static const QLatin1String xmlAttrName = QLatin1String("name");
    static const QLatin1String xmlAttrFormatKey = QLatin1String("formatKey");
    static const QLatin1String xmlAttrItalic = QLatin1String("italic");
    static const QLatin1String xmlAttrUnderline = QLatin1String("underline");
    static const QLatin1String xmlAttrBold = QLatin1String("bold");
    static const QLatin1String xmlAttrBackground = QLatin1String("background");
    static const QLatin1String xmlAttrForeground = QLatin1String("foreground");

    QDir phpqt5path(QApplication::applicationDirPath());
    phpqt5path.cd("../../PHPQt5/");

    QFile file(phpqt5path.absoluteFilePath("phpqt5.xml"));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "Could not open file" << file.fileName();
        return;
    }

    QString formatName;
    QString functionName;
    QStringList functionArgs;

    enum In { Unknown, List, Formats, FargsList, FargsListItem };
    In in = In::Unknown;

    QXmlStreamReader reader(&file);
    while (!reader.atEnd()) {
        switch (reader.readNext()) {
        case QXmlStreamReader::StartElement:
            /// <list>
            if (reader.name() == xmlTagList) {
                const QXmlStreamAttributes attributes = reader.attributes();
                formatName = attributes.value(xmlAttrName).toString().toLower();

                if (formatName.isEmpty()) {
                    qWarning() << Q_FUNC_INFO << "Could not parse" << file.fileName()
                               << ": stop at line" << reader.lineNumber();
                    return;
                }

                in = In::List;
            }
            else if(in == In::List && reader.name() == xmlTagItem) {
                reader.readNext();
                m_words.insert(reader.text().toString(), formatName);
            }

            /// <formats>
            else if(reader.name() == xmlTagFormats) {
                in = In::Formats;
            }
            else if(in == In::Formats && reader.name() == xmlTagFormat) {
                FontFormat format;

                const QXmlStreamAttributes attributes = reader.attributes();
                if (attributes.hasAttribute(xmlAttrName) && attributes.hasAttribute(xmlAttrFormatKey)) {
                    const QString formatName = attributes.value(xmlAttrName).toString().toLower();

                    if(attributes.hasAttribute(xmlAttrBackground))
                        format.background = QBrush(QColor(attributes.value(xmlAttrBackground).toString()));
                    if(attributes.hasAttribute(xmlAttrForeground))
                        format.foreground = QBrush(QColor(attributes.value(xmlAttrForeground).toString()));
                    if(attributes.hasAttribute(xmlAttrBold))
                        format.bold = attributes.value(xmlAttrBold).toInt() == 0 ? false : true;
                    if(attributes.hasAttribute(xmlAttrItalic))
                        format.italic = attributes.value(xmlAttrItalic).toInt() == 0 ? false : true;
                    if(attributes.hasAttribute(xmlAttrUnderline))
                        format.underline = attributes.value(xmlAttrUnderline).toInt() == 0 ? false : true;

                    m_formatKeys.insert(Format(attributes.value(xmlAttrFormatKey).toInt()), formatName);
                    m_fontFormats.insert(formatName, format);
                }
            }

            /// <fargslist>
            else if(reader.name() == xmlTagFargsList) {
                in = In::FargsList;
                //qDebug() << "In::FargsList";
            }
            else if(in == In::FargsList && reader.name() == xmlTagItem) {
                in = In::FargsListItem;
                //qDebug() << "In::FargsListItem";
                const QXmlStreamAttributes attributes = reader.attributes();
                if (attributes.hasAttribute(xmlAttrName)) {
                    functionName = attributes.value(xmlAttrName).toString().toLower();
                    //qDebug() << "In::FargsListItem" << functionName;
                }
                else functionName.clear();
            }
            else if(in == In::FargsListItem && reader.name() == xmlTagArgs && !functionName.isEmpty()) {
                reader.readNext();
                functionArgs << reader.text().toString();
                //qDebug() << "xmlTagArgs:" << reader.text().toString();
            }

            break;

        case QXmlStreamReader::EndElement:
            if (reader.name() == xmlTagList
                    || reader.name() == xmlTagFormats
                    || reader.name() == xmlTagFargsList) {
                in = In::Unknown;
                functionName.clear();
                functionArgs.clear();
            }
            else if(in == In::FargsListItem && reader.name() == xmlTagItem) {
                in = In::FargsList;
                m_functions.insert(functionName, functionArgs);
                functionName.clear();
                functionArgs.clear();
            }

            break;

        default: // nothing
            break;
        }
    }

    if (!m_formatKeys.contains(Format_Text)) {
        m_formatKeys.insert(Format_Text, C_DEFAULT_FORMAT_NAME);
    }

    if (!m_fontFormats.contains(C_DEFAULT_FORMAT_NAME)) {
        m_fontFormats.insert(C_DEFAULT_FORMAT_NAME, FontFormat());
    }
}

} // namespace Internal
} // namespace PHPQt5
