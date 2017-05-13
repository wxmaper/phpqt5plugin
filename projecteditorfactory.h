#ifndef PROJECTEDITORFACTORY_H
#define PROJECTEDITORFACTORY_H

#include <coreplugin/editormanager/ieditorfactory.h>

namespace PHPQt5 {
namespace Internal {

class ProjectEditorFactory : public Core::IEditorFactory
{
    Q_OBJECT

public:
    explicit ProjectEditorFactory(QObject *parent = 0);

    Core::IEditor *createEditor();
};

} // namespace Internal
} // namespace PHPQt5

#endif // PROJECTEDITORFACTORY_H
