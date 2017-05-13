#include "phpautocompleter.h"

#include "../debugger/phpqt5debugger.h"

#include <QChar>
#include <QLatin1Char>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>

#include <texteditor/tabsettings.h>
#include "texteditor/textdocumentlayout.h"

using namespace PHPQt5;
using namespace Internal;

static int blockStartState(const QTextBlock &block)
{
    int state = block.previous().userState();

    if (state == -1)
        return 0;
    else
        return state & 0xff;
}

/*
static Token tokenUnderCursor(const QTextCursor &cursor)
{
    const QString blockText = cursor.block().text();
    const int blockState = blockStartState(cursor.block());

    Scanner tokenize;
    const QList<Token> tokens = tokenize(blockText, blockState);
    const int pos = cursor.positionInBlock();

    int tokenIndex = 0;
    for (; tokenIndex < tokens.size(); ++tokenIndex) {
        const Token &token = tokens.at(tokenIndex);

        if (token.is(Token::Comment) || token.is(Token::String)) {
            if (pos > token.begin() && pos <= token.end())
                break;
        } else {
            if (pos >= token.begin() && pos < token.end())
                break;
        }
    }

    if (tokenIndex != tokens.size())
        return tokens.at(tokenIndex);

    return Token();
}
*/

static bool shouldInsertMatchingText(QChar lookAhead)
{
    switch (lookAhead.unicode()) {
    case '{': case '}':
    case ']': case ')':
    case ';': case ',':
    case '"': case '\'':
        return true;

    default:
        if (lookAhead.isSpace())
            return true;

        return false;
    } // switch
}

static bool shouldInsertMatchingText(const QTextCursor &tc)
{
    QTextDocument *doc = tc.document();
    return shouldInsertMatchingText(doc->characterAt(tc.selectionEnd()));
}

static bool shouldInsertNewline(const QTextCursor &tc)
{
    QTextDocument *doc = tc.document();
    int pos = tc.selectionEnd();

    // count the number of empty lines.
    int newlines = 0;
    for (int e = doc->characterCount(); pos != e; ++pos) {
        const QChar ch = doc->characterAt(pos);

        if (! ch.isSpace())
            break;
        else if (ch == QChar::ParagraphSeparator)
            ++newlines;
    }

    if (newlines <= 1 && doc->characterAt(pos) != QLatin1Char('}'))
        return true;

    return false;
}

static bool isCompleteStringLiteral(const QStringRef &text)
{
    if (text.length() < 2)
        return false;

    const QChar quote = text.at(0);

    if (text.at(text.length() - 1) == quote)
        return text.at(text.length() - 2) != QLatin1Char('\\'); // ### not exactly.

    return false;
}



bool PHPAutoCompleter::contextAllowsAutoBrackets(const QTextCursor &cursor,
                                                 const QString &textToInsert) const
{
    //qDebug() << "contextAllowsAutoBrackets > " << textToInsert;
    QChar ch;

    if (! textToInsert.isEmpty())
        ch = textToInsert.at(0);

    switch (ch.unicode()) {
    case '(':
    case '[':
    case '{':
    case ')':
    case ']':
    case '}':

    case ';':
        break;

    default:
        if (ch.isNull())
            break;

        return false;
    } // end of switch

    //qDebug() << "contextAllowsAutoBrackets true ";

    /*
    const Token token = tokenUnderCursor(cursor);
    switch (token.kind) {
    case Token::Comment:
        return false;

    case Token::RightBrace:
        return false;

    case Token::String: {
        const QString blockText = cursor.block().text();
        const QStringRef tokenText = blockText.midRef(token.offset, token.length);
        QChar quote = tokenText.at(0);
        // if a string literal doesn't start with a quote, it must be multiline
        if (quote != QLatin1Char('"') && quote != QLatin1Char('\'')) {
            const int startState = blockStartState(cursor.block());
            if ((startState & Scanner::MultiLineMask) == Scanner::MultiLineStringDQuote)
                quote = QLatin1Char('"');
            else if ((startState & Scanner::MultiLineMask) == Scanner::MultiLineStringSQuote)
                quote = QLatin1Char('\'');
        }

        // never insert ' into string literals, it adds spurious ' when writing contractions
        if (ch == QLatin1Char('\''))
            return false;

        if (ch != quote || isCompleteStringLiteral(tokenText))
            break;

        return false;
    }

    default:
        break;
    } // end of switch
    */

    return true;
}

