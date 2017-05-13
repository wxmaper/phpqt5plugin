#ifndef SOURCECODESTREAM_H
#define SOURCECODESTREAM_H

#include <QChar>
#include <QString>

namespace PHPQt5 {
namespace Internal {

class SourceCodeStream
{
public:
    SourceCodeStream(const QChar *text, int length)
        : m_text(text),
          m_textLength(length),
          m_position(0),
          m_markedPosition(0),
          m_line(0),
          m_linePos(0)
    {}

    inline void setAnchor(int offset = 0)
    {
        m_markedPosition = m_position + offset;
    }

    inline void move(int amount = 1)
    {
        for (int i = amount; i > 0; i--) {
            m_position++;

            if (m_newLine) {
                m_newLine = false;
                m_linePos = 0;
            }
            else m_linePos++;

            if (peek() == '\n') {
                m_line++;
                m_newLine = true;
            }
        }
        //m_position += amount;
    }

    inline void moveToEnd()
    {
        m_position = m_textLength;
    }

    inline int pos() const
    {
        return m_position;
    }

    inline int length() const
    {
        return m_position - m_markedPosition;
    }

    inline int anchor() const
    {
        return m_markedPosition;
    }

    inline bool isEnd() const
    {
        return m_position >= m_textLength;
    }

    inline QChar peek(int offset = 0) const
    {
        int pos = m_position + offset;
        if (pos >= m_textLength)
            return QLatin1Char('\0');
        return m_text[pos];
    }

    inline QString value() const
    {
        const QChar *start = m_text + m_markedPosition;
        return QString(start, length());
    }

    inline QString value(int begin, int length) const
    {
        return QString(m_text + begin, length);
    }

    inline int sourceLength() const
    {
        return m_textLength;
    }

    inline int linePos() {
        return m_linePos;
    }

    inline int line() {
        return m_line;
    }

private:
    const QChar *m_text;
    const int m_textLength;
    int m_position;
    int m_markedPosition;
    int m_line;
    int m_linePos;

    bool m_newLine = false;
};

}
}

#endif // SOURCECODESTREAM_H
