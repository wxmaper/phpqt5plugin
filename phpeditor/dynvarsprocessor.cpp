#include "dynvarsprocessor.h"

#include <utils/algorithm.h>

namespace PHPQt5 {
namespace PHPEditor {

DynVarsProcessor::DynVarsProcessor()
{

}

void DynVarsProcessor::append(const QString &v) {
    if (!m_list.contains(v)) {
        m_list << v;
        Utils::sort(m_list);
    }
}

void DynVarsProcessor::append(const QStringList &l)
{
    m_list.append(l);
    Utils::sort(m_list);
}

void DynVarsProcessor::clear()
{

}

} // namespace PHPEditor
} // namespace PHPQt5
