#ifndef UTIL_RES_DIR_H
#define UTIL_RES_DIR_H

#include <QtCore/QString>

namespace util { class ResDir; }

class util::ResDir
{
public:
    static void show_rc_by_path(const QString& path, const QString& filter);
    static void print_res_tree (const QString& path, const QString& prefix = "");

    static void list_qrc_files(const QString& path = ":/");
};

#endif // UTIL_RES_DIR_H
