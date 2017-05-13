#ifndef CLASSHELPERS_H
#define CLASSHELPERS_H

#include <QCoreApplication>
#include <QDirIterator>
#include <QHash>
#include <QFile>
#include <QDir>
#include <QDebug>

namespace PHPQt5 {
class ClassHelpers
{
public:
    static bool isClass(const QString &s, QString *path) {
        static QHash<QString, QString> m_classes;

        if (m_classes.contains(s)) {
            if (path) {
                *path = m_classes.value(s);
            }
            return true;
        }

        if ((s.length() >= 2 && s.startsWith(QLatin1Char('Q')) && s.at(1).isUpper())
                || s == "Qt") {
            QDir dir(qApp->applicationDirPath());
            dir.cd("../../PHPQt5/plastiq/include/");
            dir.setFilter(QDir::NoDotAndDotDot);

            if (dir.exists()) {
                QDirIterator it(dir.path());

                while (it.hasNext()) {
                    QDir lPath(QString("%1/%2").arg(it.next()).arg(QString("PlastiQ%1").arg(s)));

                    if (lPath.exists()) {
                        QFile cFile(QString("%1/plastiq%2.cpp").arg(lPath.path()).arg(s.toLower()));

                        if (cFile.exists()) {
                            m_classes.insert(s, cFile.fileName());

                            if (path) {
                                *path = cFile.fileName();
                            }

                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

};
} // namespace PHPQt5

#endif // CLASSHELPERS_H
