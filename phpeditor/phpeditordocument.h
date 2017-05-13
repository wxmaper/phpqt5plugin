#ifndef PHPEDITORDOCUMENT_H
#define PHPEDITORDOCUMENT_H

#include <texteditor/textdocument.h>
#include "../phpqt5constants.h"

namespace PHPQt5 {
namespace Internal {
using namespace TextEditor;

class PHPEditorDocument : public TextEditor::TextDocument
{
    Q_OBJECT

public:
    explicit PHPEditorDocument(Core::Id id = Core::Id(Constants::C_PHPEDITOR_ID));
};

}
}

#endif // PHPEDITORDOCUMENT_H
