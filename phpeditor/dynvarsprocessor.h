#ifndef DYNVARSPROCESSOR_H
#define DYNVARSPROCESSOR_H

#include <QStringList>



namespace PHPQt5 {
namespace PHPEditor {

class DynVarsProcessor
{
public:
    DynVarsProcessor();

    void append(const QString &v);
    void append(const QStringList &l);
    void clear();

    QStringList list() const { return m_list; }

private:
    QStringList m_list;
};

} // namespace PHPEditor
} // namespace PHPQt5

#endif // DYNVARSPROCESSOR_H