bool PHPAutoCompleter::contextAllowsAutoQuotes(const QTextCursor &cursor,
                                               const QString &textToInsert) const
{
    //qDebug() << "contextAllowsAutoQuotes ";
    if (!isQuote(textToInsert))
        return false;

    /*
    const Token token = tokenUnderCursor(cursor);
    switch (token.kind) {
    case Token::Comment:
        return false;

    case Token::RightBrace:
        return false;

    case Token::String: {
        const QString blockText = cursor.block().text();
        const QStringRef tokenText = blockText.midRef(token.offset, token.length);
        QChar quote = tokenText.at(0);
        // if a string literal doesn't start with a quote, it must be multiline
        if (quote != QLatin1Char('"') && quote != QLatin1Char('\'')) {
            const int startState = blockStartState(cursor.block());
            if ((startState & Scanner::MultiLineMask) == Scanner::MultiLineStringDQuote)
                quote = QLatin1Char('"');
            else if ((startState & Scanner::MultiLineMask) == Scanner::MultiLineStringSQuote)
                quote = QLatin1Char('\'');
        }

        // never insert ' into string literals, it adds spurious ' when writing contractions
        if (textToInsert.at(0) == QLatin1Char('\''))
            return false;

        if (textToInsert.at(0) != quote || isCompleteStringLiteral(tokenText))
            break;

        return false;
    }

    default:
        break;
    } // end of switch
    */

    return true;
}

bool PHPAutoCompleter::contextAllowsElectricCharacters(const QTextCursor &cursor) const
{
    //qDebug() << "contextAllowsElectricCharacters ";
    return true; //CPlusPlus::MatchingText::contextAllowsElectricCharacters(cursor);
}

bool PHPAutoCompleter::isInComment(const QTextCursor &cursor) const
{
    return false;//CPlusPlus::MatchingText::isInCommentHelper(cursor);
}

QString PHPAutoCompleter::insertMatchingBrace(const QTextCursor &cursor, const QString &text,
                                              QChar lookAhead, bool skipChars, int *skippedChars) const
{
    //qDebug() << "insertMatchingBrace ";

    if (text.length() != 1)
        return QString();

    if (! shouldInsertMatchingText(cursor))
        return QString();

    const QChar ch = text.at(0);
    switch (ch.unicode()) {
    case '(':
        return QString(QLatin1Char(')'));

    case '[':
        return QString(QLatin1Char(']'));

    case '{':
        return QString(); // nothing to do.

    case ')':
    case ']':
    case '}':
    case ';':
        if (lookAhead == ch && skipChars)
            ++*skippedChars;
        break;

    default:
        break;
    } // end of switch

    return QString();
}

QString PHPAutoCompleter::insertMatchingQuote(const QTextCursor &cursor, const QString &text,
                                              QChar lookAhead, bool skipChars, int *skippedChars) const
{
    if (isQuote(text)) {
        if (lookAhead == text && skipChars) {
            ++*skippedChars;
        }
        // кавычку ставить только перед пробелом или в конце строки,
        // но при условии, что кол-во кавычек в строке нечетное
        else if ((lookAhead.isSpace() || lookAhead == '\r' || lookAhead == '\n')
                 && !(cursor.block().text().count(text) % 2)){
            return text;
        }
    }

    return QString();
}

QString PHPAutoCompleter::insertParagraphSeparator(const QTextCursor &cursor) const
{
    if (shouldInsertNewline(cursor)) {
        QTextCursor selCursor = cursor;
        selCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        if (!selCursor.selectedText().trimmed().isEmpty())
            return QString();

        return QLatin1String("}\n");
    }

    return QLatin1String("}");
}

int PHPAutoCompleter::paragraphSeparatorAboutToBeInserted(QTextCursor &cursor, const TextEditor::TabSettings &tabSettings)
{
    QTextDocument *doc = cursor.document();
    if (doc->characterAt(cursor.position() - 1) != QLatin1Char('{'))
        return 0;

    if (!contextAllowsAutoBrackets(cursor))
        return 0;

    // verify that we indeed do have an extra opening brace in the document
    QTextBlock block = cursor.block();
    const QString textFromCusror = block.text().mid(cursor.positionInBlock()).trimmed();
    int braceDepth = TextEditor::TextDocumentLayout::braceDepth(doc->lastBlock());

    // if (braceDepth <= 0 && (textFromCusror.isEmpty() || textFromCusror.at(0) != QLatin1Char('}'))) // IT'S BUG !?
    if (braceDepth <= 0 && (textFromCusror.isEmpty() || textFromCusror.at(0) == QLatin1Char('}')))
        return 0; // braces are all balanced or worse, no need to do anything and separator inserted not between '{' and '}'

    // we have an extra brace , let's see if we should close it

    /* verify that the next block is not further intended compared to the current block.
       This covers the following case:

            if (condition) {|
                statement;
    */
    int indentation = tabSettings.indentationColumn(block.text());

    if (block.next().isValid()) { // not the last block
        block = block.next();
        //skip all empty blocks
        while (block.isValid() && tabSettings.onlySpace(block.text()))
            block = block.next();
        if (block.isValid()
                && tabSettings.indentationColumn(block.text()) > indentation)
            return 0;
    }

    const QString &textToInsert = insertParagraphSeparator(cursor);
    int pos = cursor.position();
    cursor.insertBlock();
    cursor.insertText(textToInsert);
    cursor.setPosition(pos);

    return 1;
}

