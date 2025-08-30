#include "utilities/res_dir.h"

#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>

void util::ResDir::show_rc_by_path(const QString& path, const QString& filter)
{
    QDirIterator it(path, QDir::NoDotAndDotDot | QDir::Dirs);
    while (it.hasNext()) {
        QString folder = it.next();
        show_rc_by_path(folder, filter);
    }

    QDirIterator fileIt(path, QStringList() << filter, QDir::Files);
    while (fileIt.hasNext()) {
        QString file = fileIt.next();
        qDebug() << "Found file:" << file;
    }
}

void util::ResDir::print_res_tree(const QString& path, const QString& prefix)
{
    QDirIterator it(path, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        it.next();
        const QString& name = it.fileName();
        const QString& type = it.fileInfo().isDir() ? "dir" : "file";
        qDebug() << prefix << name << "(" << type << ")";
        if (it.fileInfo().isDir()) {
            const QString& subdirPath = it.filePath();
            const QString& subPrefix = prefix + "  ";
            print_res_tree(subdirPath, subPrefix);
        }
    }
}

void util::ResDir::list_qrc_files(const QString& path)
{
    QDirIterator it(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);

    while (it.hasNext())
        qDebug() << it.next();
}
