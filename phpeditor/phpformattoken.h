#ifndef PHPFORMATTOKEN_H
#define PHPFORMATTOKEN_H

namespace PHPQt5 {
namespace Internal {

enum Format {
    Format_Text = 0,
    Format_Number,
    Format_String, // special
    Format_HardString,  // special
    Format_Keyword,
    Format_Type,
    Format_Class,
    Format_Function,
    Format_SpecMethod,
    Format_SpecVar,
    Format_Variable,
    Format_Constant,
    Format_ControlStruct,
    Format_Operator,
    Format_Comment, // special
    Format_Doxygen, // special
    Format_Identifier,
    Format_Whitespace,
    Format_Heredoc, // special
    Format_HardHeredoc, // special
    Format_Shielded,
    Format_Deprecated,
    Format_PHPQt5Function,
    Format_PHPQt5Class,
    Format_PHPQt5Constant,
    Format_PHPQt5Keyword,
    Format_PHPQt5Event,
    Format_PHPQt5OnMethod,
    Format_Annotation,
    Format_DoxyVar,
    Format_DoxyType,

    Format_User1 = 50,
    Format_User2,
    Format_User3,
    Format_User4,
    Format_User5,
    Format_User6, // special
    Format_User7, // special
    Format_User8, // special
    Format_User9, // special
    Format_User10, // special

    Format_Error = 100,
    Format_Warning,
    Format_FormatsAmount,
    Format_EndOfBlock
};

class FormatToken
{
public:
    FormatToken() {}

    FormatToken(Format format, int position, int length)
        : m_format(format), m_position(position), m_length(length)
    {}

    Format format() const { return m_format; }
    int begin() const { return m_position; }
    int end() const { return m_position + m_length; }
    int length() const { return m_length; }

private:
    Format m_format;
    int m_position;
    int m_length;
};


} // namespace Internal
} // namespace PHPQt5

#endif // PHPFORMATTOKEN_H
