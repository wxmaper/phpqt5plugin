#ifndef PHPAUTOCOMPLETER_H
#define PHPAUTOCOMPLETER_H

#include <texteditor/autocompleter.h>

namespace PHPQt5 {
namespace Internal {

class PHPAutoCompleter : public TextEditor::AutoCompleter
{
public:
    PHPAutoCompleter() {}

    bool contextAllowsAutoBrackets(const QTextCursor &cursor,
                                   const QString &textToInsert = QString()) const override;
    bool contextAllowsAutoQuotes(const QTextCursor &cursor,
                                 const QString &textToInsert = QString()) const override;
    bool contextAllowsElectricCharacters(const QTextCursor &cursor) const override;
    bool isInComment(const QTextCursor &cursor) const override;
    QString insertMatchingBrace(const QTextCursor &tc,
                                const QString &text,
                                QChar lookAhead,
                                bool skipChars,
                                int *skippedChars) const override;
    QString insertMatchingQuote(const QTextCursor &tc,
                                const QString &text,
                                QChar lookAhead,
                                bool skipChars,
                                int *skippedChars) const override;
    QString insertParagraphSeparator(const QTextCursor &tc) const override;

    // Hook to insert special characters on enter. Returns the number of extra blocks inserted.
    int paragraphSeparatorAboutToBeInserted(QTextCursor &cursor,
                                                    const TextEditor::TabSettings &tabSettings) override;
};

} // namespace Internal
} // namespace PHPQt5

#endif // PHPAUTOCOMPLETER_H
