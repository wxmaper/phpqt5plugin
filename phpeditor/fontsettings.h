#ifndef FONTSETTINGS_H
#define FONTSETTINGS_H

#include <QBrush>
#include <QColor>

namespace PHPQt5 {
namespace Internal {

struct FontFormat {
    FontFormat() {}
    FontFormat(const QBrush &f) :
        foreground(f) {}
    FontFormat(const QBrush &f, const QBrush &b) :
        foreground(f), background(b) {}

    QBrush foreground = QBrush(Qt::black);
    QBrush background = QBrush(Qt::transparent);
    bool bold = false;
    bool italic = false;
    bool underline = false;
};

} // namespace Internal
} // namespace PHPQt5
#endif // FONTSETTINGS_H
